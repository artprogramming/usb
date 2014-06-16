#include <at89x52.h>

#include "key.h"

extern void usb_irq(void) __interrupt 0;
extern int  usb_init(void);
extern void hid_init(void);
extern void serial_init(void);

extern int  reportkey(uint8_t key);

void main(void)
{
	serial_init();
	hid_init();
	usb_init();

	while (1) {
		reportkey(keyscan());
	}
}
