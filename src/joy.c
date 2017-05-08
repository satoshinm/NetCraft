#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include "joy.h"
#include "config.h"
#include "mining.h"

extern void on_scroll(GLFWwindow *window, double xdelta, double ydelta);
extern void on_key(GLFWwindow *window, int key, int scancode, int action, int mods);

static int gamepad_connected = -1;

static struct {
    float scale_moveH, scale_moveV;
    float scale_lookH, scale_lookV;

    int axis_count;
    const float *axis;
    int digitalButton_count;
    const unsigned char *digitalButton;
} gamepad_state;

int is_gamepad_connected() {
    return gamepad_connected != -1;
}

#ifdef USE_EM_GAMEPAD
#include <emscripten/html5.h>
static EM_BOOL on_gamepadconnected(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData) {
    gamepad_connected = 1;
    // TODO: track individual gamepad identifiers?
    printf("gamepad connected\n");

    return EM_TRUE;
}

static EM_BOOL on_gamepaddisconnected(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData) {
    gamepad_connected = -1;
    printf("gamepad disconnected\n");

    return EM_TRUE;
}
#endif

void handle_gamepad_input() {
    if (gamepad_connected == -1) return;

    static struct {
        int axis_count;
        float axis[16];
        int digitalButton_count;
        unsigned char digitalButton[16];
    } last_gamepad_state = {0};

#ifdef USE_EM_GAMEPAD
    static EmscriptenGamepadEvent gp;
    emscripten_get_gamepad_status(0, &gp);

    static float _axis[16] = {0};
    static unsigned char _digitalButton[16] = {0};
    if (!gamepad_state.axis) gamepad_state.axis = _axis;
    if (!gamepad_state.digitalButton) gamepad_state.digitalButton = _digitalButton;

    gamepad_state.axis_count = gp.numAxes;
    for (int i = 0; i < gp.numAxes; ++i) {
        _axis[i] = gp.axis[i];
    }

    gamepad_state.digitalButton_count = gp.numButtons;
    for (int i = 0; i < gp.numButtons; ++i) {
        _digitalButton[i] = gp.digitalButton[i];
    }
#else
    gamepad_state.axis = glfwGetJoystickAxes(gamepad_connected, &gamepad_state.axis_count);
    gamepad_state.digitalButton = glfwGetJoystickButtons(gamepad_connected, &gamepad_state.digitalButton_count);
#endif

    // Bumpers scroll
    if (gamepad_state.digitalButton_count > GAMEPAD_L1_BUMPER &&
        gamepad_state.digitalButton[GAMEPAD_L1_BUMPER] && !last_gamepad_state.digitalButton[GAMEPAD_L1_BUMPER]) {
        on_scroll(NULL, 0, SCROLL_THRESHOLD + 1);
    }
    if (gamepad_state.digitalButton_count > GAMEPAD_R1_BUMPER &&
        gamepad_state.digitalButton[GAMEPAD_R1_BUMPER] && !last_gamepad_state.digitalButton[GAMEPAD_R1_BUMPER]) {
        on_scroll(NULL, 0, -SCROLL_THRESHOLD - 1);
    }

    // Triggers click
    if (gamepad_state.digitalButton_count > GAMEPAD_L2_TRIGGER) {
        if (gamepad_state.digitalButton[GAMEPAD_L2_TRIGGER] &&!last_gamepad_state.digitalButton[GAMEPAD_L2_TRIGGER]) {
            building_start();
        } else if (!gamepad_state.digitalButton[GAMEPAD_L2_TRIGGER] && last_gamepad_state.digitalButton[GAMEPAD_L2_TRIGGER]) {
            building_stop();
        }
    }
    if (gamepad_state.digitalButton_count > GAMEPAD_R2_TRIGGER) {
        if (gamepad_state.digitalButton[GAMEPAD_R2_TRIGGER] && !last_gamepad_state.digitalButton[GAMEPAD_R2_TRIGGER]) {
            mining_start();
        } else if (!gamepad_state.digitalButton[GAMEPAD_R2_TRIGGER] && last_gamepad_state.digitalButton[GAMEPAD_R2_TRIGGER]) {
            mining_stop();
        }
    }

    // Jump key needs events to detect double-tap for toggling fly
    if (gamepad_state.digitalButton_count > GAMEPAD_A &&
        gamepad_state.digitalButton[GAMEPAD_A] && !last_gamepad_state.digitalButton[GAMEPAD_A]) {
        on_key(NULL, CRAFT_KEY_JUMP, 0, GLFW_PRESS, 0);
    }


    last_gamepad_state.axis_count =  gamepad_state.axis_count;
    for (int i = 0; i < gamepad_state.axis_count; ++i) {
        last_gamepad_state.axis[i] = gamepad_state.axis[i];
    }

    last_gamepad_state.digitalButton_count =  gamepad_state.digitalButton_count;
    for (int i = 0; i < gamepad_state.digitalButton_count; ++i) {
        last_gamepad_state.digitalButton[i] = gamepad_state.digitalButton[i];
    }
}

void init_joystick(int joy) {
    const char *name = glfwGetJoystickName(joy);

    printf("Joystick %d connected: %s\n", joy, name);
    gamepad_state.scale_lookH = GAMEPAD_LOOK_SENSITIVITY;
    gamepad_state.scale_lookV = GAMEPAD_LOOK_SENSITIVITY;
    gamepad_state.scale_moveH = GAMEPAD_MOVE_SENSITIVITY;
    gamepad_state.scale_moveV = GAMEPAD_MOVE_SENSITIVITY;

    if (strstr(name, "STANDARD GAMEPAD")) {
        // Invert axes, for some reason these seem to be backwards TODO: configurable
        gamepad_state.scale_lookV *= -1;
        gamepad_state.scale_moveV *= -1;
    }

    gamepad_connected = joy;
    handle_gamepad_input();
    printf("Joystick axes: %d, buttons: %d\n", gamepad_state.axis_count, gamepad_state.digitalButton_count);
}

void on_joystick_connection(int joy, int event) {
    if (event == GLFW_CONNECTED) {
        init_joystick(joy);
    } else if (event == GLFW_DISCONNECTED) {
        printf("Joystick disconnected: %d\n", joy);
        gamepad_connected = -1;
    }
}


void init_joystick_callbacks() {
    gamepad_connected = -1;
#ifdef USE_EM_GAMEPAD
    emscripten_set_gamepadconnected_callback(NULL, EM_FALSE, on_gamepadconnected);
    emscripten_set_gamepaddisconnected_callback(NULL, EM_FALSE, on_gamepaddisconnected);
#else
    glfwSetJoystickCallback(on_joystick_connection);
#endif
}

void joystick_apply_move(double *sx, double *sz) {
    if (gamepad_connected == -1) return;

    if (gamepad_state.digitalButton_count > GAMEPAD_DPAD_LEFT &&
        gamepad_state.digitalButton[GAMEPAD_DPAD_LEFT]) (*sx)--;

    if (gamepad_state.digitalButton_count > GAMEPAD_DPAD_RIGHT &&
        gamepad_state.digitalButton[GAMEPAD_DPAD_RIGHT]) (*sx)++;

    if (gamepad_state.axis_count > GAMEPAD_RIGHT_STICK_HORIZONTAL &&
        gamepad_state.axis_count > GAMEPAD_RIGHT_STICK_VERTICAL) {
        *sx += gamepad_state.axis[GAMEPAD_LEFT_STICK_HORIZONTAL] * gamepad_state.scale_moveH;
        *sz -= gamepad_state.axis[GAMEPAD_LEFT_STICK_VERTICAL] * gamepad_state.scale_moveV;
    }
}

void joystick_apply_look(double *mx, double *my, double px, double py) {
    if (gamepad_connected == -1) return;

    if (gamepad_state.axis_count > GAMEPAD_RIGHT_STICK_HORIZONTAL &&
        gamepad_state.axis_count > GAMEPAD_RIGHT_STICK_VERTICAL) {
        *mx = px + gamepad_state.axis[GAMEPAD_RIGHT_STICK_HORIZONTAL] * gamepad_state.scale_lookH;
        *my = py + gamepad_state.axis[GAMEPAD_RIGHT_STICK_VERTICAL] * gamepad_state.scale_lookV;
    }
}

void joystick_apply_buttons(int *jumping, int *crouching) {
    if (gamepad_connected == -1) return;

    if (gamepad_state.digitalButton_count > GAMEPAD_A &&
        gamepad_state.digitalButton[GAMEPAD_A]) *jumping = 1;

    if (gamepad_state.digitalButton_count > GAMEPAD_DPAD_UP &&
        gamepad_state.digitalButton[GAMEPAD_DPAD_UP]) *jumping = 1;

    if (gamepad_state.digitalButton_count > GAMEPAD_DPAD_DOWN &&
        gamepad_state.digitalButton[GAMEPAD_DPAD_DOWN]) *crouching = 1;
}


