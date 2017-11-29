#ifndef __OPENGL_DISPLAY_H__
#define __OPENGL_DISPLAY_H__

#include "qemu/osdep.h"

void gl_display_init(DisplayState* ds, int full_screen);
void gld_draw(void);

#endif /*__OPENGL_DISPLAY_H__*/
