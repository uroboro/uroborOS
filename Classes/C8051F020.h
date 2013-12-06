#ifndef C8051F020_H
#define C8051F020_H

//#include "base.h"
//change sbit to getbit/setbit

//board keyboard
sbit reg_board_key_1 = P0^4;
sbit reg_board_key_2 = P0^5;
sbit reg_board_key_3 = P0^6;
sbit reg_board_key_4 = P0^7;

//serial comm
#define reg_serial_rx RI0
#define reg_serial_tx TI0
#define reg_serial_buf SBUF0

//I2C
sbit I2C_SDA = P1^6;
sbit I2C_SCL = P1^7;


#endif
