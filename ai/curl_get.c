#include "curl/curl.h"
#include "yolo_detect.h"

#include <stdio.h>
#include <net/if.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

// #define GET_DETECT_STATUS_URL "http://192.168.20.162:18080/oms/equipment/queryOperate?equipmentCode="
// 发送开球事件
// #define SEND_GAME_START_URL "http://192.168.20.162:18080/oms/equipment/matchStart?equipmentCode="
// #define SEND_GAME_STOP_URL "http://192.168.20.162:18080/oms/equipment/matchStop?equipmentCode="

// 查询接口
char get_detect_status_url[200];

// 发送开球事件
char send_game_start_url[200];
char send_game_stop_url[200];

// 是否绘制roi框的接口url
char if_draw_roi_box_url[200];

// 自己的IP
char my_ip_last_part_str[20];
// int my_ip_last_part = 0;

// http访问超时时间
#define GET_TIME_OUT_SEC 1L
#define SEND_STATUS_TIME_OUT_SEC 3L

static int detect_status = 0;
int access_port = 0;
extern int g_game_status;

static int if_draw_roi = 0;

int get_if_draw_roi_val(void)
{
    return if_draw_roi;
}

// 获取上位机指令, 是否开始检测
int get_detect_status_val(void)
{
    return detect_status;
}

// 回调函数，用于处理返回的数据
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{

    int status = atoi((char *)ptr);
    detect_status = status;

    write_log("g_detect_status is %d\n", detect_status);

    return size * nmemb;
}

// 回调函数，用于处理返回的数据
size_t if_draw_roi_write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{

    int status = atoi((char *)ptr);
    if_draw_roi = status;

    write_log("if_draw_roi is %d\n", if_draw_roi);

    return size * nmemb;
}

// 通过curl库获取上位机指令, 是否开始检测
int http_get_if_draw_roi(void)
{

    CURL *curl;
    CURLcode res;
    
    if_draw_roi = 0;

    // 初始化CURL
    curl = curl_easy_init();
    if (curl)
    {
        // 设置要访问的URL
        curl_easy_setopt(curl, CURLOPT_URL, if_draw_roi_box_url);

        // 设置回调函数来处理返回的数据
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, if_draw_roi_write_callback);

        // 设置超时时间
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, GET_TIME_OUT_SEC);

        // 执行GET请求
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // 清理CURL句柄
        curl_easy_cleanup(curl);
    }

    return 0;
}

// 通过curl库获取上位机指令, 是否开始检测
int http_get_detect_status(void)
{
    CURL *curl;
    CURLcode res;

    // 初始化CURL
    curl = curl_easy_init();
    if (curl)
    {
        // 设置要访问的URL
        curl_easy_setopt(curl, CURLOPT_URL, get_detect_status_url);

        // 设置回调函数来处理返回的数据
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        curl_easy_setopt(curl, CURLOPT_TIMEOUT, GET_TIME_OUT_SEC);

        // 执行GET请求
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // 清理CURL句柄
        curl_easy_cleanup(curl);
    }

    return 0;
}

// 发送开球事件
int http_send_game_status_event(void)
{
    CURL *curl;
    CURLcode res;

    // 初始化CURL
    curl = curl_easy_init();
    if (curl)
    {
        // 设置要访问的URL
        if (g_game_status == GAMING)
        {
            curl_easy_setopt(curl, CURLOPT_URL, send_game_start_url);
        }
        else
        {
            curl_easy_setopt(curl, CURLOPT_URL, send_game_stop_url);
        }

        // 设置回调函数来处理返回的数据
        // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, SEND_STATUS_TIME_OUT_SEC);

        // 执行GET请求
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // 清理CURL句柄
        curl_easy_cleanup(curl);
    }

    return 0;
}

int get_localip(const char *eth_name, char *local_ip_addr)
{
    int ret = -1;
    register int fd;
    struct ifreq ifr;

    if (local_ip_addr == NULL || eth_name == NULL)
    {
        return ret;
    }
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) > 0)
    {
        strcpy(ifr.ifr_name, eth_name);
        if (!(ioctl(fd, SIOCGIFADDR, &ifr)))
        {
            ret = 0;
            strcpy(local_ip_addr, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
        }
    }
    if (fd > 0)
    {
        close(fd);
    }
    return ret;
}

void get_ip(void)
{
    char local_ip[20];
    get_localip("eth0", local_ip);
    // sprintf(que, "local ip is %s\n", local_ip);
    printf("local ip is %s\n", local_ip);

    uint8_t ip_length = strlen(local_ip);

    uint8_t dot_counter = 0;

    // uint8_t char_counter = 0;
    for (int i = 0; i < strlen(local_ip); i++)
    {
        if (local_ip[i] == '.')
        {
            dot_counter++;
        }
        if (dot_counter == 3)
        {
            memcpy(my_ip_last_part_str, local_ip + i + 1, ip_length - i);
            break;
        }

        // char_counter++;
    }

    write_log("ipLastPart is %s\n", my_ip_last_part_str);

    // sprintf(get_detect_status_url, "http://192.168.20.162:18080/oms/equipment/queryOperate?equipmentCode=%s", ipLastPart);

    // sprintf(send_game_start_url, "http://192.168.20.162:18080/oms/equipment/matchStart?equipmentCode=%s", ipLastPart);

    // sprintf(send_game_stop_url, "http://192.168.20.162:18080/oms/equipment/matchStop?equipmentCode=%s", ipLastPart);

    sprintf(if_draw_roi_box_url, "http://192.168.20.162:18080/oms/equipment/queryRecognize?equipmentCode=%s", my_ip_last_part_str);

    write_log("if_draw_roi_box_url is %s\n", if_draw_roi_box_url);
    // write_log("send_game_start_url is %s\n", send_game_start_url);
    // write_log("send_game_stop_url is %s\n", send_game_stop_url);
}

// 跟据检测结果发送消息给后台接口
// http://192.168.20.162:18080/oms/equipment/behaviorChange?equipmentCode=101&openBall=1&someone=0
void send_detect_result(uint8_t if_game_start, uint8_t if_person_in_roi)
{
    char url[300];

    sprintf(url, "http://192.168.20.162:18080/oms/equipment/behaviorChange?equipmentCode=%s&openBall=%d&someone=%d",
            my_ip_last_part_str, if_game_start, if_person_in_roi);

    write_log("access send_detect_result url %s\n", url);

    CURL *curl;
    CURLcode res;

    // 初始化CURL
    curl = curl_easy_init();
    if (curl)
    {
        // 设置要访问的URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_TIMEOUT, GET_TIME_OUT_SEC);

        // 执行GET请求
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // 清理CURL句柄
        curl_easy_cleanup(curl);
    }
}