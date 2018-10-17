//////////////////////////////////////////////////////////////////////
// controlusb.cpp -- Communicate with EZ-USB using Bulk EP
// Date: Fri Mar 30 09:54:18 2018   (C) ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <termios.h>
#include <assert.h>

#include <libusb-1.0/libusb.h>

static libusb_device **usb_devs = nullptr;
static ssize_t n_devices = 0;


class Tty {
	struct termios	saved,
			current;
	bool		rawf = false;
public:	Tty();
	~Tty();
	void raw_mode(bool enablef=true);
	int getc(int timeout_ms);
};

Tty::Tty() {
	int rc = tcgetattr(0,&saved);		// Get current serial attributes
	assert(rc != -1);
	current = saved;
}

Tty::~Tty() {
	if ( rawf ) {
		tcsetattr(0,TCSADRAIN,&saved);	// Restore original tty settings
		rawf = true;
	}
}

void
Tty::raw_mode(bool enablef) {
	if ( enablef ) {
		cfmakeraw(&current);			// Modify for raw I/O
		current.c_oflag |= ONLCR | OPOST;	// Translate LF as CRLF
		tcsetattr(0,TCSADRAIN,&current);	// Set stdin for raw I/O
		rawf = true;
	} else	{
		tcsetattr(0,TCSADRAIN,&saved);		// Restore tty
	}
}

int
Tty::getc(int timeout_ms) {
	struct pollfd polls[1];
	char ch;
	int rc;
	
	polls[0].fd	= 0;			// Standard input
	polls[0].events	= POLLIN;
	polls[0].revents = 0;
	rc = ::poll(&polls[0],1,timeout_ms);
	assert(rc >= 0);
	if ( rc == 0 )
		return -1;			// No data
	rc = ::read(0,&ch,1);
	if ( rc <= 0 )
		return -1;
	return ch;				// Return char
}

//////////////////////////////////////////////////////////////////////
// Locate USB device by vendor and product ID
//////////////////////////////////////////////////////////////////////

static libusb_device_handle *
find_usb_device(unsigned id_vendor,unsigned id_product) {
	
	if ( !usb_devs ) {
		libusb_init(nullptr);		// Initialize
		// Fetch list of devices
		n_devices = libusb_get_device_list(nullptr,&usb_devs);
		if ( n_devices < 0 )
			return nullptr;		// Failed
	}
	return libusb_open_device_with_vid_pid(
		nullptr,id_vendor,id_product);
}

//////////////////////////////////////////////////////////////////////
// Close usb library
//////////////////////////////////////////////////////////////////////

static void
close_usb() {

	if ( usb_devs ) {
		libusb_free_device_list(usb_devs,1);
		libusb_exit(nullptr);
	}
}

//////////////////////////////////////////////////////////////////////
// Perform a bulk read
//////////////////////////////////////////////////////////////////////

static int
bulk_read(
  libusb_device_handle *hdev,
  unsigned char endpoint,
  void *buffer,
  int buflen,
  unsigned timeout_ms
) {
	unsigned char *bufp = (unsigned char*)buffer;
	int rc, xlen = 0;

	assert(endpoint & 0x80);
	rc = libusb_bulk_transfer(hdev,endpoint,bufp,buflen,&xlen,timeout_ms);
	if ( rc == 0 || rc == LIBUSB_ERROR_TIMEOUT )
		return xlen;
	return -int(rc);
}

//////////////////////////////////////////////////////////////////////
// Perform a bulk write
//////////////////////////////////////////////////////////////////////

static int
bulk_write(
  libusb_device_handle *hdev,
  unsigned char endpoint,
  void *buffer,
  int buflen,
  unsigned timeout_ms
) {
	unsigned char *bufp = (unsigned char*)buffer;
	int rc, xlen = 0, total = 0;

	assert(!(endpoint & 0x80));

	for (;;) {
		rc = libusb_bulk_transfer(hdev,endpoint,bufp,buflen,&xlen,timeout_ms);
		if ( rc == 0 || rc == LIBUSB_ERROR_TIMEOUT ) {
			total += xlen;
			bufp += xlen;
			buflen -= xlen;
			if ( buflen <= 0 )
				return total;
		} else	{
			return -int(rc); // Failed
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Main program:
//////////////////////////////////////////////////////////////////////

int
main(int argc,char **argv) {
	Tty tty;
	int rc, ch;
	char buf[513];
	unsigned id_vendor = 0x04b4,
		id_product = 0x8613;
	libusb_device_handle *hdev;
	unsigned state = 0b0011;

	hdev = find_usb_device(id_vendor,id_product);
	if ( !hdev ) {
		fprintf(stderr,
			"Device not found. "
			"Vendor=0x%04X Product=0x%04X\n",
			id_vendor,id_product);
		return 1;
	}

	rc = libusb_claim_interface(hdev,0);
	if ( rc != 0 ) {
		fprintf(stderr,
			"%s: Claiming interface 0.\n",
			libusb_strerror(libusb_error(rc)));
		libusb_close(hdev);
		return 2;
	}

	printf("Interface claimed:\n");

	if ( (rc = libusb_set_interface_alt_setting(hdev,0,1)) != 0 ) {
		fprintf(stderr,"%s: libusb_set_interface_alt_setting(h,0,1)\n",
			libusb_strerror(libusb_error(rc)));
		return 3;
	}

	tty.raw_mode();

	// Main loop:

	for (;;) {
		if ( (ch = tty.getc(500)) == -1 ) {
			// Timed out: Try to read from EP6
			rc = bulk_read(hdev,0x86,buf,512,10/*ms*/);
			if ( rc < 0 ) {
				fprintf(stderr,
					"%s: bulk_read()\n\r",
					libusb_strerror(libusb_error(-rc)));
				break;
			}

			assert(rc < int(sizeof buf));
			buf[rc] = 0;
			printf("Read %d bytes: %s\n\r",rc,buf);
			if ( !isatty(0) )
				break;
		} else	{
			if ( ch == 'q' || ch == 'Q' || ch == 0x04 /*CTRL-D*/ )
				break;
			if ( ch == '0' || ch == '1' ) {
				unsigned mask = 1 << (ch & 1);

				state ^= mask;
				buf[0] = state;
				rc = bulk_write(hdev,0x02,buf,1,10/*ms*/);
				if ( rc < 0 ) {
					fprintf(stderr,
						"%s: write bulk to EP 2\n",
						libusb_strerror(libusb_error(-rc)));
					break;
				}
				printf("Wrote %d bytes: 0x%02X  (state 0x%02X)\n",
					rc,unsigned(buf[0]),state);
			} else	{
				printf("Press q to quit, else 0 or 1 to toggle LED.\n");
			}
		}
	}

	rc = libusb_release_interface(hdev,0);
	assert(!rc);
	libusb_close(hdev);

	close_usb();
	return 0;
}

// End controlusb.cpp
