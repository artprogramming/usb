#include "printf.h"

extern void usb_isr(void) __interrupt 0;
extern int  usb_init(void);
extern void serial_init(void);

void main(void)
{
	serial_init();
	usb_init();

	printf("Welcome to the usb world!\n");

	while (1);
}
