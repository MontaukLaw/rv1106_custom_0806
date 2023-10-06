#include "common.h"
#include "logLib.h"

extern int executeCMD(const char *cmd, char *result, int max_len);

char * str_get_next(char *nptr, char **  endptr);
static char  wifi_node_name[50];

int wifi_set_node_name(char*name)
{
	if(name)
	{
		memset(wifi_node_name, 0, sizeof(wifi_node_name));
		strncpy(wifi_node_name, name, sizeof(wifi_node_name) - 1);
	}
	
	return 0;
}


int wifi_scan_info(char *buff, int max_len)
{
	int ret = -1;
	char cmd[200];
	
	if(buff == NULL || max_len < 90)
	{
		os_dbgerr("error para!!!");
		goto exit__;
	}
	if(wifi_node_name[0] == '\0')
		strcpy(wifi_node_name, "wlan0");
	
	sprintf(cmd, "wpa_cli -i %s -p /var/run/wpa_supplicant scan", wifi_node_name);
	
	system(cmd);
	
	usleep(2 * 1000 * 1000);
	
	
	
	sprintf(cmd, "wpa_cli -i %s -p /var/run/wpa_supplicant scan_results", wifi_node_name);
	
	
	ret = executeCMD(cmd, buff, max_len);
	
	if(ret >= 0)
		ret = strlen(buff) > 90 ? 0 : -1;
	
exit__:
	return ret;
}


int wifi_connect_ap(char * ssid, char *passwd)
{
	int ret = -1;
	char cmd[200];
	char *res = NULL;
	int try_times = 4;
	int i ;
	char *ptr1, *ptr2;
	
	if(ssid == NULL)
	{
		os_dbgerr("error para!!!");
		goto exit__;
	}

	res = (char*)malloc(1500);

	os_dbg("enter...");
	
	if(wifi_node_name[0] == '\0')
		strcpy(wifi_node_name, "wlan0");

	sprintf(cmd, "ifconfig %s up", wifi_node_name);
	system(cmd);
	if(system("ps | grep rkwifi_server | grep -v grep") != 0)
	{
		os_dbgerr("no find rkwifi_server running");
		goto exit__;
	}
	usleep(100* 1000);

	//1, 判断当前 的ssid 是否连接，如果连接，则退出。
	i = 0;
	while(try_times -- > 0)
	{
		char ssidline[50];
		sprintf(cmd, "rkwifi_server  getinfo %s", ssid);
		system(cmd);
		usleep(20 * 1000);
		//get log
		sprintf(cmd, "tail -n 30 /tmp/.rkwifi_server.log");
		executeCMD(cmd, res, 1500);
		
		sprintf(ssidline, "ssid=%s", ssid);
		ptr1 = strstr(res, ssidline);
		if(ptr1)
		{
			ptr1 += strlen(ssidline);
			ptr1 = strstr(ptr1, "wpa_state=");
			if(ptr1)
			{
				ptr1 += strlen("wpa_state=");
				ptr2 = str_get_next(ptr1, NULL);
				
				if(strcmp(ptr1, "COMPLETED") == 0)
				{
					ret = 0;
					goto exit__;
				}
			}
			
		}
		usleep(500  * 1000);
	}
	os_dbg("not connected!!!");
	
		
	// 删除可能存在的ssid 记录，重新连接。
	wifi_remove_ap(ssid);
	
	//3. 开始连接热点
	sprintf(cmd, "rkwifi_server connect %s %s", ssid, passwd);
	system(cmd);
	
	ret = 0;
exit__:
	if(res) free(res);
	return ret;
}

bool wifi_is_connected(void)
{
	bool retb = false;
	char cmd[20];
	char *res;
	int ret;
	
	res = (char*)malloc(2000);
	if(res == NULL)
	{
		os_dbgerr("malloc fail");
		goto exit__;
	}
	
	ret = executeCMD("ifconfig", res, 2000);
	if(ret >= 0)
	{
		char * ptr;
		ptr = strstr(res, wifi_node_name);
		if(ptr)
		{
			ptr = strstr(ptr, "inet addr");
			retb = ptr != NULL;
		}
	}
	
exit__:
	if(res) free(res);

	return retb;
}

int wifi_remove_ap(char * ssid)
{
	char cmd[200];
	
	sprintf(cmd, "rkwifi_server disconnect %s", ssid);
	system(cmd);
	usleep(100* 1000);
	
	sprintf(cmd, "rkwifi_server forget %s", ssid);
	system(cmd);
	usleep(100* 1000);
	
}

// 取得下一个非空字符串(不含 空字符 的字符串)。
char * str_get_next(char *nptr, char **  endptr)
{
	char *s = nptr, *b=NULL;
	char c;

	if(!nptr) goto exit__;

	//1，去掉前面空格
	do { 
		c = *s++; 
    	} while (isspace((unsigned char)c) && c != '\0'); 
	if(c == '\0') goto exit__;
	
	b = s-1;
	do
	{
		c = *s++; 
	}while((!isspace((unsigned char)c)) && c != '\0');


	if(endptr)
	{
		if(c == '\0')
		{
			*endptr = NULL; // 表示 解析结束。
		}
		else
			*endptr = s;
	}
	
	s--;
	*s = '\0'; //会修改原有字符串。
	
	
exit__:
	return b;
}
