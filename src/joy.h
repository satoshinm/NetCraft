#define GAMEPAD_LOOK_SENSITIVITY 10
#define GAMEPAD_MOVE_SENSITIVITY 1

// Joystick bindings
// See standard gamepad at https://www.w3.org/TR/gamepad/#remapping
// and test site http://html5gamepad.com
#define GAMEPAD_A 0
#define GAMEPAD_L1_BUMPER 4
#define GAMEPAD_R1_BUMPER 5
#define GAMEPAD_L2_TRIGGER 6
#define GAMEPAD_R2_TRIGGER 7
#define GAMEPAD_DPAD_LEFT 8
#define GAMEPAD_DPAD_DOWN 9
#define GAMEPAD_DPAD_RIGHT 10
#define GAMEPAD_DPAD_UP 11

bool is_gamepad_connected();
void handle_gamepad_input();
void init_joystick(int joy);
void init_joystick_callbacks();
void joystick_apply_move(double *sx, double *sz);
void joystick_apply_look(double *mx, double *my, double px, double py);
void joystick_apply_buttons(bool *jumping, bool *crouching);
