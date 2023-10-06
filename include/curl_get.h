#ifndef __CURL_GET_HHH__
#define __CURL_GET_HHH__

#ifdef __cplusplus
extern "C"
{
#endif

#include "string.h"
#include "stdio.h"

    int http_get_detect_status(void);

    int get_detect_status_val(void);

    int http_send_game_status_event(void);
    
    void get_ip(void);

    void send_detect_result(uint8_t if_game_start, uint8_t if_person_in_roi);

    int http_get_if_draw_roi(void);

    int get_if_draw_roi_val(void);

#ifdef __cplusplus
}
#endif

#endif
