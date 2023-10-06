#ifndef _ONVIF_INL_H
#define _ONVIF_INL_H
#include "os_ini.h"
#include "uuid.h"
#define BASE_ONVIF_MODULE		0xE000
#define BASE_KEEPALIVE_MODULE	0xE100
#define ONVIF_MAX_TASK			8
#define ONVIF_MAX_EVENT		32
#define ONVIF_MAX_MQ			512
#define MAX_SCOPES_NUM			100
#define MAX_HTTP_CONN			16
#define MAX_SCOPES_BUFF_LEN	10*1024
#define MAX_HTTP_PKT_LEN		48*1024

#define ONVIF_DEBUG_XML

#ifdef ONVIF_DEBUG_XML

static OS_INLINT_t void xml_printf(const char *DbgFileName,
	int DbgLineNo,const char *FunctionName,const char *FmtStr,...)
{
	va_list ArgList;
	FILE *fp = stdout;
	fprintf(fp, "(%s::%s), line %d", DbgFileName, FunctionName, DbgLineNo);
	if (FmtStr) {
		fprintf(fp, ": \n");
		va_start(ArgList, FmtStr);
		vfprintf(fp, FmtStr, ArgList);
		fflush(fp);
		va_end(ArgList);
	}
	fprintf(fp, "\n");
}
#ifdef __GNUC__
#define debug_xml(args...)              \
do{     \
	xml_printf(__FILE__,__LINE__,__FUNCTION__,##args);   \
}while(0)
#else
#define debug_xml(args,...)              \
do{     \
	xml_printf(__FILE__,__LINE__,__FUNCTION__,##args);   \
}while(0)
#endif
#else
#define debug_xml(args...) {}
#endif


///////////////////////added by nosword ///////////////////////


typedef struct ttvideosourceconfigurationEx
{
	ttvideosourceconfiguration_t videosourceconfiguration;
	struct ttvideosourceconfigurationEx *next;
}ttvideosourceconfigurationEx_t;

typedef struct ttvideosourceconfigurationMgr
{
	int count;
	ttvideosourceconfigurationEx_t *next;
}ttvideosourceconfigurationMgr_t;

typedef struct videoencoderconfigurationEx
{
	videoencoderconfiguration_t videoencoderconfiguration;
	struct videoencoderconfigurationEx *next;
}videoencoderconfigurationEx_t;

typedef struct videoencoderconfigurationMgr
{
	int count;
	videoencoderconfigurationEx_t *next;
}videoencoderconfigurationMgr_t;

typedef struct audiosourceconfigurationEx
{
	audiosourceconfiguration_t audiosourceconfiguration;
	struct audiosourceconfigurationEx *next;
}audiosourceconfigurationEx_t;

typedef struct audiosourceconfigurationMgr
{
	int count;
	audiosourceconfigurationEx_t *next;
}audiosourceconfigurationMgr_t;

typedef struct audioencoderconfigurationEx
{
	audioencoderconfiguration_t  audioencoderconfiguration;
	struct audioencoderconfigurationEx *next;
}audioencoderconfigurationEx_t;

typedef struct audioencoderconfigurationMgr
{
	int count;
	audioencoderconfigurationEx_t *next;
}audioencoderconfigurationMgr_t;

typedef struct ttptznodeconfigrationEx
{
	ttptznodeconfigration_t ptznodeconfigration;
	struct ttptznodeconfigrationEx *next;
}ttptznodeconfigrationEx_t;

typedef struct ttptznodeconfigrationMgr
{
	int count;
	ttptznodeconfigrationEx_t *next;
}ttptznodeconfigrationMgr_t;

typedef struct ttmediaprofileEx
{
    char profiletoken[64];   //属性结点名
	char profilename[64];    //文件名
	int fixed_enable;   //标示此节点是否是 fixed 
	ttvideosourceconfigurationEx_t *pVideosourceconfiguration; //视频配置参数
	videoencoderconfigurationEx_t *pVideoencoderconfiguration; //视频解编码参数
	audiosourceconfigurationEx_t *pAudioSourceconfiguration;   //音频配置参数
	audioencoderconfigurationEx_t *pAudioencoderconfiguration; //音频编解码参数
	ttptznodeconfigrationEx_t *pPtznodeconfigration;       //PTZ配置参数
	struct ttmediaprofileEx *next;
}ttmediaprofileEx_t;

typedef struct ttmediaprofileMgr
{
	int count;
    ttmediaprofileEx_t *next;
}ttmediaprofileMgr_t;

typedef struct mediaMgr
{
	OS_MUTEX_t mutex;
	ttmediaprofileMgr_t mediaprofileMgr;
	ttvideosourceconfigurationMgr_t videoSourcecCfgMgr;
	videoencoderconfigurationMgr_t videoEncoderCfgMgr;
	audiosourceconfigurationMgr_t audioSourceCfgMgr;
	audioencoderconfigurationMgr_t audioEncoderCfgMgr;
	ttptznodeconfigrationMgr_t ptzNodeCfgMgr;
}mediaMgr_t;

typedef struct __ptzpreset_mgr_t
{
	char token[8];
	char name[64];
	int  useFlg;
}ptzpreset_mgr_t;
///////////////////////////////////////////////////////////////////////////////



//////////////////////////////////使用的结构体///////////////////////////////////
typedef struct _cmdmsg
{
    char cmd[64];    //onvif命令
    char urn_uuid[64];  //包含messageID
	char lastcmd[64]; //上一次的命令
	char createprofile[32];
	char scopesmatch[64];
	char eventtime[8]; //记录event时间中传入的一个时间
	char checkdatetime; //判断系统时间的标志位
	char checkstream;
	char checkfaultmsg;  //判断错误消息的标志位
	char eventflag;  //event中表示TopicExpression，1.MessageContent，2的错误
	char recordingflag; //录像信息标志位
	char url[256];	//用于区别 media media2 等不同url
}cmdmsg_t;

typedef struct onvif_namespace_
{
	const char *ns;
	const char *val;
}onvif_namespace_t;

#define NS_WSA_INDEX		0
#define NS_TT_INDEX		1
#define NS_TRT_INDEX		2
#define NS_TEV_INDEX		3
#define NS_TER_INDEX		4
#define	NS_TDS_INDEX		5
#define	NS_DN_INDEX		6
#define	NS_TNS1_INDEX		7
#define	NS_WSDL_INDEX		8
#define	NS_WSOAP12_INDEX	9
#define NS_HTTP_INDEX		10
#define NS_SOAPENC_INDEX	11
#define NS_SOAPENV_INDEX	12
#define	NS_XSD_INDEX		13
#define	NS_XSI_INDEX		14
#define	NS_D_INDEX			15
#define	NS_WSADIS_INDEX	16
#define	NS_WSTOP_INDEX		17
#define NS_WSNT_INDEX		18
#define NS_XOP_INDEX		19
#define NS_RPC_INDEX		20
#define NS_TIMG_INDEX		21
#define NS_TPTZ_INDEX		22
#define NS_TAN_INDEX		23
#define	NS_TST_INDEX		24
#define NS_TTMP_INDEX		25
#define NS_TRC_INDEX		26
#define NS_WSRF_BF_INDEX	27
#define	NS_WSNTW_INDEX		28
#define	NS_WSRF_RW_INDEX	29
#define	NS_WSAW_INDEX		30
#define	NS_WSRF_R_INDEX	31
#define	NS_TNSN_INDEX		32
#define NS_WSSE_INDEX		33
#define NS_WSU_INDEX		34
#define NS_NULL_INDEX		35

typedef IXML_Document *(*ONVIF_PROCESS_FUNC_t)(IXML_Document *doc,cmdmsg_t *pCmdMsg);

typedef struct OnvifProcessImpList
{
	const char *cmd;
	ONVIF_PROCESS_FUNC_t pProcessFunc;
}OnvifProcessImpList_t;

/////////////////////////////////内部的函数///////////////////////////////////////
void on_hand_multi_process(int fd, data_buff_t *data_buff_p, struct sockaddr_in addr);
onvif_namespace_t * get_namespace_by_index(int index);
onvif_namespace_t *get_onvif_namespace();
const char *get_namespace_value_by_index(int index);
const char *onvif_get_host_ip();
const char *onvif_get_device_uuid();
int creat_document_envelope_xml(IXML_Element *element);
IXML_Document *onvif_discovery_probe(IXML_Document * doc,cmdmsg_t *cmdmsg);
void init_scope_config();
void init_capabilities_config();
void send_http_data(http_context_t *http_context);
int on_hand_http_process(http_context_t *http_context);
THR_TASK_t *get_onvif_task_info();
int create_http_text(data_buff_t *data_buff_p,char *docbuf,cmdmsg_t *pCmdMsg);
void debug_print_xml_node(IXML_Node *node);
char *get_xmlNode_attrValue(IXML_Node *node,const char *attr);
char *get_xmlNode_value(IXML_Node *node);
void get_uuid(char *out);
int get_message_num();
ttscope_t *get_scopehead();
const char *onvif_get_discovery_mode();
void onvif_set_discovery_mode(const char *mode);
IXML_Document *create_common_response_xml(cmdmsg_t *cmdmsg);
IXML_Document *create_common_getcapabilities_xml(cmdmsg_t *cmdmsg);
IXML_Document *create_soap_fault_xml(cmdmsg_t *cmdmsg);
ttcapabilities_t *get_capabilities();
const char *onvif_get_hostname();
ttnetworkinterfaces_t *onvif_get_networkInterface();
ttmediaprofilemax_t *onvif_get_mediaprofile();
ttmulticast_t *onvif_get_multicast();
IXML_Element *build_profile(IXML_Document *doc,IXML_Element *parant,ttmediaprofileEx_t*pmediaprofile,int flag);
void init_mediaprofile_config();
ttscope_t *get_scoperemovedhead();
int add_scoperemovedhead(const char *scopeItem);
int del_scoperemovedhead(const char *scopeItem);
void clear_scoperemovedhead();
int isExist_scopehead(const char *scopeItem);
IXML_Document *creat_setscopes_fault_xml();
const char * get_scopes();
int check_hostname(char *name);
int check_ipv4_dns(char *dns);
IXML_NodeList *get_xml_node_list(IXML_Document *InDoc,const char *ns,const char *name,int *result);
IXML_Node *get_xml_node(IXML_NodeList *list,int index);
void onvif_set_hostname(const char *hostname);
ttntpinfo_t *onvif_get_NTPInfo();
int  check_datetime(struct tm *timenow);
char *get_single_xml_node_val(IXML_Document *InDoc,const char *ns,const char *name);
char *get_single_xml_node_attr(IXML_Document *InDoc,const char *ns,const char *name,char *attr);
ttcreatmediaprofile_t *onvif_get_createdmediaprofile();
void media_destroy_media_cfgMgr();
void media_init_media_cfgMgr();
void media_init_osdMgr();
void media_init_ptzCfg();
void init_ptz_config(ttptznodeconfigration_t * ptznodeconfigration);
osdconfigurationMgr_t *media_get_osdconfigurationMgr();
ttptznodeconfigrationEx_t *media_getptznodeconfigration();

osdconfiguration_t *media_get_osdconfiguration(const char *token);
ttmediaprofileEx_t *media_add_mediaprofileMgr(const char *pProfileToken,char *pProfileName,int fixed_flag);
int media_del_mediaprofileMgr(const char *pProfileToken);
ttmediaprofileEx_t *media_get_mediaprofileMgr_byToken(const char *pToken);
ttmediaprofileEx_t *media_get_mediaprofileMgr_byName(const char *pName);
int media_add_VideoSourcecCfgMgr(ttmediaprofileEx_t *pCreatmediaprofile,const char *pToken);
int media_del_VideoSourcecCfgMgr(ttmediaprofileEx_t *pCreatmediaprofile);
ttvideosourceconfigurationEx_t *media_get_VideoSourcecCfgMgr(const char *pToken);
int media_add_VideoEncoderCfgMgr(ttmediaprofileEx_t *pCreatmediaprofile,const char *pToken);
int media_del_VideoEncoderCfgMgr(ttmediaprofileEx_t *pCreatmediaprofile);
videoencoderconfigurationEx_t *media_get_VideoEncoderCfgMgr(const char *pToken);
int media_add_AudioSourceCfgMgr(ttmediaprofileEx_t *pCreatmediaprofile,const char *pToken);
int media_del_AudioSourceCfgMgr(ttmediaprofileEx_t *pCreatmediaprofile);
audiosourceconfigurationEx_t * media_get_AudioSourceCfgMgr(const char *pToken);
int media_add_AudioEncoderCfgMgr(ttmediaprofileEx_t *pCreatmediaprofile,const char *pToken);
int media_del_AudioEncoderCfgMgr(ttmediaprofileEx_t *pCreatmediaprofile);
audioencoderconfigurationEx_t * media_get_AudioEncoderCfgMgr(const char *pToken);
int media_add_PtzNodeCfgMgr(ttmediaprofileEx_t *pCreatmediaprofile,const char *pToken);
int media_del_PtzNodeCfgMgr(ttmediaprofileEx_t *pCreatmediaprofile);
ttptznodeconfigrationEx_t *media_get_PtzNodeCfgMgr(const char *pToken);
ttmediaprofileMgr_t *media_get_mediaprofileMgrList();
ttmediaprofileEx_t *media_get_mainMediaProfile();
ttmediaprofileEx_t *media_get_subMediaProfile();
ttnetworkprotocols_t *onvif_get_netprotocol();
int creat_document_event_xml(IXML_Element *element);
int onvif_get_multi_port();
const char *onvif_get_multi_ip_addr();
OS_SOCKET_t onvif_get_multi_sock();
int send_multi_bye_msg();
int send_multi_hello_msg();

IXML_Document *creat_getdatetime_document_xml();
void get_datetime(datetime_t *datetime);
void init_datetime_config();
IXML_Document *creat_getsubscribe_document_xml(cmdmsg_t *cmdmsg);
void init_relay_out();
IXML_Document *creat_getrelayoutputs_document_xml();

#endif
