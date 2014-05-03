#include "printf.h"
#include "key.h"
#include "delay.h"

extern void usb_irq(void) __interrupt 0;
extern int  usb_init(void);
extern void serial_init(void);

extern int reportkey(uint8_t key);

void main(void)
{
	uint8_t key;

	serial_init();
	usb_init();

	printf("Welcome to the usb world!\n");

	delayms(3000);
	while (1) {
		key = keyscan();
		//printf("%x\n", key);
		reportkey(key);
	}
}
