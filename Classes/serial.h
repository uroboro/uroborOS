#ifndef SERIAL_H
#define SERIAL_H

#include "registry.h"

//defines
#define SERIAL_BUFFER reg_serial_buf

#define SERIAL_RX_START() reg_serial_rx = 1
#define SERIAL_RX_STOP() reg_serial_rx = 0
#define SERIAL_TX_START() reg_serial_tx = 1
#define SERIAL_TX_STOP() reg_serial_tx = 0

#define SERIAL_STATE_BEGIN 0
#define SERIAL_STATE_CONTINUE 1
#define SERIAL_STATE_END 2

//see http://www.ascii-code.com/ for start/end of text
#define SERIAL_MESSAGE_START 0x02
#define SERIAL_MESSAGE_STOP 0x03

#define SERIAL_RX_MODE 0
#define SERIAL_TX_MODE 1

#define serial_push_rx(d) serial_push(SERIAL_RX_MODE, d)
#define serial_push_tx(d) serial_push(SERIAL_TX_MODE, d)
#define serial_pop_rx() serial_pop(SERIAL_RX_MODE)
#define serial_pop_tx() serial_pop(SERIAL_TX_MODE)

#define SERIAL_MAX_SIZE 20
#define SERIAL_TX_MAX SERIAL_MAX_SIZE
#define SERIAL_RX_MAX SERIAL_MAX_SIZE

//primitives
void serial_send_message(char *msg);
char *serial_recieve_message(unsigned char *done);

//buffers
volatile unsigned char serial_tx_buffer[SERIAL_TX_MAX];
volatile unsigned char serial_rx_buffer[SERIAL_RX_MAX];

volatile unsigned char serial_tx_in = 0, serial_tx_out = 0;
volatile unsigned char serial_rx_in = 0, serial_rx_out = 0;

// flag de fin de TX
volatile unsigned char serial_tx_flag;

//drivers
void serial_push(unsigned char mode, unsigned char dato);
int serial_pop(unsigned char mode);

void serial_process(void);

#endif