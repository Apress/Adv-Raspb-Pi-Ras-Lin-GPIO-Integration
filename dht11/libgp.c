/* Direct GPIO routines libgp.c
 * Warren W. Gay ve3wwg
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "libgp.h"

typedef uint32_t volatile uint32_v;

uint32_v *ugpio = 0;
uint32_v *upads = 0;

#define BCM2708_PERI_BASE    	0x3F000000 	// Assumed for RPi2
#define GPIO_BASE_OFFSET	0x200000	// 0x7E20_0000
#define PADS_BASE_OFFSET        0x100000        // 0x7E10_0000

//////////////////////////////////////////////////////////////////////
// GPIO Macros
//////////////////////////////////////////////////////////////////////

#define GPIOOFF(o)	(((o)-0x7E000000-GPIO_BASE_OFFSET)/sizeof(uint32_t))
#define GPIOREG(o)	(ugpio+GPIOOFF(o))
#define GPIOREG2(o,wo)	(ugpio+GPIOOFF(o)+(wo))

#define GPIO_GPFSEL0	0x7E200000 
#define GPIO_GPSET0	0x7E20001C
#define GPIO_GPCLR0	0x7E200028 
#define GPIO_GPLEV0     0x7E200034 

#define GPIO_GPEDS0 	0x7E200040 
#define GPIO_GPREN0	0x7E20004C 
#define GPIO_GPFEN0     0x7E200058 
#define GPIO_GPHEN0     0x7E200064 
#define GPIO_GPLEN0     0x7E200070 

#define GPIO_GPAREN0	0x7E20007C 
#define GPIO_GPAFEN0 	0x7E200088 

#define GPIO_GPPUD	0x7E200094
#define GPIO_GPUDCLK0	0x7E200098
#define GPIO_GPUDCLK1	0x7E200098

#define PADSOFF(o)	(((o)-0x7E000000-PADS_BASE_OFFSET)/sizeof(uint32_t))
#define PADSREG(o,x)	(upads+PADSOFF(o)+x)

#define GPIO_PADS00_27	0x7E10002C
#define GPIO_PADS28_45	0x7E100030 

//////////////////////////////////////////////////////////////////////
// Internal helper functions
//////////////////////////////////////////////////////////////////////

static uint32_v *
set_gpio10(int gpio,int *shift,uint32_t base) {
	uint32_t offset = gpio / 10;
	*shift = gpio % 10 * 3;
	return GPIOREG2(base,offset);
}

static uint32_v *
set_gpio32(int gpio,int *shift,uint32_t base) {
    uint32_t offset = gpio / 32;
    *shift = gpio % 32;
    return GPIOREG2(base,offset);
}

//////////////////////////////////////////////////////////////////////
// Configure a GPIO mode
//////////////////////////////////////////////////////////////////////

int
gpio_configure_io(int gpio,IO io) {
	int shift;
	int alt = (int)io & 0x07;

	if ( gpio < 0 )
		return EINVAL;          // Invalid parameter
	
	uint32_v *gpiosel = set_gpio10(gpio,&shift,GPIO_GPFSEL0);
	*gpiosel = (*gpiosel & ~(7<<shift)) | (alt<<shift);	
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Return currently configured Alternate function for gpio
//////////////////////////////////////////////////////////////////////

int
gpio_alt_function(int gpio,IO *io) {
	int shift;

	if ( gpio < 0 )
		return EINVAL;          // Invalid parameter

	uint32_v *gpiosel = set_gpio10(gpio,&shift,GPIO_GPFSEL0);
	uint32_t r = (*gpiosel >> shift) & 7;

	*io = (IO)r;
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Return the drive strength of a given gpio pin
//////////////////////////////////////////////////////////////////////

int
gpio_get_drive_strength(int gpio,bool *slew_limited,bool *hysteresis,int *drive) {

	if ( gpio < 0 || gpio > 53 )
	        return EINVAL;          // Invalid parameter

	uint32_t padx = gpio / 28;
	uint32_v *padreg = PADSREG(GPIO_PADS00_27,padx);

	*drive = *padreg & 7;
	*hysteresis = (*padreg & 0x0008) ? true : false;
	*slew_limited = (*padreg & 0x0010) ? true : false;
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Set the GPIO drive strength
//////////////////////////////////////////////////////////////////////

int
gpio_set_drive_strength(int gpio,bool slew_limited,bool hysteresis,int drive) {

	if ( gpio < 0 || gpio > 53 )
		return EINVAL;          // Invalid parameter
	
	uint32_t padx = gpio / 28;
	uint32_v *padreg = PADSREG(GPIO_PADS00_27,padx);
	
	uint32_t config = 0x5A000000;
	if ( slew_limited )
		config |= 1 << 4;
	if ( hysteresis )
		config |= 1 << 3;
	config |= drive & 7;
	
	*padreg = config;
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Perform small delay
//////////////////////////////////////////////////////////////////////

static void
gpio_delay() {
	for ( int i=0; i<150; i++ )
		asm volatile("nop");
}

//////////////////////////////////////////////////////////////////////
// Configure a GPIO pin to have None/Pullup/Pulldown resistor
//////////////////////////////////////////////////////////////////////

int
gpio_configure_pullup(int gpio,Pull pull) {
    
	if ( gpio < 0 || gpio >= 32 )
		return EINVAL;              // Invalid parameter

	uint32_t mask = 1 << gpio;      // GPIOs 0 to 31 only
	uint32_t pmask;

	switch ( pull ) {
	case Up :
		pmask = 0b10;               // Pullup resistor
		break;
	case Down :
		pmask = 0b01;               // Pulldown resistor
		break;
	case None :
	default:
		pmask = 0;                  // No pullup/down
		break;
	};

	uint32_v *GPPUD = GPIOREG(GPIO_GPPUD);
	uint32_v *GPUDCLK0 = GPIOREG(GPIO_GPUDCLK0);

	*GPPUD = pmask;                  // Select pullup setting
	gpio_delay();
	*GPUDCLK0 = mask;                // Set the GPIO of interest
	gpio_delay();
	*GPPUD = 0;                      // Reset pmask
	gpio_delay();
	*GPUDCLK0 = 0;                   // Set the GPIO of interest
	gpio_delay();

	return 0;
}

//////////////////////////////////////////////////////////////////////
// Read a GPIO port (single bit)
//////////////////////////////////////////////////////////////////////

int
gpio_read(int gpio) {
    int shift;
    
    if ( gpio < 0 || gpio > 31 )
        return EINVAL;

    uint32_v *gpiolev = set_gpio32(gpio,&shift,GPIO_GPLEV0);

    return !!(*gpiolev & (1<<shift));
}

//////////////////////////////////////////////////////////////////////
// Write a GPIO bit
//////////////////////////////////////////////////////////////////////

int
gpio_write(int gpio,int bit) {
	int shift;
    
	if ( gpio < 0 || gpio > 31 )
		return EINVAL;

	if ( bit ) {
		uint32_v *gpiop = set_gpio32(gpio,&shift,GPIO_GPSET0);
	        *gpiop = 1u << shift;
	} else	{
		uint32_v *gpiop = set_gpio32(gpio,&shift,GPIO_GPCLR0);
		*gpiop = 1u << shift;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Read/Write all GPIO bits at once
//////////////////////////////////////////////////////////////////////

uint32_t
gpio_read32() {
	uint32_v *gpiolev = GPIOREG(GPIO_GPLEV0);

	return *gpiolev;
}

//////////////////////////////////////////////////////////////////////
// Map memory for peripheral register access
//////////////////////////////////////////////////////////////////////

static void *
mailbox_map(off_t offset,size_t bytes) {
	int fd;

	fd = open("/dev/mem",O_RDWR|O_SYNC);
	if ( fd < 0 )
		return 0;		// Failed (see errno)
	
	void *map = (char *) mmap(
		NULL,               	// Any address
		bytes,              	// # of bytes
		PROT_READ|PROT_WRITE,
		MAP_SHARED,         	// Shared
		fd,                 	// /dev/mem
		offset
	);
	
	if ( (long)map == -1L ) {
		int er = errno;		// Save errno
		close(fd);
		errno = er;		// Restore errno
		return 0;
	}
	
	close(fd);
	return map;
}

//////////////////////////////////////////////////////////////////////
// Unmap memory
//////////////////////////////////////////////////////////////////////

static int
mailbox_unmap(uint32_v *addr,size_t bytes) {
	return munmap((caddr_t)addr,bytes);
}

//////////////////////////////////////////////////////////////////////
// Determine peripheral base address
//////////////////////////////////////////////////////////////////////

static uint32_t
peripheral_base() {
	static uint32_t pbase = 0;
	int fd, rc;
	unsigned char buf[8];
	
	fd = open("/proc/device-tree/soc/ranges",O_RDONLY);
	if ( fd >= 0 ) {
		rc = read(fd,buf,sizeof buf);
		assert(rc==sizeof buf);
		close(fd);
		pbase = buf[4] << 24 | buf[5] << 16 | buf[6] << 8 | buf[7] << 0;
	} else	{
		// Punt: Assume RPi2
		pbase = BCM2708_PERI_BASE;
	}
	
	return pbase;
}

/*
 * Returns true, if the direct GPIO registers are opened:
 */
bool
gpio_open() {
	uint32_t peri_base = peripheral_base();
	uint32_t page_size = sysconf(_SC_PAGESIZE);

	if ( !peri_base )
		return false;

        ugpio = (uint32_v *)mailbox_map(peri_base+GPIO_BASE_OFFSET,page_size);
	upads = (uint32_v *)mailbox_map(peri_base+PADS_BASE_OFFSET,page_size);

	return ugpio != NULL && upads != NULL;
}

/*
 * Close (unmap) direct access to GPIO:
 */
void
gpio_close() {
	uint32_t page_size = sysconf(_SC_PAGESIZE);

	/* Unmap memory */
	if ( ugpio ) {
		mailbox_unmap(ugpio,page_size);
		ugpio = NULL;
	}
	if ( upads ) {
		mailbox_unmap(upads,page_size);
		upads = NULL;
	}
}

/* end libgp.c */
