int mining_get_target(int *x, int *y, int *z, int *face);
bool is_mining(void);
void mining_stop(void);
void mining_tick(void);
void mining_start(void);
void building_stop(void);
void building_tick(void);
void building_start(void);

#define BLOCK_BUILD_TIME 10

extern void on_mine(void);
extern void on_build(void);

extern int mining_stage;
extern int target_x;
extern int target_y;
extern int target_z;
extern int target_face;
extern int target_w;
