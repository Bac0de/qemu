#include "glview.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

GLuint frame_texture = 0;

void frame_texture_init(void)
{
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &frame_texture);
    glBindTexture(GL_TEXTURE_2D, frame_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080, 0, GL_BGRA,
             GL_UNSIGNED_BYTE, 0);
}

double lastTime;
int nbFrames;

void drawframe_array(int width, int height, unsigned char *array)
{
    double currentTile = glfwGetTime();
    nbFrames++;
    if (currentTile - lastTime >= 1.0) {
        printf("[GLVIEW] %d fps  ,  %f ms/frame\n", nbFrames,
               1000.0 / ((double)nbFrames));
        nbFrames = 0;
        lastTime += 1.0;
    }

    glBindTexture(GL_TEXTURE_2D, frame_texture);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA,
            GL_UNSIGNED_BYTE, array);

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

static void error_callback(int error, const char *description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow * window, int key, int scancode, int action,
             int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow *window;

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

    // Hardcoded to 1080p for now
    window = glfwCreateWindow(1920, 1080, "FB array test", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    frame_texture_init();

    return 0;
}

void glview_close(void)
{
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
