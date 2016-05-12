/*
 * (c) Jeron A. Lau
 * library for making input compatible between different
 * devices.
*/

typedef enum{
	JLGR_INPUT_PRESS_ISNT, // User is not currently using the control
	JLGR_INPUT_PRESS_JUST, // User just started using the control
	JLGR_INPUT_PRESS_HELD, // User is using the control
	JLGR_INPUT_PRESS_STOP, // User just released the control.
}JLGR_INPUT_PRESS_T;

/* 
 * these are control definitions 
*/
// Game systems
	//Normal buttons
	#define JL_CT_GAME_BTNL 0 //NYI: L button: Up/Left
	#define JL_CT_GAME_BTNR 1 //NYI: R button: Down/Right
	#define JL_CT_GAME_BTNA 2 //NYI: A button: Right
	#define JL_CT_GAME_BTNB 3 //NYI: B button: Down
	#define JL_CT_GAME_BTNX 4 //NYI: X button: Up
	#define JL_CT_GAME_BTNY 5 //NYI: Y button: Left
	#define JL_CT_GAME_STRT 6 //NYI: Start button
	//XY
	#define JL_CT_GAME_STYL 7 //NYI: Stylus
	#define JL_CT_GAME_CPAD 8 //NYI: Circle Pad -or- left joystick (WiiU)
	#define JL_CT_GAME_JSTK 9 //NYI: Cirle Pad + X -or- right joystick (WiiU)
	#define JL_CT_GAME_UDLR 10//NYI: LR buttons (Y if B pressed, X if not)
	#define JL_CT_GAME_DPAD 11//NYI: D-Pad
	#define JL_CT_GAME_ABXY 12//NYI: ABXY buttons
	//Stylus
	#define JL_CT_GAME_SSUP 13 //NYI: Up
	#define JL_CT_GAME_SSDN 14 //NYI: Down
	#define JL_CT_GAME_SSRT 15 //NYI: Right
	#define JL_CT_GAME_SSLT 16 //NYI: Left
	#define JL_CT_GAME_SSUR 17 //NYI: Up Right
	#define JL_CT_GAME_SSDR 18 //NYI: Down Right
	#define JL_CT_GAME_SSUL 19 //NYI: Up Left
	#define JL_CT_GAME_SSDL 20 //NYI: Down Left
	//Cirle Pad -or- left joystick(WiiU)
	#define JL_CT_GAME_CPUP 21 //NYI: Up
	#define JL_CT_GAME_CPDN 22 //NYI: Down
	#define JL_CT_GAME_CPRT 23 //NYI: Right
	#define JL_CT_GAME_CPLT 24 //NYI: Left
	#define JL_CT_GAME_CPUR 25 //NYI: Up Right
	#define JL_CT_GAME_CPDR 26 //NYI: Down Right
	#define JL_CT_GAME_CPUL 27 //NYI: Up Left
	#define JL_CT_GAME_CPDL 28 //NYI: Down Left
	//Cirle Pad + X -or- right joystick (WiiU)
	#define JL_CT_GAME_JSUP 29 //NYI: Up
	#define JL_CT_GAME_JSDN 30 //NYI: Down
	#define JL_CT_GAME_JSRT 31 //NYI: Right
	#define JL_CT_GAME_JSLT 32 //NYI: Left
	#define JL_CT_GAME_JSUR 33 //NYI: Up Right
	#define JL_CT_GAME_JSDR 34 //NYI: Down Right
	#define JL_CT_GAME_JSUL 35 //NYI: Up Left
	#define JL_CT_GAME_JSDL 36 //NYI: Down Left
	//D-Pad
	#define JL_CT_GAME_DPUP 37 //NYI: Up
	#define JL_CT_GAME_DPDN 38 //NYI: Down
	#define JL_CT_GAME_DPRT 39 //NYI: Right
	#define JL_CT_GAME_DPLT 40 //NYI: Left
	#define JL_CT_GAME_DPUR 41 //NYI: Up Right
	#define JL_CT_GAME_DPDR 42 //NYI: Down Right
	#define JL_CT_GAME_DPUL 43 //NYI: Up Left
	#define JL_CT_GAME_DPDL 44 //NYI: Down Left
	#define JL_CT_GAME_MAXX 45
//computer
	//normal buttons
	#define JL_CT_COMP_SCRU 0 //NYI: Scroll Up
	#define JL_CT_COMP_SCRD 1 //NYI: Scroll Down
	/* Scroll Right(Mac only) -or- Ctrl Key + Down -or- page down */
	#define JL_CT_COMP_SCRR 2 //NYI
	/* Scroll Left(Mac only) -or- Ctrl Key + Up -or- page up */
	#define JL_CT_COMP_SCRL 3 //NYI
	#define JL_CT_COMP_KEYW 4 //W Key
	#define JL_CT_COMP_KEYA 5 //A Key
	#define JL_CT_COMP_KEYS 6 //S Key
	#define JL_CT_COMP_KEYD 7 //D Key
	#define JL_CT_COMP_ARUP 8 //Up Arrow Key
	#define JL_CT_COMP_ARDN 9 //Down Arrow Key
	#define JL_CT_COMP_ARRT 10//Right Arrow Key
	#define JL_CT_COMP_ARLT 11//Left Arrow Key
	#define JL_CT_COMP_NUM0 12//NYI: Numpad 0 key
	#define JL_CT_COMP_NUM1 13//NYI: Numpad 1 key
	#define JL_CT_COMP_NUM2 14//NYI: Numpad 2 key
	#define JL_CT_COMP_NUM3 15//NYI: Numpad 3 key
	#define JL_CT_COMP_NUM4 16//NYI: Numpad 4 key
	#define JL_CT_COMP_NUM5 17//NYI: Numpad 5 key
	#define JL_CT_COMP_NUM6 18//NYI: Numpad 6 key
	#define JL_CT_COMP_NUM7 19//NYI: Numpad 7 key
	#define JL_CT_COMP_NUM8 20//NYI: Numpad 8 key
	#define JL_CT_COMP_NUM9 21//NYI: Numpad 9 key
	#define JL_CT_COMP_KEYJ 22//NYI: J key/LEFT
	#define JL_CT_COMP_KEYK 23//NYI: K key/DOWN
	#define JL_CT_COMP_KEYL 24//NYI: L key/RIGHT
	#define JL_CT_COMP_KEYI 25//NYI: I key/UP
	#define JL_CT_COMP_SPAC 26//NYI: Space key
	#define JL_CT_COMP_RETN 27//Return key
	#define JL_CT_COMP_BACK 28//NYI: Backspace key
	#define JL_CT_COMP_KTAB 29//NYI: Tab key
	#define JL_CT_COMP_CLLT 30//NYI: Left Click
	#define JL_CT_COMP_CLRT 31//NYI: Right Click(PC only) -or- Ctr-Click
	#define JL_CT_COMP_CLCR 32//NYI: Middle Click
	#define JL_CT_COMP_MSVW 33//NYI: Mouse Move [ 3D: looking around ]
	#define JL_CT_COMP_MENU 34//Menu Key
	#define JL_CT_COMP_MAXX 35
//android
	//normal buttons
	#define JL_CT_ANDR_BTNA 0 //NYI: Virtual button A
	#define JL_CT_ANDR_BTNB 1 //NYI: Virtual button B
	#define JL_CT_ANDR_BTNX 2 //NYI: Virtual button X
	#define JL_CT_ANDR_BTNY 3 //NYI: Virtual button Y
	#define JL_CT_ANDR_LDUP 4 //NYI: Virtual Left D-Pad Up
	#define JL_CT_ANDR_LDDN 5 //NYI: Virtual Left D-Pad Down
	#define JL_CT_ANDR_LDRT 6 //NYI: Virtual Left D-Pad Right
	#define JL_CT_ANDR_LDLT 7 //NYI: Virtual Left D-Pad Left
	#define JL_CT_ANDR_RDUP 8 //NYI: Virtual Right D-Pad Up
	#define JL_CT_ANDR_RDDN 9 //NYI: Virtual Right D-Pad Down
	#define JL_CT_ANDR_RDRT 10 //NYI: Virtual Right D-Pad Right
	#define JL_CT_ANDR_RDLT 11 //NYI: Virtual Right D-Pad Left
	#define JL_CT_ANDR_LCUP 12 //NYI: Virtual Left Circle Pad Up
	#define JL_CT_ANDR_LCDN 13 //NYI: Virtual Left Circle Pad Down
	#define JL_CT_ANDR_LCRT 14 //NYI: Virtual Left Circle Pad Right
	#define JL_CT_ANDR_LCLT 15 //NYI: Virtual Left Circle Pad Left
	#define JL_CT_ANDR_LCUR 16 //NYI: Virtual Left Circle Pad Up Right
	#define JL_CT_ANDR_LCDR 17 //NYI: Virtual Left Circle Pad Down Right
	#define JL_CT_ANDR_LCUL 18 //NYI: Virtual Left Circle Pad Up Left
	#define JL_CT_ANDR_LCDL 19 //NYI: Virtual Left Circle Pad Down Left
	#define JL_CT_ANDR_RCUP 20 //NYI: Virtual Right Circle Pad Up
	#define JL_CT_ANDR_RCDN 21 //NYI: Virtual Right Circle Pad Down
	#define JL_CT_ANDR_RCRT 22 //NYI: Virtual Right Circle Pad Right
	#define JL_CT_ANDR_RCLT 23 //NYI: Virtual Right Circle Pad Left
	#define JL_CT_ANDR_RCUR 24 //NYI: Virtual Right Circle Pad Up Right
	#define JL_CT_ANDR_RCDR 25 //NYI: Virtual Right Circle Pad Down Right
	#define JL_CT_ANDR_RCUL 26 //NYI: Virtual Right Circle Pad Up Left
	#define JL_CT_ANDR_RCDL 27 //NYI: Virtual Right Circle Pad Down Left
	#define JL_CT_ANDR_DRUP 28 //NYI: Drag Up
	#define JL_CT_ANDR_DRDN 29 //NYI: Drag Down
	#define JL_CT_ANDR_DRRT 30 //NYI: Drag Right
	#define JL_CT_ANDR_DRLT 31 //NYI: Drag Left
	#define JL_CT_ANDR_DRUR 32 //NYI: Drag Up Right
	#define JL_CT_ANDR_DRDR 33 //NYI: Drag Down Right
	#define JL_CT_ANDR_DRUL 34 //NYI: Drag Up Left
	#define JL_CT_ANDR_DRDL 35 //NYI: Drag Down Left
	#define JL_CT_ANDR_TCUP 36 //NYI: Touch Up
	#define JL_CT_ANDR_TCDN 37 //NYI: Touch Down
	#define JL_CT_ANDR_TCRT 38 //NYI: Touch Right
	#define JL_CT_ANDR_TCLT 39 //NYI: Touch Left
	#define JL_CT_ANDR_TCUR 40 //NYI: Touch Up Right
	#define JL_CT_ANDR_TCDR 41 //NYI: Touch Down Right
	#define JL_CT_ANDR_TCUL 42 //NYI: Touch Up Left
	#define JL_CT_ANDR_TCDL 43 //NYI: Touch Down Left
	#define JL_CT_ANDR_TFUP 44 //Touch Far Up
	#define JL_CT_ANDR_TFDN 45 //Touch Far Down
	#define JL_CT_ANDR_TFRT 46 //Touch Far Right
	#define JL_CT_ANDR_TFLT 47 //Touch Far Left
	#define JL_CT_ANDR_TFUR 48 //NYI: Touch Far Up Right
	#define JL_CT_ANDR_TFDR 49 //NYI: Touch Far Down Right
	#define JL_CT_ANDR_TFUL 50 //NYI: Touch Far Up Left
	#define JL_CT_ANDR_TFDL 51 //NYI: Touch Far Down Left
	#define JL_CT_ANDR_TNUP 52 //Touch Close Up
	#define JL_CT_ANDR_TNDN 53 //Touch Close Down
	#define JL_CT_ANDR_TNRT 54 //Touch Close Right
	#define JL_CT_ANDR_TNLT 55 //Touch Close Left
	#define JL_CT_ANDR_TNUR 56 //NYI: Touch Close Up Right
	#define JL_CT_ANDR_TNDR 57 //NYI: Touch Close Down Right
	#define JL_CT_ANDR_TNUL 58 //NYI: Touch Close Up Left
	#define JL_CT_ANDR_TNDL 59 //NYI: Touch Close Down Left
	#define JL_CT_ANDR_TCCR 60 //Touch Center
	#define JL_CT_ANDR_TOUC 61 //Touch(X,Y) 255-0, 382-0
	#define JL_CT_ANDR_TCJS 62 //NYI: Touch(X,Y) 255-0, 255-0
	#define JL_CT_ANDR_MENU 63 //Menu Key
	#define JL_CT_ANDR_MAXX 64

#if JL_PLAT == JL_PLAT_COMPUTER
	#define JL_CT_ALLP(nintendo, computer, android) computer
#elif JL_PLAT == JL_PLAT_PHONE
	#define JL_CT_ALLP(nintendo, computer, android) android
#else //3DS/WiiU
	#define JL_CT_ALLP(nintendo, computer, android) nintendo
#endif

#define JL_CT_MAXX JL_CT_ALLP(\
		JL_CT_GAME_MAXX,\
		JL_CT_COMP_MAXX,\
		JL_CT_ANDR_MAXX)
		
		
#define JL_CT_PRESS JL_CT_ALLP(JL_CT_GAME_STYL,JL_CT_COMP_CLLT,JL_CT_ANDR_TOUC)
#define JL_CT_MAINUP JL_CT_ALLP(JL_CT_GAME_CPUP,JL_CT_COMP_ARUP,JL_CT_ANDR_TFUP)
#define JL_CT_MAINDN JL_CT_ALLP(JL_CT_GAME_CPDN,JL_CT_COMP_ARDN,JL_CT_ANDR_TFDN)
#define JL_CT_MAINRT JL_CT_ALLP(JL_CT_GAME_CPRT,JL_CT_COMP_ARRT,JL_CT_ANDR_TFRT)
#define JL_CT_MAINLT JL_CT_ALLP(JL_CT_GAME_CPLT,JL_CT_COMP_ARLT,JL_CT_ANDR_TFLT)
#define JL_CT_SELECT JL_CT_ALLP(JL_CT_GAME_BTNA,JL_CT_COMP_RETN,JL_CT_ANDR_TCCR)
