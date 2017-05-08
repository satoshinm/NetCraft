int mining_get_target(int *hx, int *hy, int *hz);
void mining_stop();
void mining_tick();
void mining_start();
void building_stop();
void building_tick();
void building_start();

#define BLOCK_BUILD_TIME 10

extern void on_mine();
extern void on_build();
