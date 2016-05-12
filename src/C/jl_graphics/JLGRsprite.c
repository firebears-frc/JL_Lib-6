/*
 * JL_Lib
 * Copyright (c) 2015 Jeron A. Lau 
*/
/** \file
 * JLGRsprite.c
 *	Handles the sprites.
 */
#include "JLGRinternal.h"

static void jlgr_sprite_draw_to_pr__(jl_t *jl) {
	jl_sprite_t *sprite = jl_mem_temp(jl, NULL);

	((jlgr_sprite_fnt)sprite->draw)(jl, sprite);
}

static void jlgr_sprite_redraw_tex__(jlgr_t* jlgr, jl_sprite_t *spr) {
	jl_mem_temp(jlgr->jl, spr);
	jl_gl_pr(jlgr, spr->pr, jlgr_sprite_draw_to_pr__);
}

// Redraw a sprite
static inline void jlgr_sprite_redraw__(jlgr_t* jlgr, jl_sprite_t *spr) {
	// If pre-renderer hasn't been intialized, initialize & redraw.
	if(!spr->pr) jlgr_sprite_resize(jlgr, spr, NULL);
	// Else, Redraw texture.
	else jlgr_sprite_redraw_tex__(jlgr, spr);
}

//
// Exported Functions
//

/**
 * THREAD: Any thread.
 * Empty sprite loop. ( Don't do anything )
 * @param jl: The library context
 * @param spr: The sprite
**/
void jlgr_sprite_dont(jl_t* jl, jl_sprite_t* sprite) { }

/**
 * THREAD: Main thread only.
 * Run a sprite's draw routine to draw on it's pre-rendered texture.
 *
 * @param jl: The library context
 * @param spr: Which sprite to draw.
**/
void jlgr_sprite_redraw(jlgr_t* jlgr, jl_sprite_t *spr) {
	jl_thread_mutex_lock(jlgr->jl, spr->mutex);
	spr->update = 1;
	jl_thread_mutex_unlock(jlgr->jl, spr->mutex);
}

/**
 * THREAD: Draw thread only.
 * Render a sprite's pre-rendered texture onto the screen.
 *
 * @param jl: The library context.
 * @param spr: The sprite.
**/
void jlgr_sprite_draw(jlgr_t* jlgr, jl_sprite_t *spr) {
	jl_thread_mutex_lock(jlgr->jl, spr->mutex);

	jl_pr_t *pr = spr->pr;

	if(!pr) {
		jl_print(jlgr->jl, "jlgr_sprite_drw: not init'd!"); exit(-1);
	}
	// Redraw if needed.
	if(spr->update) jlgr_sprite_redraw__(jlgr, spr);
	spr->update = 0;

	jl_gl_transform_pr_(jlgr, pr, spr->cb.x, spr->cb.y, 0.,
		1., 1., 1.);

	jl_gl_draw_pr_(jlgr->jl, pr);
	//
	jl_thread_mutex_unlock(jlgr->jl, spr->mutex);
}

/**
 * THREAD: Draw thread only.
 * Resize a sprite to the current window - and redraw.
 * @param jlgr: The library context.
 * @param spr: The sprite to use.
**/
void jlgr_sprite_resize(jlgr_t* jlgr, jl_sprite_t *spr, jl_rect_t* rc) {
	u16_t res = (jlgr->gl.cp ? jlgr->gl.cp->w : jlgr->wm.w)
		* spr->rw;

	jl_thread_mutex_lock(jlgr->jl, spr->mutex);
	// 
	if(rc) {
		// Set collision box.
		spr->cb.x = rc->x; spr->cb.y = rc->y;
		spr->cb.w = rc->w; spr->cb.h = rc->h;
		// Set real dimensions
		spr->rw = rc->w;
		spr->rh = rc->h;
	}
	// Initialize or Resize
	if(jl_gl_pr_isi_(jlgr, spr->pr)) {
		jl_gl_pr_rsz(jlgr,spr->pr,spr->rw,spr->rh,res);
	}else{
		spr->pr = jl_gl_pr_new(jlgr,
			spr->rw, spr->rh, res);
	}
	// Redraw
	jlgr_sprite_redraw_tex__(jlgr, spr);
	//
	jl_thread_mutex_unlock(jlgr->jl, spr->mutex);
}

/**
 * THREAD: Main thread only.
 * Run a sprite's loop.
 * @param jl: The library context.
 * @param spr: Which sprite to loop.
**/
void jlgr_sprite_loop(jlgr_t* jlgr, jl_sprite_t *spr) {
	jl_thread_mutex_lock(jlgr->jl, spr->mutex);
	jl_print_function(jlgr->jl, "Sprite/Loop");
	((jlgr_sprite_fnt)spr->loop)(jlgr->jl, spr);
	jl_print_return(jlgr->jl, "Sprite/Loop");
	jl_thread_mutex_unlock(jlgr->jl, spr->mutex);
}

/**
 * THREAD: Main thread only.
 * Create a new sprite.
 *
 * @param jl: The library context.
 * @param rc: The rectangle bounding box & pr size.
 * @param a: the transparency each pixel is multiplied by; 255 is
 *	solid and 0 is totally invisble.
 * @param draw: the draw function.
 * @param loop: the loop function.
 * @param ctxs: how many bytes to allocate for the sprite's context.
 * @returns: the new sprite
**/
jl_sprite_t * jlgr_sprite_new(jlgr_t* jlgr, jl_rect_t rc,
	jlgr_sprite_fnt draw, jlgr_sprite_fnt loop, u32_t ctxs)
{
	jl_sprite_t *spr = NULL;

	spr = malloc(sizeof(jl_sprite_t));
	// Set collision box.
	spr->cb.x = rc.x; spr->cb.y = rc.y;
	spr->cb.w = rc.w; spr->cb.h = rc.h;
	// Set real dimensions
	spr->rw = rc.w;
	spr->rh = rc.h;
	// Set draw function.
	spr->draw = draw;
	// Set loop
	spr->loop = loop;
	// No pre-renderer made yet.
	spr->pr = NULL;
	// Make mutex
	spr->mutex = jl_thread_mutex_new(jlgr->jl);
	// Allocate context
	if(ctxs) spr->ctx = malloc(ctxs);
	return spr; 
}

/**
 * THREAD: Main thread only.
 * test if 2 sprites collide.
 *
 * @param 'jl': library context
 * @param 'sprite1': sprite 1
 * @param 'sprite2': sprite 2
 * @return 0: if the sprites don't collide in their bounding boxes.
 * @return 1: if the sprites do collide in their bounding boxes.
**/
u8_t jlgr_sprite_collide(jlgr_t* jlgr,
	jl_sprite_t *sprite1, jl_sprite_t *sprite2)
{
	if (
		(sprite1->cb.y >= (sprite2->cb.y+sprite2->cb.h)) ||
		(sprite1->cb.x >= (sprite2->cb.x+sprite2->cb.w)) ||
		(sprite2->cb.y >= (sprite1->cb.y+sprite1->cb.h)) ||
		(sprite2->cb.x >= (sprite1->cb.x+sprite1->cb.w)) )
	{
		return 0;
	}else{
		return 1;
	}
}

void* jlgr_sprite_getcontext(jl_sprite_t *sprite1) {
	return sprite1->ctx;
}
