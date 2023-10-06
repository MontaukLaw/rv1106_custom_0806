#include "common.h"
#include "comm_type.h"
#include "param.h"
#include "comm_codec.h"
#include "comm_app.h"
#include "uuid.h"
#include "param.h"
#include "comm_net.h"
#include "comm_misc.h"
#include "taskLib.h"

#define IP_SIZE	16

/* Media types. */
enum {
    RTP_MEDIA_TYPE_VIDEO = 0,
    RTP_MEDIA_TYPE_AUDIO = 1,
};
	
/* RTP transport type */
enum {
    RTP_TP_TYPE_TCP   = 1,
    RTP_TP_TYPE_UDP   = 2,
    RTP_TP_TYPE_MCAST = 3,
};

enum {
    RTP_PT_VIDEO_H264 = 96,
    RTP_PT_AUDIO_PCMU = 0,
    RTP_PT_AUDIO_PCMA = 8,
};

/* NALU payload types in RTP packets */
enum {
    NALU_PT_FU_A = 28,
    NALU_PT_FU_B = 29,
};

/* NALU types */
enum {
    NALU_TYPE_SPS = 7,          /* sequence parameter set */
    NALU_TYPE_PPS = 8,          /* picture parameter set */
};

#define RTP_MAX_PACKET_SIZE		1372
#define RTP_MAX_PAYLOAD_SIZE	(RTP_MAX_PACKET_SIZE - 12)	/* minus sizeof(rtp_hdr_t) */

/* Interleaved header for transport(RTP over RTSP). */
typedef struct intlvd_s {
        unsigned char dollar;
        unsigned char chn;
        unsigned short size;
} intlvd_t;

typedef struct rtp_hdr_s {

	unsigned char cc:4;
	unsigned char x:1;
	unsigned char p:1;
	unsigned char v:2;
	unsigned char pt:7;
	unsigned char m:1;
	unsigned short seq;			/* sequence number */
	unsigned int   ts;          /* timestamp */
	unsigned int   ssrc;        /* synchronization source */
//	unsigned int   csrc[2];		/* optional CSRC list */
} rtp_hdr_t;

typedef struct rtp_pkt_s {
	rtp_hdr_t rtp_hdr;
	uint8_t data[2048];
} rtp_pkt_t;

typedef struct
{
	int sockfd;
	char addr[24];
	int port;
	int chann_no;
	int chann_type;
	read_pos_t read_pos;		//读指针
	rtp_hdr_t rtp_hdr;
	int max_payload_size;
	pthread_mutex_t mutex;
	pthread_t tid;
	int run_flag;
	struct sockaddr_in address;
	net_frame_t *frame;
	int64_t pts;
}rtp_session_t;
#define MAX_CAM_NUM  2
#define MAX_CAM_STREAM_NUM	3

rtp_session_t g_rtp_session[MAX_CAM_NUM][MAX_CAM_STREAM_NUM];


void comm_init_multiRtp()
{
	int chann_no;
	int stream_no;
	rtp_session_t *session_p = NULL;
	for(chann_no = 0; chann_no < MAX_CAM_NUM;chann_no++)
	{
		for(stream_no =0; stream_no < MAX_CAM_STREAM_NUM;stream_no++)
		{
			session_p = &g_rtp_session[chann_no][stream_no];
			memset(session_p,0,sizeof(rtp_session_t));
			session_p->sockfd = INVALID_SOCKET_VALUE;
			session_p->run_flag = OS_FALSE;
			os_mutex_init(&session_p->mutex);
			session_p->max_payload_size = RTP_MAX_PAYLOAD_SIZE;
			session_p->chann_no = chann_no;
			session_p->chann_type = stream_no;
		}
	}
}

static const uint8_t *h264_find_startcode_internal(const uint8_t *p, const uint8_t *end)
{
    const uint8_t *a = p + 4 - ((intptr_t)p & 3);

    for (end -= 3; p < a && p < end; p++) {
        if (p[0] == 0 && p[1] == 0 && p[2] == 1)
            return p;
    }

    for (end -= 3; p < end; p += 4) {
        uint32_t x = *(const uint32_t*)p;
//      if ((x - 0x01000100) & (~x) & 0x80008000) // little endian
//      if ((x - 0x00010001) & (~x) & 0x00800080) // big endian
        if ((x - 0x01010101) & (~x) & 0x80808080) { // generic
            if (p[1] == 0) {
                if (p[0] == 0 && p[2] == 1)
                    return p;
                if (p[2] == 0 && p[3] == 1)
                    return p+1;
            }
            if (p[3] == 0) {
                if (p[2] == 0 && p[4] == 1)
                    return p+2;
                if (p[4] == 0 && p[5] == 1)
                    return p+3;
            }
        }
    }

    for (end += 3; p < end; p++) {
        if (p[0] == 0 && p[1] == 0 && p[2] == 1)
            return p;
    }

    return end + 3;
}

const uint8_t *comm_h264_find_startcode(const uint8_t *p, const uint8_t *end)
{
    const uint8_t *out = h264_find_startcode_internal(p, end);
//    printf("%02x %02x %02x %02x %02x\n",p[0],p[1],p[2],p[3],p[4]);
    if (p < out && out < end && !out[-1]) out--;
    return out;
}
static int init_RtpHdr(rtp_hdr_t *hdrp, int media_type)
{
    if (!hdrp)
        return -1;

    hdrp->v = 2;
    hdrp->p = 0;
    hdrp->x = 0;
    hdrp->cc = 0;
    hdrp->m = 0;
    hdrp->pt = (media_type == RTP_MEDIA_TYPE_VIDEO) ?
               RTP_PT_VIDEO_H264 : RTP_PT_AUDIO_PCMU;
    hdrp->seq = random();
    hdrp->ts = random();
    hdrp->ssrc = random();

    return 0;
}

static void increase_seq_number(unsigned short *seqp)
{
    *seqp = htons(ntohs(*seqp) + 1);
    return;
}

static int udp_send_rtp(rtp_session_t *sessp, uint8_t *buf,
		int size, int media_type)
{
	int n;
	int len;

	while (sessp->run_flag == OS_TRUE) {
		fd_set wfds;
		FD_ZERO(&wfds);
		FD_SET(sessp->sockfd, &wfds);

		struct timeval tv;
		tv.tv_sec = 5;
		tv.tv_usec = 0;

		int maxfd = sessp->sockfd;
		n = select(maxfd + 1, NULL, &wfds, NULL, &tv);
		if (n <= 0) {
			if (n < 0 && errno == EINTR)
				continue;
			perror("select fail");
			return -1;
		}
		if (FD_ISSET(sessp->sockfd, &wfds)) {
			len = sendto(sessp->sockfd, buf, size, 0,(struct sockaddr*)&sessp->address,sizeof(sessp->address));
			if (len <= 0) {
				if (len < 0 && (errno == EINTR || errno == EAGAIN))
					continue;
				perror("send fail");
				return -1;
			}
			if (len == size)
				break;
		}
	}
	return 0;
}
static int h264_nal_send(rtp_session_t *s, const uint8_t *buf, int size, int last)
{
	rtp_pkt_t pkt1, *pkt = &pkt1;
	pkt->rtp_hdr = s->rtp_hdr;

    if (size <= s->max_payload_size) {
		pkt->rtp_hdr.m = last;
		memcpy(pkt->data, buf, size);
		if (udp_send_rtp(s, (uint8_t *)pkt, sizeof(rtp_hdr_t) + size, RTP_MEDIA_TYPE_VIDEO) < 0)
			return -1;
    } else {
        uint8_t type = buf[0] & 0x1F;
        uint8_t nri = buf[0] & 0x60;

        pkt->data[0] = 28;        /* FU Indicator; Type = 28 ---> FU-A */
        pkt->data[0] |= nri;

        pkt->data[1] = type;
        pkt->data[1] |= 1 << 7;

        buf += 1;
        size -= 1;

        while (size + 2 > s->max_payload_size) {
			pkt->rtp_hdr.m = 0;
            memcpy(&pkt->data[2], buf, s->max_payload_size - 2);

			int buf_size = sizeof(rtp_hdr_t) + s->max_payload_size;
			if (udp_send_rtp(s, (uint8_t *)pkt, buf_size, RTP_MEDIA_TYPE_VIDEO) < 0)
				return -1;
            buf += s->max_payload_size - 2;
            size -= s->max_payload_size - 2;
            pkt->data[1] &= ~(1 << 7);
			increase_seq_number(&pkt->rtp_hdr.seq);
        }
        pkt->data[1] |= 1 << 6;
		pkt->rtp_hdr.m = last;
        memcpy(&pkt->data[2], buf, size);
		int buf_size = sizeof(rtp_hdr_t) + size + 2;
		if (udp_send_rtp(s, (uint8_t *)pkt, buf_size, RTP_MEDIA_TYPE_VIDEO) < 0)
			return -1;
    }
	/* update sequence number */
	increase_seq_number(&pkt->rtp_hdr.seq);
	s->rtp_hdr.seq = pkt->rtp_hdr.seq;
	return 0;
}

static int rtpSendVideoFrame(rtp_session_t *sessin_p)
{

//	os_dbg("rtpSendVideoFrame ...... frame_type:%d len :%d",
//		FRAME_TYPE(sessin_p->frame->frame_head.frame_type),
//		sessin_p->frame->frame_head.frame_size);
		
    const uint8_t *p;
    const uint8_t *end;
	const uint8_t *r, *r1;
    av_frame_head_t *frm_hdr;

	frm_hdr = &sessin_p->frame->frame_head;
	p = sessin_p->frame->frame_data;
	end = p+frm_hdr->frame_size;
	sessin_p->pts = frm_hdr->pts;
	/* update rtp timestamp */
    rtp_hdr_t *rtp_hdrp = &sessin_p->rtp_hdr;
	rtp_hdrp->ts = htonl((uint32_t)sessin_p->pts);

	r = comm_h264_find_startcode(p, end);
	while(r < end)
	{
		while (!*(r++));
		r1 = comm_h264_find_startcode(r, end);
		if (h264_nal_send(sessin_p, r, r1 - r, r1 == end) < 0)
			return -1;
		r = r1;		
	}
	return 0;
}
static void *rtpProcessThread(void *pArg)
{
	rtp_session_t *session_p = (rtp_session_t *)pArg;
	char source[IP_SIZE];
	struct in_addr inaddr;
	int sock_fd = -1;
	int ch = session_p->chann_no;
	int ch_type= session_p->chann_type;	
	int *lock_pos = NULL;
	int send_IFrameflag = 0;
	net_frame_t *p_frame = NULL;

	if (comm_getIpAddr("eth0",source,IP_SIZE) < 0)
	{
		os_dbg("Get Host IP Addr Fail\n");
		return(NULL);
	}
	if ((sock_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		os_dbg("Opening mutlsocket error\n");
		return(NULL);
	}
	inaddr.s_addr = inet_addr(source);
	if (setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_IF, &inaddr, sizeof(struct in_addr)) < 0)
	{
		os_dbg("fail when setsockopt\n");
		os_close(sock_fd);
		return(NULL);
	}
	
	memset(&session_p->address, 0, sizeof(session_p->address));
	session_p->address.sin_family = AF_INET;
	session_p->address.sin_addr.s_addr = inet_addr(session_p->addr);
	session_p->address.sin_port = htons(session_p->port);	
	session_p->sockfd = sock_fd;
	session_p->run_flag = OS_TRUE;

	memset(&session_p->read_pos,0,sizeof(session_p->read_pos));	
	session_p->read_pos.read_begin = net_stream_getNetStreamWritePos(ch,ch_type);
	session_p->read_pos.read_end = net_stream_getNetStreamWritePos(ch,ch_type);
	session_p->read_pos.lock_pos = session_p->read_pos.read_begin;
	lock_pos = &session_p->read_pos.lock_pos;
    send_IFrameflag = 1;
	memset(&session_p->rtp_hdr,0,sizeof(rtp_hdr_t));
	
	os_dbg("start multi rtp chann_no:%d_%d %s:%d ",ch,ch_type,session_p->addr,session_p->port);
	init_RtpHdr(&session_p->rtp_hdr,RTP_MEDIA_TYPE_VIDEO);
	while(session_p->run_flag == OS_TRUE)
	{	
		net_stream_lockMutex(ch,ch_type,*lock_pos);
		p_frame = net_stream_getNetStreamFromPool(ch,ch_type,&(session_p->read_pos));
		if(p_frame)
		{
			if (send_IFrameflag == 1)
			{
				if (FRAME_TYPE(p_frame->frame_head.frame_type) != CODEC_I_FRAME_TYPE)
				{
					net_stream_unlockMutex(ch,ch_type, *lock_pos);
					continue;
				}
				send_IFrameflag = 0;
			}
			net_stream_unlockMutex(ch,ch_type, *lock_pos);
			//send multi rtp data
			session_p->frame = p_frame;
			if(FRAME_TYPE(p_frame->frame_head.frame_type) == CODEC_I_FRAME_TYPE ||
				FRAME_TYPE(p_frame->frame_head.frame_type) == CODEC_P_FRAME_TYPE)
			{
				rtpSendVideoFrame(session_p);
			}
		}
		else
		{
			net_stream_unlockMutex(ch,ch_type,*lock_pos);
			usleep(20000);
		}
		
	}
	os_close(session_p->sockfd);
	session_p->sockfd = -1;
	os_dbg("leave multi rtp process ......");
	return NULL;
}
int comm_start_multiRtp(const char *ip,int port,int ch,int ch_type)
{
	int iRet = OS_FALSE;
	rtp_session_t *session_p = NULL;
	if(ch< MAX_CAM_NUM && ch_type < MAX_CAM_STREAM_NUM)
	{
		session_p = &g_rtp_session[ch][ch_type];
		os_mutex_lock(&session_p->mutex);
		if(session_p->run_flag == OS_FALSE)
		{
			session_p->port = port;
			memset(session_p->addr,0,24);
			strncpy(session_p->addr,ip,24);
			os_dbg("start multi rtp %s:%d",ip,port);
			pthread_create(&session_p->tid,NULL,rtpProcessThread,session_p);
			iRet = OS_TRUE;
		}
		else
		{
			os_dbg(" multi rtp %s:%d si already running!!!",session_p->addr,session_p->port);
		}
		os_dbg("tid == %ld",session_p->tid);
		os_mutex_unlock(&session_p->mutex);
	}
	return iRet;
}

int comm_stop_multiRtp(int ch,int ch_type)
{
	int iRet = OS_FALSE;
	rtp_session_t *session_p = NULL;
	if(ch< MAX_CAM_NUM && ch_type < MAX_CAM_STREAM_NUM)
	{
	
		session_p = &g_rtp_session[ch][ch_type];
		os_dbg("stop multi rtp chann_no:%d_%d :%s:%d ",ch,ch_type,session_p->addr,session_p->port);
		os_mutex_lock(&session_p->mutex);
		if(session_p->run_flag == OS_TRUE)
		{
			session_p->run_flag = OS_FALSE;
			pthread_join(session_p->tid,NULL);
		}
		os_mutex_unlock(&session_p->mutex);
		os_dbg("tid == %ld",session_p->tid);
		os_dbg("stop multi rtp :%s:%d ",session_p->addr,session_p->port);
	}
	return OS_TRUE;
}

