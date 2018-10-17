/*********************************************************************
 * evinput.c : Event driven GPIO input
 *
 * ./evinput gpio#
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <sys/poll.h>

static int gpio_inpin = 17;	/* GPIO input pin */
static int is_signaled = 0;	/* Exit program when signaled */

typedef enum {
	gp_export=0,	/* /sys/class/gpio/export */
	gp_unexport,	/* /sys/class/gpio/unexport */
	gp_direction,	/* /sys/class/gpio%d/direction */
	gp_edge,	/* /sys/class/gpio%d/edge */
	gp_value	/* /sys/class/gpio%d/value */
} gpio_path_t;

typedef enum {
	none=0,		/* No edge detection */
	rising,		/* Rising edge */
	falling,	/* Falling edge */
	both		/* Rising and falling edge */
} gpio_edge_t;

/*
 * Internal : Create a pathname for type in buf.
 */
static const char *
gpio_setpath(int pin,gpio_path_t type,char *buf,unsigned bufsiz) {
	static const char *paths[] = {
		"export", "unexport", "gpio%d/direction",
		"gpio%d/edge", "gpio%d/value" };
	int slen;

	strncpy(buf,"/sys/class/gpio/",bufsiz);
	bufsiz -= (slen = strlen(buf));
	snprintf(buf+slen,bufsiz,paths[type],pin);
	return buf;
}

/*
 * Open path, else report fail:
 */
static FILE *
gpio_open(const char *path,const char *mode) {
	FILE *f = fopen(path,mode);

	if ( !f ) {
		fprintf(stderr,
			"%s: opening %s\n",
			strerror(errno),
			path);
		exit(1);
	}
	return f;
}

/*
 * Open /sys/class/gpio%d/value for edge detection :
 */
static FILE *
gpio_open_edge(int pin,gpio_edge_t edge) {
	static char *edge_str[] = {
		"none", "rising", "falling", "both"
	};
	char buf[128];
	FILE *f;

	/* Export pin : /sys/class/gpio/export */
	gpio_setpath(pin,gp_export,buf,sizeof buf);
	f = gpio_open(buf,"w");
	fprintf(f,"%d\n",pin);
	fclose(f);

	/* Direction :	/sys/class/gpio%d/direction */
	gpio_setpath(pin,gp_direction,buf,sizeof buf);
	f = gpio_open(buf,"w");
	assert(f);
	fprintf(f,"in\n");
	fclose(f);

	/* Edge :	/sys/class/gpio%d/edge */
	gpio_setpath(pin,gp_edge,buf,sizeof buf);
	f = gpio_open(buf,"w");
	assert(f);
	fprintf(f,"%s\n",edge_str[edge]);
	fclose(f);

	/* Value :	/sys/class/gpio%d/value */
	gpio_setpath(pin,gp_value,buf,sizeof buf);
	f = gpio_open(buf,"rw");
	return f;
}

/*
 * Close (unexport) GPIO pin :
 */
static void
gpio_close(int pin) {
	char buf[128];
	FILE *f;

	/* Unexport :	/sys/class/gpio/unexport */
	gpio_setpath(pin,gp_unexport,buf,sizeof buf);
	f = fopen(buf,"w");
	assert(f);
	fprintf(f,"%d\n",pin);
	fclose(f);
}

/*
 * This routine will block until the open GPIO pin has changed
 * value. This pin should be connected to the MCP23017 /INTA
 * pin.
 */
static int
gpio_poll(int fd) {
	struct pollfd polls[1];
	char buf[32];
	int rc, n;
	
	polls[0].fd = fd;		/* /sys/class/gpio17/value */
	polls[0].events = POLLPRI;	/* Events */
	
	do	{
		rc = poll(polls,1,-1);	/* Block */
		if ( is_signaled )
			return -1;	/* Exit if ^C received */
	} while ( rc < 0 && errno == EINTR );
	
	assert(rc > 0);

	lseek(fd,0,SEEK_SET);
	n = read(fd,buf,sizeof buf);	/* Read value */
	assert(n>0);
	buf[n] = 0;

	rc = sscanf(buf,"%d",&n);
	assert(rc==1);
	return n;			/* Return value */
}

/*
 * Signal handler to quit the program :
 */
static void
sigint_handler(int signo) {
	is_signaled = 1;		/* Signal to exit program */
}

static void
usage(const char *cmd) {
	printf("Usage: %s -g gpio [-f] [-r] [-b]\n"
		"where:\n"
		"\t-f\tdetect rising edges\n"
		"\t-r\tdetect falling edges\n"
		"\t-b\tdetect both edges\n"
		"\nDefaults are: -g17 -b\n",
		cmd);
}

/*
 * Main program :
 */
int
main(int argc,char **argv) {
	static char options[] = "hrfbg:";
	gpio_edge_t opt_edge = both;
	FILE *gf = NULL;
	int fd, v, oc;

	while ( (oc = getopt(argc,argv,options)) != -1 ) {
		switch ( oc ) {
		case 'h':
			usage(argv[0]);
			exit(0);
		case 'g':
			gpio_inpin = atoi(optarg);
			break;
		case 'r':
			opt_edge = rising;
			break;
		case 'f':
			opt_edge = falling;
			break;
		case 'b':
			opt_edge = both;
			break;
		default:
			usage(argv[0]);
			exit(1);
		}
	}

	signal(SIGINT,sigint_handler);		/* Trap on SIGINT */
	gf = gpio_open_edge(gpio_inpin,opt_edge); /* GPIO input */
	fd = fileno(gf);			/* Extract file descriptor */

	puts("Monitoring for GPIO input changes:\n");

	while ( !is_signaled
	     && (v = gpio_poll(fd)) >= 0 ) {	/* Block until input changes */
		printf("GPIO %d changed: %d\n",gpio_inpin,v);
	}

	putchar('\n');
	fclose(gf);
	gpio_close(gpio_inpin);			/* Unexport gpio */
	return 0;
}

/* End */
