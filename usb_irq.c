#include <at89x52.h>
#include <stdint.h>

#include "d12.h"
#include "printf.h"
#include "delay.h"

void usb_isr(void) __interrupt 0
{
	uint16_t status = d12_read_interrupt_register();

	switch (status) {
	case D12_INT_EP0_OUT:
		break;
	case D12_INT_EP0_IN:
		break;
	case D12_INT_EP1_OUT:
		break;
	case D12_INT_EP1_IN:
		break;
	case D12_INT_EP2_OUT:
		break;
	case D12_INT_EP2_IN:
		break;
	case D12_INT_BUS_RESET:
		break;
	case D12_INT_SUSPEND:
		break;
	case D12_INT_DMA_EOT:
		break;
	default:
		printf("Unkown interrupt\n");
		break;
	}
}

int usb_init(void)
{
	uint16_t chip_id;

	chip_id = d12_read_chip_id();
	if (chip_id != D12_CHIP_ID) {
		printf("d12 is not working!\n");
		return -1;
	}

	d12_disconnect();
	delayms(100);
	d12_init();

	return 0;
}