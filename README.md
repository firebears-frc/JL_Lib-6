*************************
*      INTRODUCTION     *
*************************

	JL_LIB is a powerful & easy to use library that uses OpenGL/OpenGLES
with SDL2.  JL_LIB takes care of any compatibility issues there might be between
opengl and opengles and sdl for all supported platforms ( any code written in
this library will run on any supported platform, no extra code writing needed ).

Supported Platforms:
	Linux ( including Raspberry Pi )
	Android

Need help porting:
	Windows PC
	Apple ( Mac )
	IOS
	Others ?

*************************
*        INSTALL        *
*************************

	1. Set up build directories & Download + Build dependencies:
		- For Android compatibility,
			make init-all --silent
		- Without,
			make init-most --silent
	2. Compile Library:
		- Build for your computer,
			make build-library --silent
		- Cross compile for ...
			( TODO: TBD )
	3. Set Environment Variables
		- In ~/.bashrc
			export JLL_PATH=/home/$(YOUR_NAME_HERE)/bin
			export JLL_HOME=/home/$(YOUR_NAME_HERE)/$(LIB-DIR)/JL_Lib-6

*************************
*         CLEAN         *
*************************

	 ** use only when you don't want to use current builds anymore! **
	1. How to clean builds of JL-Lib.
		- Clean all build folders:
			make clean-all --silent
		- Clean all dependencies:
			make clean-deps --silent
		- Clean all of the build folder:
			make clean-build --silent

*************************
*      HOW TO USE       *
*************************

	

*************************
*      CODE STYLE       *
*************************
	- Indentation is with tabs [ standard length - 8 ]
	- No more than 3 tabs to start a line, at that point a separate function
		is needed.
	- Every function starts with a shortenned project name: JL Lib = "jl"
		Then is followed by an underscore and file name without "JL":
		"jl_mem"
		After that there's another underscore and then the description.
		"jl_mem_clr"
		We'll call "jl_mem" the <NAMESPACE>.
	- Functions that are not available to user end in "__"
	- Functions that are end in a-z, 1-9
	- Functions available to the library user have nothing appended to them.
	- Whenever a function is a parameter, it doesn't have a <NAMESPACE>.
		Instead it has a "par__" before the function parameter name.
	- Each file represents a subsystem.  JLmem.c represents memory management.
	- General layout of a file:
	
		/*
		 * JL_lib
		 * Copyright (c) 2015 Jeron A. Lau 
		*/
		/** \file
		 * examplefile.c
		 *	example description
		**/
		
			// Includes.

			// Prototypes & Constants.

			/** @cond **/

			// Where all the static functions go.

			/** @endcond **/
		
			// Where all the global functions go - the ones that
			// The person who programs has access to.
		
			/** @cond **/
		
			// Where all the Exported To Other Module functions go
			// The ones the programmer doesn't have access to but
			// can't be static.
		
			/** @endcond **/

			/***   #End of File   ***/
	- Function naming:
		- Whenever there's an object being modified by functions use:
			new	create the object.
			old	delete the object.
			set	set the object's data.
			get	get the object's data
			use	start using the object.
			off	stop using the object.
			rsz	resize or redraw the object.
			rdr	redraw the object
			drw	draw the object on the screen.
			rnl	run the object's loop.
			ist	get whether it's initialized/active.
		- Subsystems Functions:
			init	run when subsystem is initialized.
			loop	run every frame.
			resz	run when window is resized.
			kill	run when subsystem is ready to be shutdown.
