#include "serial.h"

char *serial_recieve_message(unsigned char *done) {
	static char state = SERIAL_STATE_BEGIN;
	static char msg[SERIAL_MAX_SIZE];
	static char inx = 0;

	*done = 0;
	int data = serial_pop_rx();
	if (data == -1) {
		return NULL;
	}

	unsigned char dato = (unsigned char) data;
	switch (state) {
	case SERIAL_STATE_BEGIN:
		inx = 0;
		if (dato == SERIAL_MESSAGE_START) {
			state = SERIAL_STATE_CONTINUE;
		}
		break;

	case SERIAL_STATE_CONTINUE:
		if (dato == SERIAL_MESSAGE_STOP) {
			state = SERIAL_STATE_END;
		} else {
			//see http://www.ascii-code.com/ for printable characers
			if (dato >= 0x20 && dato <= 0x7F) {
				msg[inx] = (unsigned char) dato;
				inx++;
				if (inx == MAX_PALABRA - 1) {
					state = SERIAL_STATE_BEGIN;
				}
			} else { //invalid input char
				state = SERIAL_STATE_BEGIN;
			}
		}
		break;

	case SERIAL_STATE_END:
		state = SERIAL_STATE_BEGIN;
		msg[inx] = '\0';
		*done = 1;
		break;
	}

	return msg;
}

void serial_send_message(char *msg) {
	char i;
	for (i = 0; msg[i]; i++)
		serial_push_tx(msg[i]);

	if (!serial_tx_flag) {
		serial_tx_flag = 1; 
		SERIAL_TX_START();
	}
}

void serial_push(unsigned char mode, unsigned char dato) {
	if (mode == SERIAL_RX_MODE) {
		serial_rx_buffer[serial_rx_in] = dato;
		serial_rx_in ++;
		serial_rx_in %= SERIAL_RX_MAX;
	}
	if (mode == SERIAL_TX_MODE) {
		serial_tx_buffer[serial_tx_in] = dato;
		serial_tx_in ++;
		serial_tx_in %= SERIAL_TX_MAX;
	}
}

int serial_pop(unsigned char mode) {
	int dato = -1;
	if (mode == SERIAL_RX_MODE) {
		if (serial_rx_in != serial_rx_out) {
			dato = (int) serial_rx_buffer[serial_rx_out];
			serial_rx_out ++;
			serial_rx_out %= SERIAL_RX_MAX;
		}
	}
	if (mode == SERIAL_TX_MODE) {
		if (serial_tx_in != serial_tx_out) {
			dato = (int) serial_tx_buffer[serial_tx_out];
			serial_tx_out ++;
			serial_tx_out %= SERIAL_TX_MAX;
		}
	}

	return dato;
}

void serial_process(void) {
	unsigned char datotx;
	int datorx;
	if (reg_serial_rx) {
		SERIAL_RX_STOP();
		datorx = SERIAL_BUFFER;
		serial_push_rx(datorx);
	}
	SERIAL_TX_STOP();
	datotx = serial_pop_tx();
	if (datotx != -1)
		SERIAL_BUFFER = (unsigned char) datotx;
	else
		serial_tx_flag = 0;
}

void serial_interrupt(void) interrupt 4 {
	serial_process();
}
