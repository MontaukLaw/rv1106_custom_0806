/*
 * ??????��?2007 ?????????????????
 * ?? ?? ????ptzLib.h
 * ?    ???????????��?????
 * ?????????2007-09-07
 * ??    ??: maohw
 * ??    ??: ????	????	???????????
 */

#ifndef __PTZLIB_H__
#define __PTZLIB_H__
#include <linux/rtc.h>
// ��??????
#define PTZ_PELCO_D 0x01
#define PTZ_PELCO_P 0x02
#define PTZ_PANASONIC 0x03
#define PTZ_YAAN 0x04
#define PTZ_SANTACHI 0x05
#define PTZ_SANTACHI_IN 0x06
#define PTZ_SAMSUNG 0x07
#define PTZ_ALEC 0x08
#define MATRIX_MS750 0x09
#define PTZ_CUSTOM01 0x0A

// ????????
#define PTZ_UPSTART 0x01
#define PTZ_UPSTOP 0x02
#define PTZ_DOWNSTART 0x03
#define PTZ_DOWNSTOP 0x04
#define PTZ_LEFTSTART 0x05
#define PTZ_LEFTSTOP 0x06
#define PTZ_RIGHTSTART 0x07
#define PTZ_RIGHTSTOP 0x08
#define PTZ_LEFTUPSTART 0x09
#define PTZ_LEFTUPSTOP 0x0a
#define PTZ_RIGHTUPSTART 0x0b
#define PTZ_RIGHTUPSTOP 0x0c
#define PTZ_LEFTDOWNSTART 0x0d
#define PTZ_LEFTDOWNSTOP 0x0e
#define PTZ_RIGHTDOWNSTART 0x0f
#define PTZ_RITHTDOWNSTOP 0x10

#define PTZ_PREVPOINTSET 0x11
#define PTZ_PREVPOINTCALL 0x12
#define PTZ_PREVPOINTDEL 0x13
#define PTZ_ZOOMADDSTART 0x14
#define PTZ_ZOOMADDSTOP 0x15
#define PTZ_ZOOMSUBSTART 0x16
#define PTZ_ZOOMSUBSTOP 0x17
#define PTZ_FOCUSADDSTART 0x18
#define PTZ_FOCUSADDSTOP 0x19
#define PTZ_FOCUSSUBSTART 0x1a
#define PTZ_FOCUSUBSTOP 0x1b
#define PTZ_IRISADDSTART 0x1c
#define PTZ_IRISADDSTOP 0x1d
#define PTZ_IRISSUBSTART 0x1e
#define PTZ_IRISSUBSTOP 0x1f

#define PTZ_GOTOZEROPAN 0x20
#define PTZ_FLIP180 0x21
#define PTZ_SETPATTERNSTART 0x22
#define PTZ_SETPATTERNSTOP 0x23
#define PTZ_RUNPATTERN 0x24
#define PTZ_SETAUXILIARY 0x25
#define PTZ_CLEARAUXILIARY 0x26

#define PTZ_AUTOSCANSTART 0x27
#define PTZ_AUTOSCANSTOP 0x28
#define PTZ_RANDOMSCANSTART 0x29
#define PTZ_RANDOMSCANSTOP 0x2a

/*cruise*/
#define PTZ_CRUISESTART 0x2b //??????
#define PTZ_CRUISESTOP 0x2c	 //?????
#define PTZ_REPOSITION 0x2d	 //?????��
/*3D PC=>3512*/
#define PTZ_POINTCENTER 0x30
#define PTZ_VIEWCENTER 0x31
#define PTZ_ZOOMCENTER 0x37
/*3D 3512=>MCU*/
#define PTZ_LEFT_OFFSET 0x32
#define PTZ_RIGHT_OFFSET 0x33
#define PTZ_DOWN_OFFSET 0x34
#define PTZ_UP_OFFSET 0x35
#define PTZ_ZOOM_MULTIPLE 0x36

#define PTZ_MAX_CMD_NUMBER 0x40

#define MAX_PTZ_NAME_LEN 12

// ????????
#define MATRIX_UDP_CONNECT 0x000			  // 0   UDP connect
#define MATRIX_LOGIN_CONNECT 0x001			  // 1   Login connect
#define MATRIX_LOGIN_WITH_PASSWORD 0x002	  // 2   Login with  operater-password
#define MATRIX_LOGIN_IN_AUTO 0x003			  // 3   login-in Auto
#define MATRIX_LOGOUT 0x004					  // 4   login-out
#define MATRIX_MONITOR_SELECT 0x005			  // 5   Monitor select
#define MATRIX_CAMERA_SELECT 0x006			  // 6   Camera select
#define MATRIX_QUWEI_SELECT 0x007			  // 7   qu wei select
#define MATRIX_TOUR_SELECT 0x008			  // 8   Tour  select
#define MATRIX_GROUP_SELECT 0x009			  // 9   Group select
#define MATRIX_BACK_SEQ 0x00a				  // 10   Back Seq
#define MATRIX_FOWARD_SEQ 0x00b				  // 11   Foward Seq
#define MATRIX_DEC 0x00c					  // 12   DEC -1
#define MATRIX_INC 0x00d					  // 13  INC +1
#define MATRIX_PAUSH 0x00e					  // 14  PAUSH
#define MATRIX_STOP_SEQ 0x00f				  // 15  Stop seq
#define MATRIX_HOLD_ON 0x010				  // 16  Hold ON
#define MATRIX_HOLD_OFF 0x011				  // 17   Hold OFF
#define MATRIX_DISPLAY_ALL 0x012			  // 18   Display  ALL
#define MATRIX_DISPLAY_TIME 0x013			  // 19   DISPLAY TIME
#define MATRIX_DISPLAY_TITLE 0x014			  // 20   Display TITLE
#define MATRIX_DISPLAY_STATUS 0x015			  // 21   DISPLAY STATUS
#define MATRIX_ALARM_AREA_STATUS 0x016		  // 22   AlarmAreaStatus  ???????????
#define MATRIX_DETECTOR_STATUS 0x017		  // 23   DetectorStatus   ????????????
#define MATRIX_KEY_JUMP 0x018				  // 24  JUMP
#define MATRIX_MS_SETUP_COMMAND 0x019		  // 25   MSsetupCommand   ???????????????????
#define MATRIX_DISP_STATUS 0x01a			  // 26  Display Status
#define MATRIX_REQUEST_MONITOR_STATUS 0x01b	  // 27  Request Monitor Status
#define MATRIX_REQUEST_MONITOR_MESSAGE 0x01c  // 28  Request Monitor Message
#define MATRIX_REQUEST_CAM_STATUS 0x01d		  // 29  Request CAM Status
#define MATRIX_REQUEST_CAM_MESSAGE 0x01e	  // 30  Request CAM Message
#define MATRIX_REQUEST_ALARM_MESSAGE 0x01f	  // 31  Request Alam Message
#define MATRIX_REQUEST_CONTROLS_MESSAGE 0x020 // 32  Request Controls Message
#define MATRIX_ALARM_STATUS_MENU 0x021		  // 33  AlarmStatusMenu ?????????????
#define MATRIX_ALARM_RECORD_MENU 0x022		  // 34  AlarmRecordMenu ??????????????
#define MATRIX_SYSTEM_STATUS_MENU 0x023		  // 35  SystemStatusMenu ???????????
#define MATRIX_MENU_EXIT 0x024				  // 36  MenuExit ??????
#define MATRIX_SUBNET_SELECT 0x025			  // 37 SubnetSelect ???????
#define MATRIX_SUBNET_EXIT 0x026			  // 38 SubnetExit ???????
#define MATRIX_SYSTEM_TIME 0x027			  // 39 transfer system time
#define MATRIX_USER_KEY 0x028				  // 40 transfer user key   ???????
#define MATRIX_SYSTEM_SETUP 0x029			  // 41 system setup   ??????
#define MATRIX_SUBNET_MON_SELECT 0x02a		  // 42  Subnet Mon select

// SetupCommand ???????????
#define MATRIX_SETUP_ENTER 0x02b // 43  SetupEnter
#define MATRIX_SETUP_DATA1 0x02c // 44  SetupDATA1
#define MATRIX_SETUP_DATA2 0x02d // 45  SetupDATA2
#define MATRIX_SETUP_DATA3 0x02e // 46  SetupDATA3
#define MATRIX_SETUP_DATA4 0x02f // 47  SetupDATA4
#define MATRIX_SETUP_DATA5 0x030 // 48  SetupDATA5
#define MATRIX_SETUP_DATA6 0x031 // 49  SetupDATA6
#define MATRIX_SETUP_DATA7 0x032 // 50  SetupDATA7
#define MATRIX_SETUP_DATA8 0x033 // 51  SetupDATA8
#define MATRIX_SETUP_DATA9 0x034 // 52  SetupDATA9
#define MATRIX_SETUP_DATA0 0x035 // 53  SetupDATA0
#define MATRIX_SETUP_UP 0x036	 // 54  SetupUP
#define MATRIX_SETUP_DOWN 0x037	 // 55  SetupDOWN
#define MATRIX_SETUP_LEFT 0x038	 // 56  SetupLEFT
#define MATRIX_SETUP_RIGHT 0x039 // 57  SetupRIGHT
#define MATRIX_SETUP_INC 0x03a	 // 58  SetupINC
#define MATRIX_SETUP_DEC 0x03b	 // 59  SetupDEC
#define MATRIX_SETUP_ESC 0x03c	 // 60  SetupESC
#define MATRIX_SETUP_SET 0x03d	 // 61  SetupSET
#define MATRIX_SETUP_NEXT 0x03e	 // 62  SetupNEXT
#define MATRIX_SETUP_PRE 0x03f	 // 63  SetupPRE
#define MATRIX_SETUP_POPUP 0x040 // 64  SetupPOP-UP
#define MATRIX_SETUP_PRINT 0x041 // 65  SetupPRINT
#define MATRIX_SETUP_CLEAR 0x042 // 66  SetupCLEAR
#define MATRIX_SETUP_SAVE 0x043	 // 67  SetupSAVE
#define MATRIX_SETUP_READ 0x044	 // 68  SetupREAD
#define MATRIX_SETUP_DELE 0x045	 // 69  SetupDELE

// AskAndAnswer??????????
#define MATRIX_ASK_DEVEICEID 0x046		 // 70 AskDeveiceID  ????��????
#define MATRIX_ANSWER_DEVEICEID 0x047	 // 71 AnswerDeveiceID  ????��????
#define MATRIX_ASK_SOFTWARE_VER 0x048	 // 72 AskSoftwareVer  ????????��???
#define MATRIX_ANSWER_SOFTWARE_VER 0x049 // 73 AnswerSoftwareVer  ????????��???
#define MATRIX_ASK_BOARD_STATUS 0x04a	 // 74 AskBoardStatus  ????????
#define MATRIX_ANSWER_BOARD_STATUS 0x04b // 75 AnswerBoardStatus  ????????

// AlarmControl ???????????
#define MATRIX_DEFEND_SETUP 0x04c		 // 76 Defend setup  ???
#define MATRIX_DEFEND_SOLVE 0x04d		 // 77  Defend solve????
#define MATRIX_BELL_OFF 0x04e			 // 78  Bell OFF  ?????
#define MATRIX_BELL_ON 0x04f			 // 79  Bell ON???��
#define MATRIX_BRACH_RODE 0x050			 // 80  BRACH RODE     ??��
#define MATRIX_CLEAR_RODE 0x051			 // 81  CLEAR RODE     ????��
#define MATRIX_RECORD_ON 0x052			 // 82  Record ON       ???
#define MATRIX_RECORD_OFF 0x053			 // 83  Record OFF      ??
#define MATRIX_ALARM_INC 0x054			 // 84  Alarm inc       ???????+1
#define MATRIX_ALARM_DEC 0x055			 // 85  ALarm dec       ???????-1
#define MATRIX_RESET_ONE_RECORD 0x056	 // 86  reset one record ??????��??1???
#define MATRIX_RESET_ALL_RECORD 0x057	 // 87  Reset all Record  ??????��???
#define MATRIX_RECORD_DISP 0x058		 // 88  Record disp       ??????????
#define MATRIX_WARN_MODE_BY_HAND 0x059	 // 89 Warn Mode BY hand ??????-???
#define MATRIX_WARN_MODE_TIME 0x05a		 // 90 Warn Mode Time    ??????-???
#define MATRIX_WARNING_RECORD_EXIT 0x05b // 91 WARING RECORED EXIT   ?????????????

//?????????
#define MATRIX_PAN_UP 0x05c		   // 92 PAN UP (Speed 1-8)
#define MATRIX_PAN_DOWN 0x05d	   // 93  PAN DOWN(Speed 1-8)
#define MATRIX_PAN_RIGHT 0x05e	   // 94  PAN RIGHT(Speed 1-8)
#define MATRIX_PAN_LEFT 0x05f	   // 95  PAN LEFT(Speed 1-8)
#define MATRIX_PAN_RIGHTUP 0x060   // 96  PAN RIGHT UP(Speed 1-8)
#define MATRIX_PAN_LEFTUP 0x061	   // 97  PAN LEFT UP(Speed 1-8)
#define MATRIX_PAN_RIGHTDOWN 0x062 // 98  PAN RIGHT DOWN(Speed 1-8)
#define MATRIX_PAN_LEFTDOWN 0x063  // 99  PAN LEFT DOWN(Speed 1-8)
#define MATRIX_PAN_STOP 0x064	   // 100  PAN STOP

//???????
#define MATRIX_IRIS_OPEN 0x065			// 101  IRIS OPEN
#define MATRIX_IRIS_CLOSE 0x066			// 102  IRIS CLOSE
#define MATRIX_FOCUS_STOP 0x067			// 103  IRIS STOP
#define MATRIX_FOCUS_NEAR 0x068			// 104  FOCUS NEAR
#define MATRIX_FOCUS_FAR 0x069			// 105  FOCUS FAR
#define MATRIX_FOCUS_TITLE 0x06a		// 106  FOCUS TITLE
#define MATRIX_FOCUS_WIDE 0x06b			// 107  FOCUS WIDE
#define MATRIX_AUTO_FOCUS_ENABLE 0x06c	// 108  AUTO focus enable   ????????????
#define MATRIX_AUTO_FOCUS_DISABLE 0x06d // 109  AUTO focus disable  ??????????
#define MATRIX_AUTO_IRIS_ENABLE 0x06e	// 110  auto iris  enable  ??????????
#define MATRIX_AUTO_IRIS_DISABLE 0x06f	// 111  auto iris  disable ?????????
#define MATRIX_STOP 0x070				// 112   STOP (--NEAR WIDE FAR TELE) ???????

//??????????
#define MATRIX_AUTO_H_PAN 0x071		// 113  AUTO H PAN     ???????????
#define MATRIX_AUTO_PAN_STOP 0x072	// 114  AUTO PAN STOP  ?????????????
#define MATRIX_RANDM_H_PAN 0x073	// 115  RANDM H PAN    ???????????
#define MATRIX_RANDM_PAN_STOP 0x074 // 116  RANDM PAN STOP  ?????????????
//
#define MATRIX_AUX1_ON 0x075		  // 117   AUX1 ON      ?????????
#define MATRIX_AUX1_OFF 0x076		  // 118  AUX1 OFF              ?????????
#define MATRIX_RAIN_BRUSH_OPEN 0x077  // 119  Rain Brush Open ????????
#define MATRIX_RAIN_BRUSH_CLOSE 0x078 // 120  Rain Brush Close ????????
#define MATRIX_DEFORST_OPEN 0x079	  // 121  Deforst Open    ????????
#define MATRIX_DEFORST_CLOSE 0x07a	  // 122  Deforst Close   ????????
#define MATRIX_HEAT_ON 0x07b		  // 123  Heart On        ?????
#define MATRIX_HEAT_CLOSE 0x07c		  // 124  Heart Close     ?????
#define MATRIX_POWER_ON 0x07d		  // 125 Power On        ????????
#define MATRIX_POWER_CLOSE 0x07e	  // 126  Power Close     ????????

//?????
#define MATRIX_CAMERA_POSITION_CALL 0x07f // 127   CAMER Position call 1-999    ?????��?????
#define MATRIX_PRESET_CALL 0x080		  // 128   Preset  call     ???��?????
#define MATRIX_PRESET_SET 0x081			  // 129   Preset  Set      ???��??????
#define MATRIX_PRESET_RESET 0x082		  // 130  Preset Reset     ???��???��
#define MATRIX_FUNCTION_SELECT 0x083	  // 131   Function select  ???????
#define MATRIX_TURN_OVER 0x084			  // 132  TURN OVER        ???

#define MATRIX_CAM_ENTER_SETUP_MENU 0x085 // 133  CAM ENTER SETUP MENU  ????????��??????
#define MATRIX_CURUP 0x086				  // 134  CUR UP           ?????????-??
#define MATRIX_CURDN 0x087				  // 135  CUR DN           ?????????-??
#define MATRIX_CURLF 0x088				  // 136  CUR LF           ?????????-??
#define MATRIX_CURRT 0x089				  // 137  CUR RT           ?????????-??
#define MATRIX_CONFIREM 0x08a			  // 138  CONFIREM         ?????????-???
#define MATRIX_ESC 0x08b				  // 139  ESC              ?????????-???
#define MATRIX_CAM_INC 0x08c			  // 140  INC +1               ?????????-???
#define MATRIX_CAM_DEC 0x08d			  // 141  DEC -1               ?????????-???

//????
#define MATRIX_SMART_BEGIN 0x08e  // 142  SMART-Begin       ????-???
#define MATRIX_SMART_END 0x08f	  // 143  SMART_End
#define MATRIX_SMART_REPEAT 0x090 // 144  SMART_Repeat
#define MATRIX_SMART_RUN 0x091	  // 145  SMART_Run

#define MAX_MATRIX_COMMAND_NUM 0x92

typedef struct __ptz_cmd_param_t
{
	int cmd;
	int value;
	char user_name[12];
	char user_pwd[12];
} ptz_cmd_param_t;

int st_ptz_initPTZLib(void);
int st_ptz_deinitPTZLib(void);
int comm_ptz_sendCommand(int channel, ptz_cmd_param_t *param);
void st_ptz_sendTransparentData(char *buf, int size);

typedef struct __matrix_login_t
{
	char user_name[12];
	char user_pwd[12];
} matrix_login_t;

typedef struct __matrix_control_t
{
	int param_control;
	char unusd[32];
} matrix_control_t;
typedef struct rtc_time rtc_time_t;
int st_matrix_sendCommand(int command_type, char *data);
int st_ptz_pelco_dGetTime(rtc_time_t *p_rtc_time); // get time through ptz_cmd with pelco_d
int st_ptz_pelco_dSetTime(rtc_time_t *p_rtc_time);
int st_ptz_pelco_dGetAlarmInState(unsigned char *state);
int st_ptz_pelco_dSetAlarmOutState(unsigned char state);
int st_ptz_pelco_dSetVideoFrame(int device_type, int frame_rate);
int st_ptz_pelco_dGetVersion(char *ver);
void comm_ptz_start_3AService();
void comm_ptz_stop_3AService();
int comm_ptz_sendHumanData(int x_p, int y_p, int w_p, int h_p);
void comm_set_3AGridWights(unsigned char *pDayGridWeights, unsigned char *pNightGridWeights);

#endif
