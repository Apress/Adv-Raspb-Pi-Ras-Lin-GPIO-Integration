/********************************************
 * spiloop.c − Example loop test
 * Connect MOSI (GPIO 10) to MISO (GPIO 9)
 ********************************************/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

static void
errxit(const char *msg) {
	perror(msg);
	exit(1);
}

int
main(int argc, char ** argv) {
	static uint8_t tx[] = {0x12, 0x23, 0x45, 0x67};
	static uint8_t rx[] = {0xFF, 0xFF, 0xFF, 0xFF};
	struct spi_ioc_transfer ioc = {
		.tx_buf = (unsigned long) tx,
		.rx_buf = (unsigned long) rx,
		.len = 4,
		.speed_hz = 100000,
		.delay_usecs = 10,
		.bits_per_word = 8,
		.cs_change = 1
	};
	uint8_t mode = SPI_MODE_0;
	int rc, fd=-1;

	fd = open("/dev/spidev0.0",O_RDWR);
	if ( fd < 0 )
		errxit("Opening SPI device.");

	rc = ioctl(fd,SPI_IOC_WR_MODE,&mode);
	if ( rc < 0 )
		errxit("ioctl (2) setting SPI mode.");

	rc = ioctl(fd,SPI_IOC_WR_BITS_PER_WORD,&ioc.bits_per_word);
	if ( rc < 0 )
		errxit("ioctl (2) setting SPI bits perword.");

	rc = ioctl(fd,SPI_IOC_MESSAGE(1),&ioc);
	if ( rc < 0 )
		errxit("ioctl (2) for SPI I/O");
	close(fd);

	printf("rc=%d %02X %02X %02X %02X\n",
		rc, rx[0], rx[1], rx[2], rx[3]);
	return 0;
}
