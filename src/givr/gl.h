// TODO: figure out how to do all of this better.
#include <glad/glad.h>

// Shamelessly copied from GLFW
/* If we are we on Windows, we want a single define for it.
 *  */
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
 #define _WIN32
#endif /* _WIN32 */

/* It is customary to use APIENTRY for OpenGL function pointer declarations on
 *  * all platforms.  Additionally, the Windows OpenGL header needs APIENTRY.
 *   */
#ifndef APIENTRY
 #ifdef _WIN32
  #define APIENTRY __stdcall
 #else
  #define APIENTRY
 #endif
 #define GIVR_APIENTRY_DEFINED
#endif /* APIENTRY */

/* Some Windows OpenGL headers need this.
 *  */
#if !defined(WINGDIAPI) && defined(_WIN32)
 #define WINGDIAPI __declspec(dllimport)
 #define GIVR_WINGDIAPI_DEFINED
#endif /* WINGDIAPI */

/* Some Windows GLU headers need this.
 *  */
#if !defined(CALLBACK) && defined(_WIN32)
 #define CALLBACK __stdcall
 #define GIVR_CALLBACK_DEFINED
#endif /* CALLBACK */

/* Include because most Windows GLU headers need wchar_t and
 *  * the macOS OpenGL header blocks the definition of ptrdiff_t by glext.h.
 *   * Include it unconditionally to avoid surprising side-effects.
 *    */
#include <stddef.h>

/* Include because it is needed by Vulkan and related functions.
 *  * Include it unconditionally to avoid surprising side-effects.
 *   */
#include <stdint.h>

/* Include the chosen OpenGL or OpenGL ES headers.
 *  */
#if defined(GIVR_INCLUDE_ES1)

 #include <GLES/gl.h>
 #if defined(GIVR_INCLUDE_GLEXT)
  #include <GLES/glext.h>
 #endif

#elif defined(GIVR_INCLUDE_ES2)

 #include <GLES2/gl2.h>
 #if defined(GIVR_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GIVR_INCLUDE_ES3)

 #include <GLES3/gl3.h>
 #if defined(GIVR_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GIVR_INCLUDE_ES31)

 #include <GLES3/gl31.h>
 #if defined(GIVR_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GIVR_INCLUDE_ES32)

 #include <GLES3/gl32.h>
 #if defined(GIVR_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GIVR_INCLUDE_GLCOREARB)

 #if defined(__APPLE__)

  #include <OpenGL/gl3.h>
  #if defined(GIVR_INCLUDE_GLEXT)
   #include <OpenGL/gl3ext.h>
  #endif /*GIVR_INCLUDE_GLEXT*/

 #else /*__APPLE__*/

  #include <GL/glcorearb.h>

 #endif /*__APPLE__*/

#elif !defined(GIVR_INCLUDE_NONE)

 #if defined(__APPLE__)

  #if !defined(GIVR_INCLUDE_GLEXT)
   #define GL_GLEXT_LEGACY
  #endif
  #include <OpenGL/gl.h>
  #if defined(GIVR_INCLUDE_GLU)
   #include <OpenGL/glu.h>
  #endif

 #else /*__APPLE__*/

  #include <GL/gl.h>
  #if defined(GIVR_INCLUDE_GLEXT)
   #include <GL/glext.h>
  #endif
  #if defined(GIVR_INCLUDE_GLU)
   #include <GL/glu.h>
  #endif

 #endif /*__APPLE__*/

#endif /* OpenGL and OpenGL ES headers */

#if defined(GIVR_INCLUDE_VULKAN)
  #include <vulkan/vulkan.h>
#endif /* Vulkan header */

