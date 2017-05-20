#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include "config.h"
#include "vr.h" // vr_update_viewport

static GLFWwindow *window;
#if !USE_EM_FULLSCREEN
static int window_width;
static int window_height;
static int window_xpos;
static int window_ypos;
static GLFWmonitor *fullscreen_monitor;
static int fullscreen_width;
static int fullscreen_height;
#endif

extern int get_scale_factor(GLFWwindow *window);

#ifdef __EMSCRIPTEN__
static EM_BOOL on_canvassize_changed(int eventType, const void *reserved, void *userData) {
    // Resize window to match canvas size (as browser is resized).
    int w = 0;
    int h = 0;
    int isFullscreen = 0;
    emscripten_get_canvas_size(&w, &h, &isFullscreen);

    double cssW, cssH;
    emscripten_get_element_css_size(0, &cssW, &cssH);

    glfwSetWindowSize(window, w, h);

    int fb_w, fb_h;
    glfwGetFramebufferSize(window, &fb_w, &fb_h);

    // http://www.glfw.org/docs/latest/window.html#window_size
    // "Note
    // Do not pass the window size to glViewport or other pixel-based OpenGL calls.
    // The window size is in screen coordinates, not pixels. Use the framebuffer
    // size, which is in pixels, for pixel-based calls."
    int w_w, w_h;
    glfwGetWindowSize(window, &w_w, &w_h);
    //printf("Canvas resized: WebGL RTT size: %dx%d, framebuffer: %dx%d, window: %dx%d, canvas CSS size: %02gx%02g\n", w, h, fb_w, fb_h, w_w, w_h, cssW, cssH);


    return EM_FALSE;
}

// Emscripten's "soft fullscreen" = maximizes the canvas in the browser client area, wanted to toggle soft/hard fullscreen
static void maximize_canvas() {
    EmscriptenFullscreenStrategy strategy = {
        .scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH,
        .canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF,
        .filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT, // or EMSCRIPTEN_FULLSCREEN_FILTERING_NEAREST
        .canvasResizedCallback = on_canvassize_changed,
        .canvasResizedCallbackUserData = NULL
    };

    emscripten_enter_soft_fullscreen("#canvas", &strategy);

    on_canvassize_changed(0, NULL, NULL);
}
#endif


void on_window_size(GLFWwindow* window, int width, int height) {
#ifdef __EMSCRIPTEN__
    static int wasFullscreen = 0;

    int isInFullscreen = EM_ASM_INT_V(return !!(document.fullscreenElement || document.mozFullScreenElement || document.webkitFullscreenElement || document.msFullscreenElement));
    if (isInFullscreen && !wasFullscreen) {
        printf("Successfully transitioned to fullscreen mode!\n");
        wasFullscreen = isInFullscreen;

        // Set canvas size to full screen, all the pixels
        EM_ASM("Browser.setCanvasSize(screen.width, screen.height)");
    }

    if (wasFullscreen && !isInFullscreen) {
        wasFullscreen = isInFullscreen;
        maximize_canvas();
    }
#endif

    vr_update_viewport(width, height);
}



#ifdef USE_EM_FULLSCREEN
static EM_BOOL fullscreen_change_callback(int eventType, const EmscriptenFullscreenChangeEvent *event, void *userData) {
    printf("fullscreen_change_callback, isFullscreen=%d\n", event->isFullscreen);

    if (!event->isFullscreen) {
        // Go back to windowed mode with full-sized <canvas>, when user escapes out (instead of F11)
        maximize_canvas();
    }

    return EM_TRUE;
}

static bool is_fullscreen() {
    EmscriptenFullscreenChangeEvent fsce;

    emscripten_get_fullscreen_status(&fsce);
    return fsce.isFullscreen != 0;
}

static void fullscreen_exit() {
    printf("Exiting fullscreen...\n");
    emscripten_exit_fullscreen();

    printf("Maximizing to canvas...\n");
    maximize_canvas();
}

static void fullscreen_enter() {
    emscripten_exit_soft_fullscreen();

    // Workaround https://github.com/kripken/emscripten/issues/5124#issuecomment-292849872
    // Force JSEvents.canPerformEventHandlerRequests() in library_html5.js to be true
    // For some reason it is not set even though we are in an event handler and it works
    EM_ASM(JSEvents.inEventHandler = true);
    EM_ASM(JSEvents.currentEventHandler = {allowsDeferredCalls:true});

    // Enter fullscreen
    /* this returns 1=EMSCRIPTEN_RESULT_DEFERRED if EM_TRUE is given to defer
     * or -2=EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED if EM_FALSE
     * but the EM_ASM() JS works immediately?
     */
    EmscriptenFullscreenStrategy strategy = {
        .scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH,
        .canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF,
        .filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT,
        .canvasResizedCallback = on_canvassize_changed,
        .canvasResizedCallbackUserData = NULL
    };
    EMSCRIPTEN_RESULT ret = emscripten_request_fullscreen_strategy(NULL, EM_FALSE, &strategy);
    printf("emscripten_request_fullscreen_strategy = %d\n", ret);
    //EM_ASM(Module.requestFullscreen(1, 1));
}

#else
static bool is_fullscreen() {
    return glfwGetWindowMonitor(window) != NULL;
}

static void fullscreen_exit() {
    glfwSetWindowMonitor(window, NULL, window_xpos, window_ypos, window_width, window_height, GLFW_DONT_CARE);
}

static void fullscreen_enter() {
    glfwGetWindowPos(window, &window_xpos, &window_ypos);
    glfwGetWindowSize(window, &window_width, &window_height);
    glfwSetWindowMonitor(window, fullscreen_monitor, 0, 0, fullscreen_width, fullscreen_height, GLFW_DONT_CARE);
}

#endif

void init_fullscreen_monitor_dimensions(GLFWwindow *w) {
    window = w;

    glfwSetWindowSizeCallback(window, on_window_size);

#ifndef USE_EM_FULLSCREEN
    int mode_count;
    fullscreen_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(fullscreen_monitor);
    if (mode->width == 1920 && mode->height == 1080 && mode->refreshRate == 75) {
        fullscreen_width = mode->width;
        fullscreen_height= mode->height;
        // This is the native mode for Oculus Rift DK2, even though it isn't the
        // "highest" (last) resolution (which is 1600x2560), it is the native and
        // what we want and need for correct 90 degree rotation.
        printf("Using current video mode for fullscreen: %d x %d @ %d Hz\n", mode->width, mode->height, mode->refreshRate);
    } else {
        const GLFWvidmode *modes = glfwGetVideoModes(fullscreen_monitor, &mode_count);
        fullscreen_width = modes[mode_count - 1].width;
        fullscreen_height= modes[mode_count - 1].height;
    }

    GLFWwindow *test_window = glfwCreateWindow(
        fullscreen_width, fullscreen_height, "Craft", NULL, NULL);
    int scale = get_scale_factor(test_window);
    glfwDestroyWindow(test_window);
    fullscreen_width /= scale;
    fullscreen_height /= scale;
#else
    maximize_canvas();
    emscripten_set_fullscreenchange_callback(NULL, NULL, EM_TRUE, fullscreen_change_callback);
#endif
}


void fullscreen_toggle() {
    if (is_fullscreen()) {
        fullscreen_exit();
    } else {
        fullscreen_enter();
    }
}

