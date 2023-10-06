#ifndef __OSD_CPP_IF_H
#define __OSD_CPP_IF_H

#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <codecvt>
#include <locale>
#include <rga/rga.h>
#include <rga/im2d.h>

#include "osd/osd_common.h"
#include "osd/osd_producer.h"
#include "osd/color_table.h"

typedef enum
{
	FONT_BG_COLOR_0 = 0,
	FONT_BG_COLOR_1,
	FONT_BG_COLOR_2,
	FONT_BG_COLOR_3,
	FONT_BG_COLOR_4,
	FONT_BG_COLOR_TOP = FONT_BG_TABLE_LEN,
} FONT_BG_COLOR_INDEX;

extern unsigned int DEFAULT_FONT_BG_PALETTE_TABLE[FONT_BG_TABLE_LEN];

#define MAX_OSD_REGION 8

enum
{
	OSD_REGION_ID_START = -1,			// useless, start mark
	OSD_REGION_ID_INVADE = 0,			// ID_TEXT3-4 now use to region invade
	OSD_REGION_ID_MASK,					// ID_MASK0  (1-2-3)
	OSD_REGION_ID_IMAGE,				// ID_IMAGE
	OSD_REGION_ID_TIMEDATE,				// timedate
	OSD_REGION_ID_TEXT0,				// ID_TEXT0
	OSD_REGION_ID_TEXT1,				// ID_TEXT1  (2)
	OSD_REGION_ID_CHANNEL,				// ID_CHANNEL
	OSD_REGION_ID_TIP,					// ID_TEXT5-6-7
	OSD_REGION_ID_END = MAX_OSD_REGION, // useless, end mark
};

typedef enum
{
	REGION_TYPE_OVERLAY = 0, // overlay with alpha by mpp
	REGION_TYPE_COVER,		 // cover by mpp
	REGION_TYPE_MOSAIC,		 // mosaic by mpp
	REGION_TYPE_OVERLAY_EX,	 // overlay ex by rga
	REGION_TYPE_COVER_EX,	 // cover ex by rga
} REGION_TYPE;

typedef struct
{
	uint8_t *buffer; // Content: ID of palette
	uint32_t pos_x;
	uint32_t pos_y;
	uint32_t width;
	uint32_t height;
	uint32_t inverse;
	uint32_t region_id; // max = 8.
	uint8_t enable;
	REGION_TYPE region_type;
	uint32_t cover_color; // for REGION_TYPE_COVER
} OsdRegionData;

class Osd_If
{
public:
	Osd_If(int enc_chan_id_p, int w, int h, bool is_jpeg = false);
	~Osd_If();

	// for osd
	int set_font_bg_color(FONT_BG_COLOR_INDEX index = FONT_BG_COLOR_0);
	/*
	 *  note: 1. region_id 的范围 是 0 ~ 7； color 的格式是 ARGB, 最高位是A。
	 *        2. 以下函数 的坐标， 宽高 最好 16 对齐（16倍数）；否则 显示出来的会跟你预想的有一些偏差（最大16像素）
	 *        3. thick = 1 目前颜色有点不对，建议从2开始使用。
	 */
	void SetOsdTip(int region_id, int x, int y, std::string tip, unsigned int color, int font_size = 32, bool have_background = true);
	void SetOsdBmp(int region_id, int x, int y, std::string bmp_path);
	void SetOsdLine(int region_id, int x1, int y1, int x2, int y2, int display_style, int thick = 20, unsigned int color = 0x00ff0000);
	void SetOsdRegionBorder(int region_id, int x_p, int y_p, int w_p, int h_p, int display_style, int thick = 10, unsigned int color = 0x00ff0000);

	// 待完善
	void SetOsdHide(int region_id, bool isHide);

	void Clear_Osd(int region_id);

	static void config_font_data(unsigned char alpha, const char *fontpath = "/oem/usr/share/simsun_en.ttf", unsigned int *bg_palette_table = DEFAULT_FONT_BG_PALETTE_TABLE);

	bool getInitStatus(void);
	unsigned int getMaskFlag(void);

	// 以下函数 在YUV 直接画的，效率可能不高
	/*
	 *  在yuv 上画线。
	 */
	static void SetRegionBorder(void *yuvbuff, int w, int h, int x_p, int y_p, int w_p, int h_p, int thick = 10, unsigned int color = 0x00ff0000);
	static bool SetLine(void *yuvbuff, int w, int h, int x1, int y1, int x2, int y2, int thick = 10, unsigned int color = 0x00ff0000);
	/*
	 * 下面几个函数是将 SetLine 拆成3个函数，方便上层的高效使用。
	 */
	void *GetOsdLineInfo(int x1, int y1, int x2, int y2, int thick, int line_type);
	void SetLine(void *line_info, void *nv12_buff, unsigned int color = 0x00ff0000);
	void ReleaseOsdLineInfo(void *line_info);

	void SetBitmap(void *bitmap, void *nv12_buff, int x, int y, int w, int h, unsigned int color = 0x00ff0000);

	// 在yuv上写字符
	void *GetOsdStrInfo(int x, int y, std::string tip, unsigned int color, int font_size);
	void SetTip(void *str_info, void *nv12_buff);
	void ReleaseOsdStrInfo(void *str_info);

	static void draw_lines_quick(short *array, unsigned char *buff, int y, int u, int v, int vi_width, int vi_height);

private:
	int enc_chan_id;
	int vi_width;
	int vi_height;

	// for osd
	OsdRegionData osd_data[MAX_OSD_REGION];
	OsdRegionData osd_data_save[MAX_OSD_REGION]; // 用于保存当前的osd 数据。
	std::shared_ptr<OSDProducer> osd_producer_;
	float gradient_x_;
	float gradient_y_;
	osd_data_s osds_db_data_[MAX_OSD_REGION];
	bool osd_need_init(OsdRegionData *osd);

	void GetStrLineInfo(void *str_info, short *array, int maxlen);

	unsigned char cur_index_bg; // current font's background color

	static char path_font[256];

	int RK_MPI_VENC_set_osd_region_local(int chan, OsdRegionData *data);

	bool initOK;
	bool is_jpeg;

	unsigned int mask_feature;

	// draw line
	static void nv12_line(unsigned char *pic, int x1_p, int y1_p, int x2_p, int y2_p, int thick, unsigned int color, int vi_width, int vi_height);
	static void get_outline_data(short *data1, short *data2, short *data3, short *data4, short *result_array, int vi_height);
	// void get_point_to_array(int x, int y, void * array);
	void draw_lines(short *array, unsigned char *buff, int y, int u, int v);
	void draw_point(unsigned char *buff, int x, int y, int Y, int U, int V);
	void draw_horizontal_line(unsigned char *buff, int x1, int x2, int y, int Y, int U, int V);

	void *GetOsdLineInfo_local(int x1, int y1, int x2, int y2, int thick);
};

#endif
