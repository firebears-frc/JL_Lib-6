/*
 * JL_Lib
 * Copyright (c) 2015 Jeron A. Lau 
*/
/** \file
 * JLGRthread.c
 *	This file handles a separate thread for drawing graphics.
**/
#include "JLGRinternal.h"

static void jlgr_thread_resize(jlgr_t* jlgr, u16_t w, u16_t h) {
	jl_print(jlgr->jl, "Resizing to %dx%d....", w, h);
	// Reset aspect ratio stuff.
	jl_dl_resz__(jlgr, w, h);
	// Update the actual window.
	jl_gl_resz__(jlgr);
	// Update the size of the background.
	jl_sg_resz__(jlgr->jl);
	// Taskbar resize.
	if(jlgr->menubar.menubar) {
		jl_menubar_t *ctx = jlgr->menubar.menubar->ctx;

		//Menu Bar
		ctx->redraw = 2;
		jlgr_sprite_resize(jlgr, jlgr->menubar.menubar, NULL);
	}
	// Mouse resize
	if(jlgr->mouse) jlgr_sprite_resize(jlgr, jlgr->mouse, NULL );
	// Program's Resize
	jl_fnct resize_ = jlgr->draw.redraw.resize;
	resize_(jlgr->jl);
}

static void jlgr_thread_event(jl_t* jl, void* data) {
	jlgr_t* jlgr = jl->jlgr;
	jlgr_thread_packet_t* packet = data;

	switch(packet->id) {
		case JLGR_COMM_RESIZE: {
			jl_wm_updatewh_(jlgr);
			if(packet->x == 0) packet->x = jlgr_wm_getw(jlgr);
			if(packet->y == 0) packet->y = jlgr_wm_geth(jlgr);
			jlgr_thread_resize(jlgr, packet->x, packet->y);
			break;
		} case JLGR_COMM_KILL: {
			jl_print(jl, "Thread exiting....");
			jlgr->draw.rtn = 1;
			break;
		} case JLGR_COMM_SEND: {
			if(packet->x==0) jlgr->draw.redraw.single = packet->fn;
			if(packet->x==1) jlgr->draw.redraw.upper = packet->fn;
			if(packet->x==2) jlgr->draw.redraw.lower = packet->fn;
			if(packet->x==3) {
				jlgr->draw.fn = packet->fn;
				packet->fn(jl);
			}
			break;
		} case JLGR_COMM_NOTIFY: {
			jlgr_comm_notify_t* packeta = data;
			jl_mem_copyto(packeta->string,
				jlgr->gr.notification.message, 255);
			jlgr->gr.notification.timeTilVanish = 8.5;
			break;
		} default: {
			break;
		}
	}
}

static void jlgr_thread_resize_event(jl_t* jl, void* data) {
	jlgr_t* jlgr = jl->jlgr;
	jlgr_thread_packet_t* packet = data;

	switch(packet->id) {
		case JLGR_COMM_RESIZE: {
			uint16_t w = packet->x;
			uint16_t h = packet->y;
			jl_print(jlgr->jl, "Resizing to %dx%d....", w, h);
			// Reset aspect ratio stuff.
			jl_dl_resz__(jlgr, w, h);
			// Update the actual window.
			jl_gl_resz__(jlgr);
			break;
		} case JLGR_COMM_INIT: {
			jlgr->draw.fn = packet->fn;
			jlgr->draw.rtn = 2;
			break;
		} default: {
			break;
		}
	}
}

static u8_t jlgr_thread_draw_event__(jl_t* jl) {
	jlgr_t* jlgr = jl->jlgr;
	jlgr->draw.rtn = 0;

	jl_thread_comm_recv(jl, jlgr->comm2draw, jlgr_thread_event);
	return jlgr->draw.rtn;
}

static void jlgr_thread_draw_init__(jl_t* jl) {
	jlgr_t* jlgr = jl->jlgr;
	jlgr_thread_packet_t packet = { JLGR_COMM_DRAWFIN, 0, 0 };

	// Initialize subsystems
	JL_PRINT_DEBUG(jl, "Creating the window....");
	jl_dl_init__(jlgr);
	JL_PRINT_DEBUG(jl, "Resize Adjust");
	jlgr->draw.rtn = 0;
	while(jlgr->draw.rtn != 2) {
		jl_thread_comm_recv(jl, jlgr->comm2draw,
			jlgr_thread_resize_event);
	}
	JL_PRINT_DEBUG(jl, "Loading default graphics from package....");
	jl_sg_inita__(jlgr);
	JL_PRINT_DEBUG(jl, "Setting up OpenGL....");
	jl_gl_init__(jlgr);
	JL_PRINT_DEBUG(jl, "Load graphics....");
	jlgr_init__(jlgr);
	JL_PRINT_DEBUG(jl, "Creating Taskbar sprite....");
	jlgr_menubar_init__(jlgr);
	JL_PRINT_DEBUG(jl, "Creating Mouse sprite....");
	jlgr_mouse_init__(jlgr);
	JL_PRINT_DEBUG(jl, "User's Init....");
	jlgr->draw.fn(jl);
	jlgr_thread_resize(jlgr, jlgr_wm_getw(jlgr), jlgr_wm_geth(jlgr));
	jlgr_wm_setwindowname(jlgr, jl->name);
	JL_PRINT_DEBUG(jl, "Sending finish packet....");
	// Tell main thread to stop waiting.
	jl_thread_comm_send(jl, jlgr->comm2main, &packet);
}

void jlgr_thread_send(jlgr_t* jlgr, u8_t id, u16_t x, u16_t y, jl_fnct fn) {
	jlgr_thread_packet_t packet = { id, x, y, fn };

	// Send resize packet.
	jl_thread_comm_send(jlgr->jl, jlgr->comm2draw, &packet);
}

int jlgr_thread_draw(void* data) {
	jl_t* jl = data;
	jlgr_t* jlgr = jl->jlgr;

	// Initialize subsystems
	jl_thread_mutex_use(jl, jlgr->mutex, jlgr_thread_draw_init__);
	// Redraw loop
	while(1) {
		// Check for events.
		if(jlgr_thread_draw_event__(jl)) break;
		// Deselect any pre-renderer.
		jlgr->gl.cp = NULL;
		//Redraw screen.
		_jl_sg_loop(jlgr);
		//Update Screen.
		jl_dl_loop__(jlgr);
	}
	jl_dl_kill__(jlgr); // Kill window
	jlgr_pr_old(jlgr, jlgr->sg.bg.up);
	jlgr_pr_old(jlgr, jlgr->sg.bg.dn);
	return 0;
}

void jlgr_thread_init(jlgr_t* jlgr) {
	jlgr->thread = jl_thread_new(jlgr->jl, "JL_Lib/Graphics",
		jlgr_thread_draw);
}

void jlgr_thread_kill(jlgr_t* jlgr) {
	jl_thread_old(jlgr->jl, jlgr->thread);
}
