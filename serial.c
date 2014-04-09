#include <at89x52.h>
#include "configs.h"

void serial_init(void)
{
	TMOD &= 0x0F;
	TMOD |= 0x02 << 4;
	TH1 = 256 - OSC_FREQ / (BAUD_RATE * 32 * 12);
	TL1 = 256 - OSC_FREQ / (BAUD_RATE * 32 * 12);
	TR1 = 1;

	SCON = 0x01 << 6;
}
