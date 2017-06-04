struct ItemStack {
    int type;
    int count;
};

extern struct ItemStack hotbar[9];

bool itemstack_can_merge(struct ItemStack *stack1, struct ItemStack *stack2);
int itemstack_add(struct ItemStack *stack, int quantity);
int inventory_add(struct ItemStack *inventory, int size, struct ItemStack *stack);

#define HOTBAR_INVENTORY_SIZE 9
