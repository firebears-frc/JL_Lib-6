// SDL OpenGL 2
#if JL_GLTYPE == JL_GLTYPE_SDL_GL2
	#include "SDL_opengl.h"
	#include "lib/glext.h"
#endif

// OpenGL 2
#if JL_GLTYPE == JL_GLTYPE_OPENGL2
	#if JL_PLAT == JL_PLAT_COMPUTER
		#include "lib/glext.h"
	#else
		#error "JL_GLTYPE_OPENGL2 ain't supported by non-pc comps, man!"
	#endif
	#include "lib/glew/glew.h"
	#define JL_GLTYPE_HAS_GLEW
#endif

// SDL OpenGLES 2
#if JL_GLTYPE == JL_GLTYPE_SDL_ES2
	#include "SDL_opengles2.h"
#endif
	
// OpenGLES 2
#if JL_GLTYPE == JL_GLTYPE_OPENES2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif
