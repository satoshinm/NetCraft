#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "mining.h"
#include "touch.h"

bool touch_forward = false;
bool touch_jump = false;

long touch_active = 0;
long touch_clientX = 0;
long touch_clientY = 0;

static GLFWwindow *window = NULL;

#ifndef __EMSCRIPTEN__
void init_touch_callbacks(GLFWwindow *w) {}

#else
#include <emscripten/html5.h>

static double touch_activated_at = 0;
static bool touch_just_activated = false;

extern void set_just_clicked(void);

static EM_BOOL on_touchstart(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData) {
    if (touch_active) {
        // Another finger was touched when one was already touching.
        if (touchEvent->numTouches == 2) {
            // 2-finger = move forward
            touch_forward = true;
            touch_jump = false;
        } else if (touchEvent->numTouches == 3) {
            // 3-finger = jump
            touch_jump = true;
        }
        // TODO: support other interesting gestures
        return EM_TRUE;
    }

    touch_active = touchEvent->touches[0].identifier;
    touch_activated_at = glfwGetTime();
    touch_just_activated = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return EM_TRUE;
}

static EM_BOOL on_touchmove(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData) {
    if (touch_just_activated) {
        set_just_clicked(); // ignore next movement, since two datapoints are needed to move
        touch_just_activated = false;
    }
    touch_clientX = touchEvent->touches[0].clientX;
    touch_clientY = touchEvent->touches[0].clientY;
    return EM_TRUE;
}

static EM_BOOL on_touchend(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData) {
    if (touchEvent->numTouches <= 2) {
        touch_forward = false;
    }

    if (touchEvent->numTouches <= 3) {
        touch_jump = false;
    }

    for (int i = 0; i < touchEvent->numTouches; ++i) {
        EmscriptenTouchPoint touch = touchEvent->touches[i];
        if (touch.isChanged && touch.identifier == touch_active) {
            // Was the first touch released? If so, exit touch.
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            touch_active = 0;

            double duration = glfwGetTime() - touch_activated_at;
            if (duration < 0.07) {
                // Short duration = tap = left-click = break blocks
                // TODO: only tap if touchstart position =~ touchend position?

                int width = 0, height = 0;
                glfwGetWindowSize(window, &width, &height);

                //printf("tap at (%ld,%ld) within (%d,%d)\n", touch.clientX, touch.clientY, width, height);

                if (touch.clientX < 80) { // TODO: && touch.clientY < height - 80? (bottom left center vs corner)
                    // Tapping near the item icon = place
                    on_build();
                } else {
                    // Tapping elswhere = break
                    on_mine();
                }
            }
        }
    }

    return EM_TRUE;
}

static EM_BOOL on_touchcancel(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData) {
    touch_active = 0;
    return EM_TRUE;
}

void init_touch_callbacks(GLFWwindow *w) {
    window = w;
    emscripten_set_touchstart_callback(NULL, NULL, EM_FALSE, on_touchstart);
    emscripten_set_touchmove_callback(NULL, NULL, EM_FALSE, on_touchmove);
    emscripten_set_touchend_callback(NULL, NULL, EM_FALSE, on_touchend);
    emscripten_set_touchcancel_callback(NULL, NULL, EM_FALSE, on_touchcancel);
}
#endif
