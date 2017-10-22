#include "glview.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

GLuint frame_texture = 0;



void frame_texture_init()
{

	glEnable(GL_TEXTURE_2D);
	
	glGenTextures(1, &frame_texture);
	glBindTexture(GL_TEXTURE_2D, frame_texture);



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
	
}

void drawframe_array(int width, int height, unsigned char* array)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, array);

	glBindTexture(GL_TEXTURE_2D, frame_texture);

	glBegin(GL_QUADS);
   	glTexCoord2f(0.0, 1.0); 
	glVertex2f(-1, -1);
    
   	glTexCoord2f(1.0, 1.0); 
	glVertex2f(1, -1);
    
   	glTexCoord2f(1.0, 0.0); 
	glVertex2f(1, 1);
    
   	glTexCoord2f(0.0, 0.0); 
	glVertex2f(-1, 1);
	glEnd();
	
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


 void testcall(void)
{
	printf("HelloWorld!\n");
}



GLFWwindow* window;

void glfw_update(void)
{

	glfwSwapBuffers(window);
	glfwPollEvents();
}


int glview_init(void)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
	window = glfwCreateWindow(1920, 1080, "FB array test", NULL, NULL);
    
	if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
	glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
   

	frame_texture_init();	

}

void glview_close(void)
{
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
