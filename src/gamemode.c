#include <stdbool.h>
#include "gamemode.h"

static bool survival = false;

bool is_creative_gamemode() {
    return !survival;
}

bool is_survival_gamemode() {
    return survival;
}

void set_survival_gamemode() {
    survival = true;
}

void set_creative_gamemode() {
    survival = false;
}

