#ifndef __YOLO_DETECT_H__
#define __YOLO_DETECT_H__

#include <stdint.h>

#define OBJ_NAME_MAX_SIZE 16
#define OBJ_NUMB_MAX_SIZE 64

#define IMAGE_INPUT_WIDTH 640
#define IMAGE_INPUT_HEIGHT 640
#define IO_IN_NUMBER 1
#define IO_OUT_NUMBER 3

#define NMS_THRESH 0.45
#define BOX_THRESH 0.35

#define OBJ_CLASS_NUM 3
#define PROP_BOX_SIZE (5 + OBJ_CLASS_NUM)
#define DETECT_ITERVAL_FRAME 5
#define DETECT_STATUS_INTERVAL_FRAME 20

#define MAX_OBJNUM 30

#define GAMING 1
#define NO_GAMING 0

    typedef enum rkCOLOR_INDEX_E
    {
        RGN_COLOR_LUT_INDEX_0 = 0,
        RGN_COLOR_LUT_INDEX_1 = 1,
    } COLOR_INDEX_E;

    typedef struct _BOX_RECT
    {
        int left;
        int right;
        int top;
        int bottom;
    } BOX_RECT;

    typedef struct __detect_result_t
    {
        char name[OBJ_NAME_MAX_SIZE];
        BOX_RECT box;
        float prop;
    } detect_result_t;

    typedef struct _detect_result_group_t
    {
        int id;
        int obj_num;
        detect_result_t results[OBJ_NUMB_MAX_SIZE];
    } detect_result_group_t;

    typedef struct node
    {
        long timeval;
        detect_result_group_t detect_result_group;
        struct node *next;
    } Node;

    typedef struct my_stack
    {
        int size;
        Node *top;
    } rknn_list_t;

    // typedef struct comm_rect_t
    // {
    //     int left;   ///< Most left coordinate
    //     int top;    ///< Most top coordinate
    //     int right;  ///< Most right coordinate
    //     int bottom; ///< Most bottom coordinate
    // } comm_rect_t;

    // typedef struct nn_object_t
    // {
    //     int id; ///< Track id
    //     // int cls_idx;            ///< Class index
    //     unsigned int cls_idx : 8;
    //     unsigned int color : 24;
    //     comm_rect_t box; ///< Object Region
    //     float score;     ///< Object confidence score
    //     char name[OBJ_NAME_MAX_SIZE];
    // } nn_object_t;

    // typedef struct nn_object_array_t
    // {
    //     int count;                      ///< Array Count(0 <= count < 256)
    //     nn_object_t object[MAX_OBJNUM]; ///< Objects
    // } nn_object_array_t;

    void create_rknn_list(void);

    // int rknn_detect(unsigned char *input_data, detect_result_group_t *detect_result_group);
    int rknn_detect(unsigned char *input_data);
    // int rknn_detect(unsigned char *input_data, nn_object_array_t *detect_result_group);
    int init_model(void);

    void quit_rknn(void);
    
    long int get_current_time_sec(void);

#endif // __YOLO_DETECT_H__