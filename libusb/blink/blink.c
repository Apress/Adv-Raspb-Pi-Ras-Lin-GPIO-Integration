//////////////////////////////////////////////////////////////////////
// blink.c -- Blink LEDs on PA0 and PA1
// Date: Wed Mar 28 23:00:12 2018   (C) ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#include <fx2regs.h>
#include <fx2sdly.h>

static void
delay(unsigned times) {
	unsigned int x, y;
	
	for ( x=0; x<times; x++ ) {
		for ( y=0; y<200; y++ ) {
			SYNCDELAY;
		}
	}
}

void
main(void) {

	OEA = 0x03;		// PA0 & PA1 is output

	for (;;) {
		PA0 = 1;
		PA1 = 0;
		delay(1000);
		PA0 = 0;
		PA1 = 1;
		delay(1000);
	}
}

// End blink.c

