#include "printf.h"
#include "d12.h"
#include "delay.h"
#include <stdint.h>

extern void usb_irq(void) __interrupt 0;
extern int  usb_init(void);
extern void serial_init(void);

extern void cdc_init(void);

void main(void)
{
	uint8_t aa = 'a';
	serial_init();
	usb_init();
	cdc_init();

	printf("Welcome to the usb world!\n");

	while (1);
	delayms(3000);
	while (1) {
		printf("write data\n");
		d12_write_buffer(D12_EPINDEX_2_IN, (uint8_t *)&aa, 1);
		//delayms(500);
	}
}
