#include <stdint.h>
#include <stdio.h>
#include "gen/JL_Lib.h"

// Get Embeded Media
char *jl_gem(void) {
	return JL_Lib;
}

uint32_t jl_gem_size(void) {
	return sizeof(JL_Lib);
}
