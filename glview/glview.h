#ifndef __GLVIEW_H__
#define __GLVIEW_H__

	void testcall(void);
 	void frame_texture_init(void);
	void drawframe_array(int width, int height, unsigned char* array);
	int glview_init(void);
	void glview_close(void); 
	void glfw_update(void);


#endif /*__GLVIEW_H__*/
