/* Demonstration program gp.c
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
#include <assert.h>

typedef enum IO {   // GPIO Input or Output:
	Input=0,    // GPIO is to become an input pin
	Output=1,   // GPIO is to become an output pin
	Alt0=4,
	Alt1=5,
	Alt2=6,
	Alt3=7,
	Alt4=3,
	Alt5=2
} IO;

typedef enum Pull { // GPIO Input Pullup resistor:
	None,       // No pull up or down resistor
	Up,         // Activate pullup resistor
	Down        // Activate pulldown resistor
} Pull;

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

void
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
	case None :
		pmask = 0;                  // No pullup/down
		break;
	case Up :
		pmask = 0b10;               // Pullup resistor
		break;
	case Down :
		pmask = 0b01;               // Pulldown resistor
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

void *
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

int
mailbox_unmap(uint32_v *addr,size_t bytes) {
	return munmap((caddr_t)addr,bytes);
}

//////////////////////////////////////////////////////////////////////
// Determine peripheral base address
//////////////////////////////////////////////////////////////////////

uint32_t
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

//////////////////////////////////////////////////////////////////////
// Display command usage info:
//////////////////////////////////////////////////////////////////////

static void
usage(const char *cmd) {

	printf("Usage: %s -g gpio { input_opts | output_opts | -a | drive_opts} [-v]\n"
		"where:\n"
		"\t-g gpio\tGPIO number to operate on\n"
		"\t-A n\tSet alternate function n\n"
		"\t-a\tQuery alt function\n"
		"\t-q\tQuery drive, slew and hysteresis\n"
		"\t-v\tVerbose messages\n"
		"\n"
		"Input options:\n"
		"\t-i n\tSelects input mode, reading for n seconds\n"
		"\t-I\tInput mode, but performing one read only\n"
		"\t-u\tSelects pull-up resistor\n"
		"\t-d\tSelects pull-down resistor\n"
		"\t-n\tSelects no pull-up/down resistor\n"
		"\n"
		"Output options:\n"
		"\t-o n\tWrite 0 or 1 to gpio output\n"
		"\t-b n\tBlink for n seconds\n"
		"\n"
		"Drive Options:\n"
		"\t-D n\tSet drive level to 0-7\n"
		"\t-S\tEnable slew rate limiting\n"
		"\t-H\tEnable hysteresis\n"
		,cmd);
}

//////////////////////////////////////////////////////////////////////
// Main program:
//////////////////////////////////////////////////////////////////////

int
main(int argc,char **argv) {
	static char options[] = "hg:i:Iudnvo:aA:D:H:S:qb:";
	bool opt_verbose = false;
	int opt_gpio = -1;
	int opt_input = -1;
	int opt_output = -1;
	int opt_blink = -1;
	bool opt_altq = false;
	int opt_alt = -1;
	int opt_Drive = -1;
	int opt_Hysteresis = -1, opt_Slew = -1;
	bool opt_query = false;
	Pull opt_pull = Up;
	int oc, rc;

	while ( (oc = getopt(argc,argv,options)) != -1 ) {
		switch ( oc ) {
		case 'g':
			opt_gpio = atoi(optarg);
			if ( opt_gpio <= 0 ) {
				fprintf(stderr,"Invalid gpio: -g %s\n",
					optarg);
				exit(1);
			}
			break;
		case 'i':
			if ( optarg )
				opt_input = atoi(optarg);
			else	opt_input = 0;
			break;
		case 'I':
			opt_input = 0;
			break;
		case 'o':
			opt_output = atoi(optarg);
			break;
		case 'b':
			opt_blink = atoi(optarg);
			break;
		case 'u':
			opt_pull = Up;
			break;
		case 'd':
			opt_pull = Down;
			break;
		case 'n':
			opt_pull = None;
			break;
		case 'a':
			opt_altq = true;
			break;
		case 'A':
			opt_alt = atoi(optarg);
			if ( opt_alt < 0 || opt_alt > 5 ) {
				fprintf(stderr,"Must be 0-5: -A %s\n",optarg);
				exit(1);
			}
			break;
		case 'D':
			opt_Drive = atoi(optarg);
			if ( opt_Drive < 0 || opt_Drive > 7 ) {
				fprintf(stderr,"Invalid range: -D %s\n",optarg);
				exit(1);
			}
			break;
		case 'H':
			opt_Hysteresis = !!atoi(optarg);
			break;
		case 'S':
			opt_Slew = !!atoi(optarg);
			break;
		case 'q':
			opt_query = true;
			break;
		case 'v':
			opt_verbose = true;
			break;
		case 'h':
			usage(argv[0]);
			exit(0);
		default:
			usage(argv[0]);
			exit(1);
		}		
	}

	if ( opt_gpio < 0 ) {
		usage(argv[0]);
		exit(1);
	}

	uint32_t peri_base = peripheral_base();
	uint32_t page_size = sysconf(_SC_PAGESIZE);

        ugpio = (uint32_v *)mailbox_map(peri_base+GPIO_BASE_OFFSET,page_size);
	upads = (uint32_v *)mailbox_map(peri_base+PADS_BASE_OFFSET,page_size);

	if ( !ugpio ) {
		fprintf(stderr,
			"Failed to mmap peripherals: Need root?\n");
		exit(2);
	}

	if ( opt_verbose )
		printf("gpio_peri_base = %08X\n",peri_base);

	if ( opt_input >= 0 ) {
		gpio_configure_io(opt_gpio,Input);
		gpio_configure_pullup(opt_gpio,opt_pull);
	
		time_t t0 = time(0);
		int gbit = 2, nbit;

		do	{
			nbit = gpio_read(opt_gpio);
			if ( nbit != gbit )
				printf("GPIO = %d\n",gbit = nbit);
		} while ( time(0) - t0 < opt_input );
	}

	if ( opt_output >= 0 ) {
		gpio_configure_io(opt_gpio,Output);
		gpio_write(opt_gpio,opt_output);		
		if ( opt_verbose )
			printf("Wrote %d to gpio %d\n",opt_output,opt_gpio);
	}

	if ( opt_blink > 0 ) {
		time_t t0 = time(0);
		int v = opt_output & 1;

		if ( opt_output < 0 )
			gpio_configure_io(opt_gpio,Output);

		do	{
			gpio_write(opt_gpio,!!v);
			if ( opt_verbose )
				printf("GPIO %d -> %d\n",opt_gpio,!!v);
			v ^= 1;
			sleep(1);
		} while ( time(0) - t0 < opt_blink );
	}

	if ( opt_alt >= 0 ) {
		static IO alts[] = { Alt0, Alt1, Alt2, Alt3, Alt4, Alt5 };
		IO io = alts[opt_alt];

		rc = gpio_configure_io(opt_gpio,io);
		assert(!rc);
	}

	if ( opt_altq ) {
		IO io;
		const char *alt;

		rc = gpio_alt_function(opt_gpio,&io);
		assert(!rc);

		switch ( io ) {
		case Input:
			alt = "Input";
			break;
		case Output:
			alt = "Output";
			break;
		case Alt0:
			alt = "ALT0";
			break;
		case Alt1:
			alt = "ALT1";
			break;
		case Alt2:
			alt = "ALT2";
			break;
		case Alt3:
			alt = "ALT3";
			break;
		case Alt4:
			alt = "ALT4";
			break;
		case Alt5:
			alt = "ALT5";
			break;
		}

		printf("GPIO %d is in %s mode.\n",opt_gpio,alt);
	}

	if ( opt_Drive >= 0 ) {
		bool slew_limited = !!(opt_Slew >= 0 ? opt_Slew : 0);
		bool hysteresis = opt_Hysteresis >= 0 ? opt_Hysteresis : 0;

		gpio_set_drive_strength(opt_gpio,
			slew_limited,
			hysteresis,
			opt_Drive);
		if ( opt_verbose )
			printf("  Set Drive=%d, slew=%s, hysteresis=%s\n",
				opt_Drive,
				slew_limited ? "true" : "false",
				hysteresis ? "true" : "false");
	}

	if ( opt_query ) {
		bool slew_limited, hysteresis;
		int drive;

		rc = gpio_get_drive_strength(opt_gpio,&slew_limited,&hysteresis,&drive);
		assert(!rc);

		printf("  Got Drive=%d, slew=%s, hysteresis=%s\n",
			drive,
			slew_limited ? "true" : "false",
			hysteresis ? "true" : "false");
	}

	/* Unmap memory */
	mailbox_unmap(ugpio,page_size);
	mailbox_unmap(upads,page_size);

	return 0;
}
