#include "board_kb.h"
#include "i2c.h"
#include "rtc.h"
#include "serial.h"

/*
//init:

	// Configuro el modo 1 de funcionamiento para el timer 0
	TMOD = 0x21;
	TH0 = Tmr_TH0;
	TL0 = Tmr_TL0 + TL0;
	TR0 = 1;

	TF0 = 0;
	ET0 = 1;
	EA = 1;

	//Configuro interrupcion puerto SERIE
    SCON0 = 0x50;        // MODO 1, 8 BIT
    TMOD = 0x21;        // timer 1, modo 2
    TH1  = 0xFD;        //9600 baudios
    TR1  = 1;
    TI0  = 0;

*/


// valores de timer para 5ms
#define Tmr_TH0 0xEC
#define Tmr_TL0 0x77

#define DECIMAS 	20
#define SEGUNDOS	10

unsigned char decimas = DECIMAS, segundos = SEGUNDOS;

void timer_process(void) {
	unsigned char h, m, s;

	board_keyboard_set();
	serial_process();

	decimas--;
	if (!decimas) {
		decimas = DECIMAS;

		h = RTC_read(0x02);
		h = (h >> 4)*10 + (h & 0x0F);
		m = RTC_read(0x01);
		m = (m >> 4)*10 + (m & 0x0F);
		s = RTC_read(0x00);
		s = (s >> 4)*10 + (s & 0x0F);

		TiempoRTC = (int)h*3600 + (int)m*60 + (int)s;
		
		unsigned char kb = board_keyboard_get();
		BotonT = (kb & BOARD_KEY1)? ON:OFF;
		BotonN = (kb & BOARD_KEY2)? ON:OFF;
		BotonM = (kb & BOARD_KEY3)? ON:OFF;
		BotonB = (kb & BOARD_KEY4)? ON:OFF;

		segundos--;
		if(!segundos) {
			segundos = SEGUNDOS;

			EnviarKeepAlive(); //Keep Alive
		}
	}

	return;
}

//SCHEDULER
void timer_interrupt(void) interrupt 1 {
	TR0 = 0; //disable interrupts

	TH0 = Tmr_TH0;
	TL0 += Tmr_TL0;

	P5 = 0x10;

	timer_process();

	TR0 = 1; //enable interrupts

	return;
}
