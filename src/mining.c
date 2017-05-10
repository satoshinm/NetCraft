#include <stdbool.h>
#include "mining.h"
#include "item.h"

static int mining_progress = 0;
static bool holding_mine_button = false;
static int building_progress = 0;
static bool holding_build_button = false;

static int mining_x = 0;
static int mining_y = 0;
static int mining_z = 0;

static int target_x = 0;
static int target_y = 0;
static int target_z = 0;
static int target_face = 0;
static int target_w = 0;

extern void on_mine();
extern void on_build();
extern int get_targeted_block(int *x, int *y, int *z, int *face);

// Get cached mining target (fast)
int mining_get_target(int *x, int *y, int *z, int *face) {
    *x = target_x;
    *y = target_y;
    *z = target_z;
    *face = target_face;
    return target_w;
}

void mining_stop() {
    holding_mine_button = false;
    mining_progress = false;
}

void mining_tick() {
    // Retarget block every tick
    target_w = get_targeted_block(&target_x, &target_y, &target_z, &target_face);

    if (holding_mine_button) {
        if (target_x != mining_x || target_y != mining_y || target_z != mining_z) {
            mining_x = target_x;
            mining_y = target_y;
            mining_z = target_z;

            mining_progress = 0;
            return;
        }

        if (mining_progress == is_hardness(target_w)) {
            mining_progress = 0;
            on_mine();
            return;
        }

        mining_progress++;
        // TODO: block break indicator
    }
}

void mining_start() {
    holding_mine_button = true;

    (void) get_targeted_block(&mining_x, &mining_y, &mining_z, &target_face);
}


void building_stop() {
    holding_build_button = false;
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
    holding_build_button = true;
    building_progress = BLOCK_BUILD_TIME; // place first instantly
}
