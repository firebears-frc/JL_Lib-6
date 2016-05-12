/*
 * JL_Lib
 * Copyright (c) 2015 Jeron A. Lau 
*/
/** \file
 * JLGRmouse.c
 *	This file handles the mouse.
**/
#include "JLGRinternal.h"

//
// Static Functions
//

//
// Used elsewhere in library.
//

// Run when mouse needs to be redrawn.
void jlgr_mouse_draw_(jl_t* jl, jl_sprite_t* sprite) {
	// Draw mouse, if using a computer.
#if JL_PLAT == JL_PLAT_COMPUTER //if computer
	jlgr_t* jlgr = jl->jlgr;
	jl_sprite_t* mouse = jlgr->mouse;
	jl_vo_t* mouse_vo = mouse->ctx;
	jl_rect_t rc = { 0.f, 0.f, 1.f, 1.f };

	jlgr_vos_image(jlgr, &(mouse_vo[0]), rc, 0,
		JL_IMGI_FONT, 255, 255);
	jlgr_draw_vo(jlgr, mouse_vo, NULL);
#endif
}

// Run every frame for mouse
void jlgr_mouse_loop_(jl_t* jl, jl_sprite_t* sprite) {
	jlgr_t* jlgr = jl->jlgr;
	jl_sprite_t* mouse = jlgr->mouse;

//Update Mouse
	mouse->cb.x = jl_ct_gmousex(jlgr);
	mouse->cb.y = jl_ct_gmousey(jlgr);
}

void jlgr_mouse_init__(jlgr_t* jlgr) {
	jl_rect_t rc = { 0.f, 0.f, .075f, .075f };
	jl_sprite_t* mouse;
	#if JL_PLAT == JL_PLAT_COMPUTER //if computer
		jl_vo_t *mouse_vo = jl_gl_vo_make(jlgr, 1);
	#endif

	jlgr->mouse = jlgr_sprite_new(
		jlgr, rc, jlgr_mouse_draw_,
		jlgr_mouse_loop_,
	#if JL_PLAT == JL_PLAT_COMPUTER //if computer
			sizeof(jl_vo_t*));
		// Set the context to the vertex object.
		((jl_sprite_t*)jlgr->mouse)->ctx = mouse_vo;
	#elif JL_PLAT == JL_PLAT_PHONE // if phone
			0);
		// Set the context to NULL.
		((jl_sprite_t*)jlgr->mouse)->ctx = NULL;
	#endif
	mouse = jlgr->mouse;
	jlgr_sprite_resize(jlgr, mouse, NULL);
	// Set the mouse's collision width and height to 0
	mouse->cb.w = 0.f;
	mouse->cb.h = 0.f;
}
