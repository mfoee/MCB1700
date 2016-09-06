#ifndef ___game
#define __game
#define height 25
#define width 25
#define length 1250
#define x_space 1
#define y_space 5
#define y_pitch 30
#define x_pitch 26
extern unsigned char all[];
extern unsigned char facedown[];
extern unsigned int shuffle[60];
extern int facedown_cards(int row);
extern int create_level(int level);
extern int draw_line(int x, int y);
extern int clear_line(int x, int y);
extern int start_game(int lvl);
extern void game_on (void);
#endif
