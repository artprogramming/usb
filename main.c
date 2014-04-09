#include "printf.h"

extern void serial_init(void);
extern int  usb_init(void);

extern void usb_isr(void) __interrupt 0;

int main(void)
{
	serial_init();

	if (usb_init()) {
		printf("usb initialize error!\n");
		return -1;
	}

	printf("Welcome to the usb world!\n");
	while (1);

	return 0;
}
