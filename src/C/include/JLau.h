#ifndef JLAU
#define JLAU

#include "jl.h"
#include "SDL_mixer.h"

// Types:
typedef struct {
	Mix_Music *_MUS;
	char _VOL;
}jlau_jmus_t__;

typedef struct{
	jl_t* jl;
	uint32_t idis; //Which music to play next
	uint8_t sofi; //Seconds Of Fade In
	uint8_t sofo; //Seconds Of Fade Out
	int total; //Music Stack Maximum Music pieces
	jlau_jmus_t__ *jmus; //Pointer Of "total" # of Music Pieces
	double pofr; //Point Of Return (Where Music Should Start)
}jlau_t;

// Prototypes:
void jlau_load(jlau_t* jlau, u32_t IDinStack, const void *data, u32_t dataSize,
	u8_t volumeChange);
void jlau_mus_play(jlau_t* jlau, u32_t IDinStack, u8_t secondsOfFadeOut,
	u8_t secondsOfFadeIn);
void jlau_mus_halt(u8_t p_secondsOfFadeOut);
uint8_t jlau_mus_playing(void);
void jlau_panning(uint8_t left, uint8_t toCenter);
void jlau_panning_default(void);
void jlau_add_audio(jlau_t* jlau, str_t pzipfile, uint16_t pigid);
jlau_t* jlau_init(jl_t* jl);
void jlau_kill(jlau_t* jlau);

#endif
