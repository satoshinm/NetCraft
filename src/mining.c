#include "mining.h"
#include "item.h"

static int mining_progress = 0;
static int holding_mine_button = 0;
static int building_progress = 0;
static int holding_build_button = 0;

static int mining_x = 0, mining_y = 0, mining_z = 0;

extern void on_mine();
extern void on_build();
extern int get_targeted_block(int *hx, int *hy, int *hz);

void mining_stop() {
    holding_mine_button = 0;
    mining_progress = 0;
}

void mining_tick() {
    if (holding_mine_button) {
        int x, y, z;
        int w = get_targeted_block(&x, &y, &z);

        if (x != mining_x || y != mining_y || z != mining_z) {
            mining_x = x;
            mining_y = y;
            mining_z = z;

            mining_progress = 0;
            return;
        }

        if (mining_progress == is_hardness(w)) {
            mining_progress = 0;
            on_mine();
            return;
        }

        mining_progress++;
        // TODO: block break indicator
    }
}

void mining_start() {
    holding_mine_button = 1;

    (void) get_targeted_block(&mining_x, &mining_y, &mining_z);
}


void building_stop() {
    holding_build_button = 0;
    building_progress = 0;
}


void building_tick() {
    if (holding_build_button) {
        if (building_progress == BLOCK_BUILD_TIME) {
            building_progress = 0;
            on_build();
        }

        building_progress++;
    }
}

void building_start() {
    holding_build_button = 1;
    building_progress = BLOCK_BUILD_TIME; // place first instantly
}
