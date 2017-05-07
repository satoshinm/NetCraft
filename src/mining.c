#include "mining.h"

static int mining_progress = 0;
static int holding_mine_button = 0;
static int building_progress = 0;
static int holding_build_button = 0;

extern void on_left_click();
extern void on_right_click();

void mining_stop() {
    holding_mine_button = 0;
    mining_progress = 0;
}

void mining_tick() {
    if (holding_mine_button) {
        if (mining_progress == BLOCK_BREAK_TIME) { // TODO: variable hardness
            mining_progress = 0;
            on_left_click();
        }

        mining_progress++;
        // TODO: block break indicator
    }
}

void mining_start() {
    holding_mine_button = 1;
}


void building_stop() {
    holding_build_button = 0;
    building_progress = 0;
}


void building_tick() {
    if (holding_build_button) {
        if (building_progress == BLOCK_BUILD_TIME) {
            building_progress = 0;
            on_right_click();
        }

        building_progress++;
    }
}

void building_start() {
    holding_build_button = 1;
    building_progress = BLOCK_BUILD_TIME; // place first instantly
}
