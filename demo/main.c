#include "common.h"
#include "comm_type.h"
// #include "param.h"
#include "comm_codec.h"
#include "codec2_process.h"
#include "yolo_detect.h"

extern int mcu_proc_start(void);

static bool quit = false;
static void sigterm_handler(int sig)
{
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sigterm_handler);

	os_dbg("--------  enter main !!!!! -------------");
	init_common_lib();
	comm_param_initRtcTime();
	comm_param_initParamLib();
	comm_param_startParamService();
	comm_param_startNetwork();
	comm_init_sched();

	comm_app_init();
	// ai process start
	comm_ai_init();
	app_start_ai_process();

	app_init_ircut();
	comm_com_initComLib();

	// app_init_storage();
	comm_ptz_start_3AService();
	st_net_startEncNetService();
	// app_audio_init_play(8000, 1, AUDIO_CODEC_G711U);

	comm_init_record("/userdata/nfs_share");

	comm_start_record(0, 0, 0);

	while (!quit)
	{
		usleep(200 * 1000);
		// app_ircut_refresh(CAM0_STREAM_MAIN); //有ircut 硬件的话，可以调用。
	}

	// quit_rknn();
	comm_app_deinit();
	os_dbg("--------  exit main !!!!! -------------");

	return 0;
}
