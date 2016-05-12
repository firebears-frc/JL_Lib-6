#include "jl_pr.h"

/**
 * Reset a timer.
 * @param jl: The library context.
 * @param timer: Pointer to timer variable.
**/
double jl_sdl_timer(jl_t* jl, uint32_t* timer) {
	uint32_t prev_tick = *timer; // Temporarily Save Old Value
	*timer = SDL_GetTicks(); // Set New Value
	// milliseconds / 1000 to get seconds
	return ((double)(*timer - prev_tick)) / 1000.;
}

double jl_sdl_seconds_past__(jl_t* jl) {
	jl->time.this_tick = SDL_GetTicks();
	// milliseconds / 1000 to get seconds
	return ((double)(jl->time.this_tick - jl->time.prev_tick)) / 1000.;
}

// internal functions:

void jl_sdl_init__(jl_t* jl) {
	jl->time.psec = 0.f;
	jl->time.prev_tick = 0;
	jl->time.fps = JL_FPS;
}
