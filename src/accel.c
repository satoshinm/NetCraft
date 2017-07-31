#ifndef __EMSCRIPTEN__
void init_accel_callbacks(void) {
}

#else
#include <emscripten.h>
#include <emscripten/html5.h>

static EM_BOOL on_devicemotion(int eventtype, const EmscriptenDeviceMotionEvent *event, void *userData) {
    //printf("accel x=%g, y=%g, %g\n", event->accelerationX, event->accelerationY, event->accelerationZ);
    // TODO: free look

    return EM_TRUE;
}

void init_accel_callbacks(void) {
    emscripten_set_devicemotion_callback(NULL, EM_TRUE, on_devicemotion);
}
#endif

