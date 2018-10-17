//////////////////////////////////////////////////////////////////////
// ezusb.c -- Example CY7C68013A firmware to control LEDS and
//	      and demonstrate some bulk USB I/O.
// Date: Mon Apr  2 22:49:16 2018   (C) ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#include <fx2regs.h>
#include <fx2sdly.h>

static void
initialize(void) {

	CPUCS = 0x10;		// 48 MHz, CLKOUT disabled.
	SYNCDELAY;		
	IFCONFIG = 0xc0;	// Internal IFCLK @ 48MHz
	SYNCDELAY;		
	REVCTL = 0x03;		// Disable auto-arm + Enhanced packet handling
	SYNCDELAY;		
	EP6CFG = 0xE2;		// 1110_0010 bulk IN, 512 bytes, double-buffered
	SYNCDELAY;
	EP2CFG = 0xA2;		// 1010_0010 bulk OUT, 512 bytes, double-buffered
	SYNCDELAY;
	FIFORESET = 0x80;	// NAK all requests from host.
	SYNCDELAY;	
	FIFORESET = 0x82;	// Reset EP 2
	SYNCDELAY;	
	FIFORESET = 0x84;	// Reset EP 4..
	SYNCDELAY;
	FIFORESET = 0x86;
	SYNCDELAY;
	FIFORESET = 0x88;
	SYNCDELAY;
	FIFORESET = 0x00;	// Back to normal..
	SYNCDELAY;		
	EP2FIFOCFG = 0x00;	// Disable AUTOOUT
	SYNCDELAY;		
	OUTPKTEND = 0x82;	// Clear the 1st buffer
	SYNCDELAY;		
	OUTPKTEND = 0x82;	// ..both of them
	SYNCDELAY;		
}

//////////////////////////////////////////////////////////////////////
// Accept a command byte from EP2
//////////////////////////////////////////////////////////////////////

static void
accept_cmd(void) {
	__xdata const unsigned char *src = EP2FIFOBUF;
	unsigned len = ((unsigned)EP2BCH)<<8 | EP2BCL;

	if ( len < 1 )
		return;		// Nothing to process
	PA0 = *src & 1;		// Set PA0 LED
	PA1 = *src & 2;		// Set PA1 LED
	OUTPKTEND = 0x82;	// Release buffer
}

//////////////////////////////////////////////////////////////////////
// Send a state message back to host over EP6
//////////////////////////////////////////////////////////////////////

static void
send_state(void) {
	__xdata unsigned char *dest = EP6FIFOBUF;
	const char *msg1 = PA0 ? "PA0=1" : "PA0=0";
	const char *msg2 = PA1 ? "PA1=1" : "PA1=0";
	unsigned char len=0;

	while ( *msg1 ) {
		*dest++ = *msg1++;
		++len;  
	}
	*dest++ = ',';
	++len;
	while ( *msg2 ) {
		*dest++ = *msg2++;
		++len;
	}

	SYNCDELAY;  
	EP6BCH=0;
	SYNCDELAY;  
	EP6BCL=len;	// Arms the endpoint for transmission
}

//////////////////////////////////////////////////////////////////////
// Main program
//////////////////////////////////////////////////////////////////////

void
main(void) {

	OEA = 0x03;	// Enable PA0 and PA1 outputs
	initialize();	// Initialize USB
    
	PA0 = 1;	// Turn off LEDs..
	PA1 = 1;

	for (;;) {
		if ( !(EP2CS & bmEPEMPTY) )
			accept_cmd();	// Have data in EP2

		if ( !(EP6CS & bmEPFULL) )
			send_state();	// EP6 is not full
	}
}

// End ezusb.c
