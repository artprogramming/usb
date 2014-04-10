#include "printf.h"

extern void serial_init(void);

int main(void)
{
	serial_init();

	printf("Welcome to the usb world!\n");

	while (1);

	return 0;
}
