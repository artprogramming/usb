#include <at89x52.h>
#include <stdint.h>

#include "delay.h"

uint8_t keyscan(void)
{
	P1 = 0xff;

	if (~P1) {
		delayms(5);
		if (~P1) {
			return ~P1;
		}
	}

	return 0x00;
}
