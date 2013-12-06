#include "board_kb.h"

unsigned char board_keyboard_get(void) {
	unsigned char k = board_keyboard;
	board_keyboard = BOARD_KEY_NO_KEY;
	return k;
}

void board_keyboard_process(void) {
	static unsigned char prev_keyboard = BOARD_KEY_NO_KEY;
	static unsigned char curr_keyboard;
	static unsigned char stable_states = 0xFF;

	curr_keyboard = board_key_read();

	switch (stable_states) {
	case 0:
		stable_states = 1;
		prev_keyboard = curr_keyboard;
		break;

	case board_keyboard_bounces:
		stable_states++;
		board_keyboard = prev_keyboard;
		break;

	default:
//		if ((curr_keyboard == BOARD_KEY_NO_KEY) || (curr_keyboard != prev_keyboard)) {
		if (curr_keyboard != prev_keyboard) {
			stable_states = 0;
			prev_keyboard = curr_keyboard;
		} else {
			if (stable_states < board_keyboard_bounces) {
				stable_states++;
			}
		}
		break;
	}

	return;
}

unsigned char board_keyboard_read(void) {
	unsigned char k = BOARD_KEY_NO_KEY;
	k |= (reg_board_key_1 == BOARD_KEY_DOWN)? BOARD_KEY_1 : BOARD_KEY_NO_KEY;
	k |= (reg_board_key_2 == BOARD_KEY_DOWN)? BOARD_KEY_2 : BOARD_KEY_NO_KEY;
	k |= (reg_board_key_3 == BOARD_KEY_DOWN)? BOARD_KEY_3 : BOARD_KEY_NO_KEY;
	k |= (reg_board_key_4 == BOARD_KEY_DOWN)? BOARD_KEY_4 : BOARD_KEY_NO_KEY;
	return k;
}
