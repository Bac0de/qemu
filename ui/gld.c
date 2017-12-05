#include <stdlib.h>

#include "ui/gld.h"
#include "qemu/osdep.h"
#include "ui/console.h"
#include "hw/display/qxl.h"
#include <GLFW/glfw3.h>

#define GL_FRAMERATE 1

// xhost si:localuser:libvirt-qemu
// xhost si:localuser:root

static DisplayChangeListener *dcl;
static GLuint frame_texture = 0;
static GLFWwindow *window;

static double frame_rate_last = 0;
static int frame_rate_frames = 0;
static int display_refresh_rate = 0;

static void gld_update(DisplayChangeListener * dcl, int x, int y, int w, int h)
{}

static void gld_show_frame_rate(void)
{
    double currentTile = glfwGetTime();

    frame_rate_frames++;
    if (currentTile - frame_rate_last >= 1.0) {
        printf("[GLD] %d fps  ,  %f ms/frame\n", frame_rate_frames,
               1000.0 / ((double)frame_rate_frames));
        frame_rate_frames = 0;
        frame_rate_last += 1.0;
    }

}

void gld_draw(void)
{
    if (GL_FRAMERATE)
        gld_show_frame_rate();

    glBindTexture(GL_TEXTURE_2D, frame_texture);

    void *vram = get_vram_ptr();
    glViewport(0, 0, 1920, 1080);
    //printf("tex %d vram_ptr %p \n",frame_texture ,vram);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1920, 1080,
            GL_BGRA, GL_UNSIGNED_BYTE, vram);

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

    glfwSwapBuffers(window);
}

static void gld_refresh(DisplayChangeListener * dcl)
{
    graphic_hw_update(NULL);

    gld_draw();
    dcl->update_interval = 10;
}

static const DisplayChangeListenerOps dcl_ops = {
    .dpy_name = "gld",
    .dpy_gfx_update = gld_update,
    .dpy_refresh = gld_refresh
};

static void glfw_error(int error, const char *description)
{
    perror("GLD failed");
}

static void frame_texture_init(void)
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

static void gld_create_window(void)
{
    // Setup monitor for Full-screen OpenGL
    char* mon_char = getenv("QEMU_MONITOR");
    int mon = 0;

    if (mon_char)
        sscanf(mon_char, "%d", &mon);

    putenv("DISPLAY=:0");
    putenv("XAUTHORITY=/home/batu/.Xauthority");
    fprintf(stderr, "DISPLAY=%s\n", getenv("DISPLAY"));

    glfwSetErrorCallback(glfw_error);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    int count = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&count);

    window = glfwCreateWindow(1920, 1080, "GLD", monitors[mon], NULL);
    glfwWindowHint(GLFW_DECORATED, 0);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    const GLFWvidmode *mode = glfwGetVideoMode(monitors[mon]);
    display_refresh_rate = mode->refreshRate;

    printf("r %d \n", display_refresh_rate);
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);
    frame_texture_init();
}

void gl_display_init(DisplayState * ds, int full_screen)
{
    gld_create_window();
    dcl = g_new0(DisplayChangeListener, 1);
    dcl->ops = &dcl_ops;
    register_displaychangelistener(dcl);

}
