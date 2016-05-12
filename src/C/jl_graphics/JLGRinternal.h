#include "JLgr.h"
#include "jl_pr.h"

typedef struct{
	// Used for all icons on the menubar.
	jl_vo_t* icon;
	// Not Pressed & Pressed & Redraw Functions for 10 icons.
	jlgr_input_fnct inputfn[10];
	jlgr_fnct redrawfn[10];
	// Redraw? - 0 = no, 1 = yes
	m_u8_t redraw;
	// Draw thread Cursor
	m_i8_t draw_cursor;
	// Main thread cursor
	m_i8_t main_cursor;
}jl_menubar_t;

typedef enum{
	JLGR_ID_NULL,
	JLGR_ID_UNKNOWN,
	JLGR_ID_FLIP_IMAGE,
	JLGR_ID_SLOW_IMAGE,
	JLGR_ID_GOOD_IMAGE,
	JLGR_ID_TASK_MAX //how many taskbuttons
}jlgr_id_t;

typedef enum{
	JLGR_COMM_NONE,		/** Does nothing */
	JLGR_COMM_RESIZE,	/** main --> draw: Resize the drawing screen */
	JLGR_COMM_KILL,		/** main --> draw: Close the window*/
	JLGR_COMM_DRAWFIN,	/** main <-- draw: Stop waiting, fool! */
	JLGR_COMM_INIT,		/** main --> draw: Send program's init func. */
	JLGR_COMM_SEND,		/** main --> draw: Send redraw func.'s */
	JLGR_COMM_NOTIFY,	/** main --> draw: Draw a notification */
}jlgr_thread_asdf_t;

typedef struct{
	uint8_t id;
	char string[256];
}jlgr_comm_notify_t;

void jl_sg_add_some_imgs_(jlgr_t* jlgr, u16_t x);
uint32_t _jl_sg_gpix(/*in */ SDL_Surface* surface, int32_t x, int32_t y);
void jl_gl_viewport_screen(jlgr_t* jlgr);
void jl_gl_poly(jlgr_t* jlgr, jl_vo_t* pv, uint8_t vertices, const float *xyzw);
void jl_gl_vect(jlgr_t* jlgr, jl_vo_t* pv, uint8_t vertices, const float *xyzw);
void jl_gl_clrc(jlgr_t* jlgr, jl_vo_t* pv, jl_ccolor_t* cc);
jl_ccolor_t* jl_gl_clrcs(jlgr_t* jlgr, u8_t *rgba, uint32_t vc);
jl_ccolor_t* jl_gl_clrcg(jlgr_t* jlgr, u8_t *rgba, uint32_t vc);
void jl_gl_clrg(jlgr_t* jlgr, jl_vo_t* pv, u8_t *rgba);
void jl_gl_clrs(jlgr_t* jlgr, jl_vo_t* pv, u8_t *rgba);
void jl_gl_txtr(jlgr_t* jlgr, jl_vo_t* pv, u8_t map, u8_t a, u16_t pgid, u16_t pi);
void jl_gl_txtr_(jlgr_t* _jlc, jl_vo_t* pv, u8_t map, u8_t a, u32_t tx);
void jl_gl_transform_pr_(jlgr_t* jlgr, jl_pr_t* pr, float x, float y, float z,
	float xm, float ym, float zm);
void jl_gl_transform_vo_(jlgr_t* jlgr, jl_vo_t* vo, float x, float y, float z,
	float xm, float ym, float zm);
void jl_gl_transform_chr_(jlgr_t* jlgr, jl_vo_t* vo, float x, float y, float z,
	float xm, float ym, float zm);
void jl_gl_draw(jlgr_t* jlgr, jl_vo_t* pv);
void jl_gl_draw_chr(jlgr_t* jlgr, jl_vo_t* pv,
	m_f32_t r, m_f32_t g, m_f32_t b, m_f32_t a);
void jl_gl_draw_pr_(jl_t* jlc, jl_pr_t* pr);
uint8_t jl_gl_pr_isi_(jlgr_t* jlgr, jl_pr_t* pr);
void jl_gl_pr_use(jlgr_t* jlgr, jl_vo_t* pv);
void jl_gl_pr_off(jlgr_t* jlgr);
void jl_gl_pr_scr(jlgr_t* jlgr);

//DL
void _jl_dl_loop(jvct_t* _jlc);
void _jl_sg_loop(jlgr_t* jlgr);
float jl_sg_seconds_past_(jl_t* jlc);
data_t* jl_vi_make_jpeg_(jl_t* jlc,i32_t quality,m_u8_t* pxdata,u16_t w,u16_t h);
m_u8_t* jl_vi_load_(jl_t* jlc, data_t* data, m_u16_t* w, m_u16_t* h);

// Resize function
void jl_dl_resz__(jlgr_t* jlgr, uint16_t x, uint16_t y);
void jl_gl_resz__(jlgr_t* jlgr);
void jl_sg_resz__(jl_t* jlc);
void jlgr_resz(jlgr_t* jlgr, u16_t x, u16_t y);
// init functions.
void jl_dl_init__(jlgr_t* jlgr);
void jl_sg_inita__(jlgr_t* jlgr);
void jl_gl_init__(jlgr_t* jlgr);
void jlgr_init__(jlgr_t* jlgr);
void jl_ct_init__(jlgr_t* jlgr);
void jlgr_fl_init(jlgr_t* jlgr);
void jlgr_menubar_init__(jlgr_t* jlgr);
void jlgr_mouse_init__(jlgr_t* jlgr);
void jlgr_thread_init(jlgr_t* jlgr);
// loop
void jl_ct_loop__(jlgr_t* jlgr);
void jl_dl_loop__(jlgr_t* jlgr);
void _jlgr_loopa(jlgr_t* jlgr);
// kill
void jl_dl_kill__(jlgr_t* jlgr);
void jlgr_thread_kill(jlgr_t* jlgr);
//
void jlgr_thread_send(jlgr_t* jlgr, u8_t id, u16_t x, u16_t y, jl_fnct fn);

//
void jl_wm_updatewh_(jlgr_t* jlgr);
