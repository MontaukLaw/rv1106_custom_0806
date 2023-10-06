#ifndef AO_C_IF_H_________
#define AO_C_IF_H_________

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


bool app_audio_init_play(int samplerate, int chan_num, AUDIO_CODEC_TYPE codec_type);

bool app_audio_start_play(void);

// volume: 0 - 100
bool app_audio_set_volume(int volume);

bool app_audio_stop_play(void);

int app_audio_send_data_to_play(unsigned char * data, int len);

bool app_audio_deinit_play(void);


#ifdef __cplusplus
};
#endif

#endif