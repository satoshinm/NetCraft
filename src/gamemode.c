#include <stdbool.h>
#include "gamemode.h"

bool is_creative = true;

void set_survival_gamemode() {
    is_creative = false;
}

void set_creative_gamemode() {
    is_creative = true;
}

