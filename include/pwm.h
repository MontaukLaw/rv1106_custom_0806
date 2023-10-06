#ifndef PWM_H___
#define PWM_H___

#ifdef __cplusplus
extern "C" {
#endif

int pwm_set(int pwmid, int freq, int percent_of_duty);
int pwm_start(int pwmid);
int pwm_set_duty(int pwmid, int percent_of_duty);
int pwm_stop(int pwmid);
int pwm_close(int pwmid);


#ifdef __cplusplus
};
#endif


#endif