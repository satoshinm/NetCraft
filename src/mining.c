#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "mining.h"
#include "item.h"

static int mining_progress = 0;
static bool holding_mine_button = false;
static int building_progress = 0;
static bool holding_build_button = false;

static int mining_x = 0;
static int mining_y = 0;
static int mining_z = 0;
int mining_stage = 0;

int target_x = 0;
int target_y = 0;
int target_z = 0;
int target_face = 0;
int target_w = 0;

extern void on_mine();
extern void on_build();
extern int get_targeted_block(int *x, int *y, int *z, int *face);

bool is_mining() {
    return holding_mine_button && target_w != 0;
}

void mining_stop() {
    holding_mine_button = false;
    mining_progress = false;
    mining_stage = 0;
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
            mining_stage = 0;
            return;
        }

        if (!target_w) return;

        int hardness = is_hardness(target_w);

        // Block break indicator
        if (hardness == 0) mining_stage = 9;
        else mining_stage = (int)roundf((float)mining_progress / hardness * 9);

        if (mining_progress == hardness) {
            mining_progress = 0;
            mining_stage = 0;
            on_mine();
            return;
        }

        mining_progress++;
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
