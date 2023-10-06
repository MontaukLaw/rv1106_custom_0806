#include "common.h"

#include "comm_type.h"
#include "comm_codec.h"
#include <rga/rga.h>
#include <rga/im2d.h>

#include "codec2_process.h"
#include "osd_if.h"
#include "RgaProc.h"

#include "snapshot.h"
#include <rk_mpi_mb.h>
#include "rkai_demo.h"

#include "yolo_detect.h"
#include "tde.h"
#include "curl_get.h"
#include "math.h"
#include "post_process.h"
#include "amix_control.h"

// 桌子的分类idx(目前是椅子)
#define TABLE_CLASS_IDX 2
// 人的分类idx (目前用手机替代)
#define PERSON_CLS_IDX 1
// 开球的分类idx (目前是bottle)
#define GAME_START_CLS_IDX 0
// 0开球，1人，2球桌
#define PERSON_MOVING_DISTANCE 50

// static CROP_RECT crop[] = {
// 	{100, 100, 640, 480},
// 	{100, 100, 640, 480},
// 	{100, 100, 640, 480},
// 	{100, 100, 640, 480}};

extern "C" int
yuv_data_coming(int stream_index, void *mb);

// #define YUV_TO_RGB
// #define RGB_SAVE_TO_FILE
// #define JPEG_OSD_TEST
#define TEST_AI_RESULT
// #define RK_IVA_ENABLE
// #define RKNN_LETTERL_BOX
#define RK_RKNN_YOLO_ENABLE
#define MAX_DISPLAY_BOX_NUM 7
#define PERSON_ROI_OSD_IDX 7

#define ROI_TABLE_RANGE_LEFT 300
#define ROI_TABLE_RANGE_RIGHT 300
#define ROI_TABLE_RANGE_TOP 400
#define ROI_TABLE_RANGE_BOTTOM 50

#define LINE_THICK 2
// 开球框出现的延迟帧数量
#define GAME_START_DELAY_FRAME 2
#define GAME_START_DELAY_SEC (5 * 1) // 为了测试方便, 这里改成5秒
// #define GAME_START_DELAY_SEC (5 * 60)

#define NO_PERSON_IN_ROI_SEC (2 * 60)

typedef struct
{
	int centerx;
	int centery;

} object_center_t;

extern "C" void notify_ROI_changed(int channel_no)
{
}

static avcodec_osd_rect_t rknn_boxes[MAX_DISPLAY_BOX_NUM];
nn_object_array_t ai_result = {0};
static void *ss_hd; // 截图句柄
static avcodec_osd_rect_t roi_box;

// 是否游戏开始
int g_game_status = NO_GAMING;

// 0821 用于判断是否检测到开球的变量
uint8_t if_get_game_start_box = 0;
uint8_t if_get_person_in_roi = 0;

RgaProc *rga_hd[2];

static unsigned char *rgb_buff[MAX_STREAM_NUM];
static CROP_RECT crop[] = {
	{0, 0, 1920, 1080},
	{100, 100, 640, 640},
	{100, 100, 640, 640},
	{100, 100, 640, 640}};

object_center_t moving_person_buf;

// 游戏开始时间
static uint64_t game_start_timer_sec = 0;
static uint64_t last_person_in_roi_sec = 0;

void get_box_from_ai_result(avcodec_osd_rect_t *rknn_box, int idx)
{
	nn_object_t *detect_object;
	int left, top, right, bottom;

	detect_object = &ai_result.object[idx];
	left = detect_object->box.left * RKNN_VI_WIDTH / MID_TERM_WIDTH;
	top = (detect_object->box.top - DETECT_X_START) * RKNN_VI_HEIGHT / MID_TERM_HEIGHT;

	right = detect_object->box.right * RKNN_VI_WIDTH / MID_TERM_WIDTH;
	bottom = (detect_object->box.bottom - DETECT_X_START) * RKNN_VI_HEIGHT / MID_TERM_HEIGHT;

	if (right > RKNN_VI_WIDTH)
	{
		right = RKNN_VI_WIDTH;
	}
	if (bottom > RKNN_VI_HEIGHT)
	{
		bottom = RKNN_VI_HEIGHT;
	}

	if (detect_object->cls_idx == TABLE_CLASS_IDX)
	{
		// write_log("table left = %d, top = %d, right = %d, bottom = %d\n", left, top, right, bottom);
	}

	if (top < 0 || left < 0 || bottom < 0 || right < 0)
	{
		return;
	}

	rknn_box->index = idx;
	rknn_box->enable = 1;
	rknn_box->x = left;
	rknn_box->y = top;
	rknn_box->w = right - left - LINE_THICK;
	rknn_box->h = bottom - top - LINE_THICK;
	rknn_box->thick = LINE_THICK;

	if (detect_object->cls_idx == TABLE_CLASS_IDX)
	{
		rknn_box->color = 0x0000ff;
	}
	else
	{
		rknn_box->color = 0x00ff00;
	}
}

static void clean_all_boxes(void)
{
	int i;
	for (i = 0; i < MAX_DISPLAY_BOX_NUM + 1; i++)
	{
		rknn_boxes[i].enable = 0;
		rknn_boxes[i].index = i;
		rknn_boxes[i].x = 0;
		rknn_boxes[i].y = 0;
		rknn_boxes[i].w = 100;
		rknn_boxes[i].h = 100;
		rknn_boxes[i].thick = 4;
		rknn_boxes[i].color = 0xff0000;

		app_set_stream_sw_osd_rect(CAM0_STREAM_MAIN, &rknn_boxes[i]);
	}
}

static void clean_useless_boxes(void)
{
	int i;
	for (i = ai_result.count; i < MAX_DISPLAY_BOX_NUM; i++)
	{
		rknn_boxes[i].enable = 0;
		rknn_boxes[i].index = i;
		rknn_boxes[i].x = 0;
		rknn_boxes[i].y = 0;
		rknn_boxes[i].w = 100;
		rknn_boxes[i].h = 100;
		rknn_boxes[i].thick = 4;
		rknn_boxes[i].color = 0xff0000;

		app_set_stream_sw_osd_rect(CAM0_STREAM_MAIN, &rknn_boxes[i]);
	}
}

int get_first_specific_box(int classIdx)
{
	int i;
	for (i = 0; i < ai_result.count; i++)
	{
		if (ai_result.object[i].cls_idx == classIdx)
		{
			return i;
		}
	}
	return OS_FAILD;
}

static uint8_t get_roi_box_base_on_table(void)
{
	// 第一步, 获取桌子的idx
	int table_idx = get_first_specific_box(TABLE_CLASS_IDX);
	if (table_idx == OS_FAILD)
	{
		return OS_FAILD;
	}

	int roiX, roiY, roiW, roiH;

	// 第二步, 获取桌子的坐标
	avcodec_osd_rect_t *table_box = &rknn_boxes[table_idx];

	roiX = table_box->x - ROI_TABLE_RANGE_LEFT;
	if (roiX < 0)
	{
		roiX = 0;
	}

	roiY = table_box->y - ROI_TABLE_RANGE_TOP;
	if (roiY < 0)
	{
		roiY = 0;
	}

	roiW = table_box->w + ROI_TABLE_RANGE_LEFT + ROI_TABLE_RANGE_RIGHT;
	if (roiW + roiX > RKNN_VI_WIDTH)
	{
		roiW = RKNN_VI_WIDTH - LINE_THICK - roiX;
	}

	roiH = table_box->h + ROI_TABLE_RANGE_TOP + ROI_TABLE_RANGE_BOTTOM;
	if (roiH + roiY > RKNN_VI_HEIGHT)
	{
		roiH = RKNN_VI_HEIGHT - LINE_THICK - roiY;
	}

	memset(&roi_box, 0, sizeof(avcodec_osd_rect_t));

	roi_box.index = PERSON_ROI_OSD_IDX;
	roi_box.enable = 1;
	roi_box.x = roiX;
	roi_box.y = roiY;
	roi_box.w = roiW;
	roi_box.h = roiH;
	roi_box.thick = LINE_THICK;
	roi_box.color = 0xff0000;

	// write_log("roiX = %d, roiY = %d, roiW = %d, roiH = %d\n", roiX, roiY, roiW, roiH);
	return OS_SUCCESS;
}

void draw_roi_range(void)
{
	app_set_stream_sw_osd_rect(CAM0_STREAM_MAIN, &roi_box);
}

// 开球只考虑start框出现，不考虑人
void check_if_game_start(void)
{
	static int delay_counter = 0;
	if (g_game_status == GAMING)
	{
		return;
	}

	// 如果有出现开球框, 就加延迟判断
	if (get_first_specific_box(GAME_START_CLS_IDX) != OS_FAILD)
	{
		// write_log(">>>>>>>>>>>>>> game start delay counter = %d\n", delay_counter);
		write_log("get game start box!!!!!!!! \n");
		if_get_game_start_box = 1;
		// delay_counter++;
	}
	else
	{
		if_get_game_start_box = 0;
		delay_counter = 0;
	}

	return;

	// 如果持续N帧都有开球框, 就认为开球了
	if (delay_counter >= GAME_START_DELAY_FRAME)
	{
		// g_game_status = GAMING;
		// write_log("game start !!!!!!!! \n");
		// 记录开球时间
		// game_start_timer_sec = get_current_time_sec();
		// 加上这个, 避免一种奇怪的情况, 测试中又出现但是实际不会出现的情况, 就是开球之后没人, 导致last_person_in_roi_sec不会刷新. 而直接停止游戏.
		// last_person_in_roi_sec = get_current_time_sec();
		// http_send_game_status_event();

		// delay_counter = 0;
	}
}

// 计算两个中心点之间的蒙特卡洛距离
int get_distance_between_two_point(object_center_t p1, object_center_t p2)
{
	return sqrt(pow(p1.centerx - p2.centerx, 2) + pow(p1.centery - p2.centery, 2));
}

void check_if_person_in_roi(void)
{
	// if_get_person_in_roi
	if_get_person_in_roi = 0;

	write_log("check if person in roi\n");

	int i = 0;

	object_center_t person_center = {0, 0};
	// bool if_person_in_roi = false;

	for (i = 0; i < ai_result.count; i++)
	{
		if (ai_result.object[i].cls_idx == PERSON_CLS_IDX)
		{
			// 有人, 计算人框的中心点, 看是否在roi框内
			int person_center_x = rknn_boxes[i].x + rknn_boxes[i].w / 2;
			int person_center_y = rknn_boxes[i].y + rknn_boxes[i].h / 2;

			// write_log("person_center_x = %d, person_center_y = %d\n", person_center_x, person_center_y);
			// write_log("roi_box.x = %d, roi_box.y = %d, roi_box.w = %d, roi_box.h = %d\n", roi_box.x, roi_box.y, roi_box.w, roi_box.h);

			if (person_center_x > roi_box.x && person_center_x < roi_box.x + roi_box.w &&
				person_center_y > roi_box.y && person_center_y < roi_box.y + roi_box.h)
			{
				person_center = {person_center_x, person_center_y};
				if_get_person_in_roi = 1;
				// if_person_in_roi = true;
				write_log("!!!!!!!!!!!!!!!!!!!!! person in roi\n");
				break;
			}
		}
	}
}

// 当star的框出现之后。然后。过五分钟。
// 检测球桌。
// 球桌增大一圈之后是否有人出现，以及他是否动，如果没人，
// 以及有人但超过两分钟不动就算结束，另外从start框出现后超过十分钟也算结束
void check_if_game_end(void)
{
	int i = 0;
	static int no_moving_person_in_roi_sec = 0;

	// 游戏如果没开始, 就不用检测结束了
	if (g_game_status == NO_GAMING)
	{
		return;
	}

	long int nowSec = get_current_time_sec();
	if (nowSec - game_start_timer_sec < GAME_START_DELAY_SEC)
	{
		write_log("game start in 5 min, no need test game stop\n");
		return;
	}

	// start框出现后超过十分钟也算结束
	if (nowSec - game_start_timer_sec > 10 * 60)
	{
		write_log("game start more than 10 min, game stop\n");
		g_game_status = NO_GAMING;
		http_send_game_status_event();
		return;
	}

	// 检查roi框内是否有人
	// get_roi_box_base_on_table();
	// memset(&moving_person_buf, 0, sizeof(person_center_t));

	object_center_t person_center = {0, 0};
	bool if_person_in_roi = false;

	for (i = 0; i < ai_result.count; i++)
	{
		if (ai_result.object[i].cls_idx == PERSON_CLS_IDX)
		{
			// 有人, 计算人框的中心点, 看是否在roi框内
			int person_center_x = rknn_boxes[i].x + rknn_boxes[i].w / 2;
			int person_center_y = rknn_boxes[i].y + rknn_boxes[i].h / 2;

			write_log("person_center_x = %d, person_center_y = %d\n", person_center_x, person_center_y);
			write_log("roi_box.x = %d, roi_box.y = %d, roi_box.w = %d, roi_box.h = %d\n", roi_box.x, roi_box.y, roi_box.w, roi_box.h);

			if (person_center_x > roi_box.x && person_center_x < roi_box.x + roi_box.w &&
				person_center_y > roi_box.y && person_center_y < roi_box.y + roi_box.h)
			{
				person_center = {person_center_x, person_center_y};
				if_person_in_roi = true;
				write_log("!!!!!!!!!!!!!!!!!!!!! person in roi\n");
				break;
			}
		}
	}

	// 有人, 但是在动
	if (if_person_in_roi && get_distance_between_two_point(person_center, moving_person_buf) > PERSON_MOVING_DISTANCE)
	{
		//
		write_log("person moving\n");
		no_moving_person_in_roi_sec == get_current_time_sec();

		moving_person_buf.centerx = person_center.centerx;
		moving_person_buf.centery = person_center.centery;

		last_person_in_roi_sec = get_current_time_sec();

		// memcpy(moving_person_buf, person_center, sizeof(object_center_t));
	}
	else if (if_person_in_roi == false)
	{
		write_log("no person in roi\n");
		memset(&moving_person_buf, 0, sizeof(object_center_t));
	}
	else
	{
		write_log("at least 1 person, not moving\n");
	}

	if (nowSec - last_person_in_roi_sec > NO_PERSON_IN_ROI_SEC)
	{
		write_log("no person in roi more than 2 min, game stop\n");
		g_game_status = NO_GAMING;
		http_send_game_status_event();
		return;
	}
}

extern "C" int comm_ai_process(int stream_index, void *mb, void *puser)
{
	static long detectCount = 0;
	static long getDetectStatusCounter = 0;
	int i = 0;
	// int line_pixel = 4;
	int k;
	// int left, top, right, bottom;

	if (stream_index != CAM0_STREAM_MAIN)
	{
		return OS_SUCCESS;
	}

	
	// 第一步, 通过api接口, 获取是否需要检测
	getDetectStatusCounter++;
	detectCount++;
	RgaProc *hd;
	unsigned char *buff = NULL;

	// 大概4,5秒左右
	if (getDetectStatusCounter == DETECT_STATUS_INTERVAL_FRAME)
	{

		// http_get_detect_status();
		// 获取是否需要画roi框的接口
		write_log("access http_get_if_draw_roi\n");
		http_get_if_draw_roi();
		getDetectStatusCounter = 0;
	}

// write_log("start get detect command\n");
#if 0 
	if (get_detect_status_val() == 0)
	{
		// write_log("no need detect \n");
		write_log("detect not start but for test, still continue\n");
		// return OS_SUCCESS;
	}
#endif

	if (detectCount % DETECT_ITERVAL_FRAME != 1)
	{
		return OS_SUCCESS;
	}

	// if (g_game_status == GAMING)
	// {
	// 	write_log("------------ gaming -----------\n");
	// }
	// else
	// {
	// 	write_log("------ ---- no gaming -----------\n");
	// }
	// write_log("stream_index: %d w = %d, h = %d\n", stream_index, w, h);

	// 第二步,开始检测
	int w, h;
	MB_BLK dstBlk = RK_NULL;
	app_get_resolution(stream_index, &w, &h, NULL);
	// 将yuv转成rgb640x640, 用于检测
	trans_1920x1080_yuv_to_640x640_rgb((VIDEO_FRAME_INFO_S *)mb, &dstBlk);
	// write_log("creating data\n");
	// if (count == 10)
	// {
	// 	// 写成一个文件
	// 	FILE *fp = fopen("/userdata/nfs/640x640.rgb", "wb");
	// 	if (fp)
	// 	{
	// 		fwrite(RK_MPI_MB_Handle2VirAddr(dstBlk), 1, MODLE_WIDTH * MODLE_WIDTH * 3, fp);
	// 		fclose(fp);
	// 	}
	// }

	// detect_result_group_t detect_result_group;
	// memset(&detect_result_group, 0, sizeof(detect_result_group_t));
	// ai_result
	// 检测, 返回ai_result
	rknn_detect((unsigned char *)(RK_MPI_MB_Handle2VirAddr(dstBlk)));
	// rknn_detect((unsigned char *)(RK_MPI_MB_Handle2VirAddr(dstBlk)), &detect_result_group);
	if (dstBlk)
	{
		RK_MPI_SYS_MmzFree(dstBlk);
	}

	// 加入判断是否添加roi框
	if (get_if_draw_roi_val())
	{
		// 第三步. 后处理，画osd
		write_log("ai_result.count = %d\n", ai_result.count);
		// max is 8
		memset(rknn_boxes, 0, sizeof(avcodec_osd_rect_t) * MAX_DISPLAY_BOX_NUM);
		for (i = 0; i < ai_result.count; i++)
		{
			if (i >= MAX_DISPLAY_BOX_NUM)
			{
				break;
			}

			get_box_from_ai_result(&rknn_boxes[i], i);
			app_set_stream_sw_osd_rect(CAM0_STREAM_MAIN, &rknn_boxes[i]);
		}

		// 获取roi框
		int ret = get_roi_box_base_on_table();
		if (ret == OS_SUCCESS)
		{
			// 画出roi框
			draw_roi_range();
		}
		else
		{
			write_log("no table in view");
		}
		// 为了避免画框的osd不能及时清除, 这里手动清除一下.
		clean_useless_boxes();
	}
	else
	{
		// 不检测, 也需要获取roi框
		int ret = get_roi_box_base_on_table();
		if (ret != OS_SUCCESS)
		{
			write_log("no table in view");
		}
		// 清除所有的框
		clean_all_boxes();
	}

	// 第四步, 判断是否开球.
	// 是否开球的依据就是是否出现了开球的框, 超过N帧, 就认为开球了.
	// 0821 仅仅判断是否出现了开球的框, 不判断是否有人.
	check_if_game_start();

	check_if_person_in_roi();

	// 发送检测结果
	send_detect_result(if_get_game_start_box, if_get_person_in_roi);

	// 第五步, 判断是否结束
	// check_if_game_end();

	return OS_SUCCESS;
}

extern "C" int comm_ai_init()
{
	int ret;
	os_dbg("enter");

	//. set para
	app_set_ai_process_para(CAM0_STREAM_MAIN, comm_ai_process, 4, NULL);

	// AI 初始化...
#ifdef RK_IVA_ENABLE
	rmm_yuv_handle_init();
#endif

#ifdef RK_RKNN_YOLO_ENABLE

	init_model();

	init_lables();

	get_ip();

	// set volumn
	control_vol();

#endif
	//
	//	start_AlarmProcess();
	os_dbg(" exit");
	return OS_SUCCESS;
}
