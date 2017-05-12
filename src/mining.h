int mining_get_target(int *x, int *y, int *z, int *face);
bool is_mining();
void mining_stop();
void mining_tick();
void mining_start();
void building_stop();
void building_tick();
void building_start();

#define BLOCK_BUILD_TIME 10

extern void on_mine();
extern void on_build();

extern int mining_stage;
extern int target_x;
extern int target_y;
extern int target_z;
extern int target_face;
extern int target_w;
