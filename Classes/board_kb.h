#ifndef BOARD_KB_H
#define BOARD_KB_H

#include "base.h"
#include "registry.h"

//defines
#define BOARD_KEY_DOWN !ON
//to be used with masks. Allows multiple buttons to be pressed at the same time
#define BOARD_KEY_NO_KEY 0x00
#define BOARD_KEY_1 0x01
#define BOARD_KEY_2 0x02
#define BOARD_KEY_3 0x04
#define BOARD_KEY_4 0x08

#define board_keyboard_bounces 4

//primitives
unsigned char board_keyboard_get(void);

//buffers
//supports up to 8 keys
unsigned char board_keyboard;

//drivers
void board_keyboard_process(void);
unsigned char board_keyboard_read(void);

#endif
