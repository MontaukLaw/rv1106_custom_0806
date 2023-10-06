#ifndef __GPIO_H_XXXXXXXXXXXXXX
#define __GPIO_H_XXXXXXXXXXXXXX

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define GPIO_DIRECT_IN        0
#define GPIO_DIRECT_OUT 			1

#define GPIO_LEVEL_H					1
#define GPIO_LEVEL_L					0


int comm_gpio_set_direction(int gpioid, int isout);
int comm_gpio_get_level(int gpioid);
int comm_gpio_set_level(int gpioid, int ishigh);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif