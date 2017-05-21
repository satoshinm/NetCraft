#ifndef _config_h_
#define _config_h_

// app parameters
#define DEBUG true
#define FULLSCREEN false
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define VSYNC true
#define SCROLL_THRESHOLD 0.1
#define JUMP_FLY_THRESHOLD 0.2
#define SCROLL_BLOCK_SELECT true
#define MAX_MESSAGES 20
#define MAX_VISIBLE_MESSAGES 4
#define DB_PATH "craft.db"
#ifdef __EMSCRIPTEN__
#define USE_CACHE false
#else
#define USE_CACHE true
#endif
#define DAY_LENGTH 600
#define INVERT_MOUSE false
#define PLAYER_NAME_DISTANCE 96 // Maximum distance to display a player's name at

// rendering options
#define SHOW_LIGHTS true
#define SHOW_PLANTS true
#define SHOW_CLOUDS true
#define SHOW_TREES true
#define SHOW_ITEM true
#define SHOW_CROSSHAIRS true
#define SHOW_WIREFRAME true
#define SHOW_INFO_TEXT true
#define SHOW_CHAT_TEXT true
#define SHOW_PLAYER_NAMES true

// key bindings
#define CRAFT_KEY_FORWARD 'W'
#define CRAFT_KEY_BACKWARD 'S'
#define CRAFT_KEY_LEFT 'A'
#define CRAFT_KEY_RIGHT 'D'
#define CRAFT_KEY_JUMP GLFW_KEY_SPACE
#define CRAFT_KEY_SPRINT GLFW_KEY_TAB
#define CRAFT_KEY_CROUCH GLFW_KEY_LEFT_SHIFT
#define CRAFT_KEY_OBSERVE 'O'
#define CRAFT_KEY_OBSERVE_INSET 'P'
#define CRAFT_KEY_ITEM_NEXT 'E'
#define CRAFT_KEY_ITEM_PREV 'R'
#define CRAFT_KEY_ZOOM 'Z'
#define CRAFT_KEY_ORTHO 'F'
#define CRAFT_KEY_CHAT 't'
#define CRAFT_KEY_COMMAND '/'
#define CRAFT_KEY_SIGN '`'
#define CRAFT_KEY_FULLSCREEN GLFW_KEY_F11
#define CRAFT_KEY_VR GLFW_KEY_F4
#define CRAFT_KEY_DEBUG GLFW_KEY_F3
#define CRAFT_KEY_SCREENSHOT GLFW_KEY_F2
#define CRAFT_KEY_UI GLFW_KEY_F1

#ifdef __EMSCRIPTEN__
//#define USE_EM_GAMEPAD 1
#define USE_EM_FULLSCREEN 1
#endif

#define GAMEPAD_LEFT_STICK_HORIZONTAL 0
#define GAMEPAD_LEFT_STICK_VERTICAL 1
#define GAMEPAD_RIGHT_STICK_HORIZONTAL 2
#define GAMEPAD_RIGHT_STICK_VERTICAL 3

// advanced parameters
#define CREATE_CHUNK_RADIUS 10
#define RENDER_CHUNK_RADIUS 10
#define RENDER_SIGN_RADIUS 4
#define DELETE_CHUNK_RADIUS 14
#define CHUNK_SIZE 32
#define COMMIT_INTERVAL 5
#define ORTHO_ZOOM_MIN 8
#define ORTHO_ZOOM_MAX 128

#endif
