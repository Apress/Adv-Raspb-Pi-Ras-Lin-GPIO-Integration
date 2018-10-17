//////////////////////////////////////////////////////////////////////
// libgp.h -- Direct access GPIO functions
// Date: Sun Jul 22 04:50:53 2018   (C) ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#ifndef LIBGP_H
#define LIBGP_H

#include <stdint.h>
#include <stdbool.h>

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

bool gpio_open();
void gpio_close();

int gpio_configure_io(int gpio,IO io);
int gpio_alt_function(int gpio,IO *io);
int gpio_get_drive_strength(int gpio,bool *slew_limited,bool *hysteresis,int *drive);
int gpio_set_drive_strength(int gpio,bool slew_limited,bool hysteresis,int drive);
int gpio_configure_pullup(int gpio,Pull pull);

int gpio_read(int gpio);
int gpio_write(int gpio,int bit);

uint32_t gpio_read32();

#endif // LIBGP_H

// End libgp.h
