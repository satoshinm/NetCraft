#include <stdlib.h>
#include <stdbool.h>
#include "inventory.h"

#define MAX_COUNT 16

struct ItemStack hotbar[9] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
};

bool itemstack_can_merge(struct ItemStack *stack1, struct ItemStack *stack2) {
    if (stack1->count == MAX_COUNT) return false;
    if (stack1->type == 0) return true;
    if (stack1->type != stack2->type) return false;
    return true;
}

int itemstack_add(struct ItemStack *stack, int quantity) {
    int remainder = 0;

    stack->count += quantity;
    if (stack->count > MAX_COUNT) {
        remainder = stack->count - MAX_COUNT;
        stack->count = MAX_COUNT;
    }

    return remainder;
}

int itemstack_subtract(struct ItemStack *stack, int quantity) {
    int missing = 0;

    stack->count -= quantity;
    if (stack->count <= 0) {
        missing = -stack->count;
        stack->count = 0;
        stack->type = 0;
    }

    return missing;
}

int inventory_add(struct ItemStack *inventory, int size, struct ItemStack *stack) {
    int remainder = stack->count;

    for (int i = 0; i < size; ++i) {
        if (itemstack_can_merge(&inventory[i], stack)) {
            inventory[i].type = stack->type;
            remainder = itemstack_add(&inventory[i], remainder);
            if (remainder == 0) break;
        }
    }

    return remainder;
}

int inventory_subtract(struct ItemStack *inventory, int size, struct ItemStack *stack) {
    int missing = stack->count;

    for (int i = 0; i < size; ++i) {
        if (inventory[i].type == stack->type) {
            missing = itemstack_subtract(&inventory[i], missing);
            if (missing == 0) break;
        }
    }

    return missing;
}
