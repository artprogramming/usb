#include <at89x52.h>
#include <stdint.h>
#include "d12.h"
#include "printf.h"
#include "delay.h"
#include "usb.h"

void usb_isr(void) __interrupt 0
{
	uint16_t status = d12_read_interrupt_register();

	switch (status) {
	case D12_INT_EP0_OUT:
		usb_ep0_out();
		break;
	case D12_INT_EP0_IN:
		usb_ep0_in();
		break;
	case D12_INT_EP1_OUT:
		usb_ep1_out();
		break;
	case D12_INT_EP1_IN:
		usb_ep1_in();
		break;
	case D12_INT_EP2_OUT:
		usb_ep2_out();
		break;
	case D12_INT_EP2_IN:
		usb_ep2_in();
		break;
	case D12_INT_BUS_RESET:
		usb_bus_reset();
		break;
	case D12_INT_SUSPEND:
		usb_suspend();
		break;
	case D12_INT_DMA_EOT:
		usb_dma_eot();
		break;
	default:
		break;
	}
}

int usb_init(void)
{
	uint16_t chip_id;

	chip_id = d12_read_id();
	if (chip_id != D12_CHIP_ID) {
		printf("d12 is not working!\n");
		return -1;
	}

	d12_disconnect();
	delayms(100);
	d12_init();

	return 0;
}

void usb_ep0_out(void)
{
	uint8_t status;

	printf("usb_ep0_out\n");

	status = d12_read_last_transaction(D12_EP0_OUT);
}

void usb_ep0_in(void)
{
	printf("usb_ep0_in\n");
}

void usb_ep1_out(void)
{
	printf("usb_ep1_out\n");
}

void usb_ep1_in(void)
{
	printf("usb_ep1_in\n");
}

void usb_ep2_out(void)
{
	printf("usb_ep2_out\n");
}

void usb_ep2_in(void)
{
	printf("usb_ep2_in\n");
}

void usb_bus_reset(void)
{
	printf("usb_bus_reset\n");
}

void usb_suspend(void)
{
	printf("usb_suspend\n");
}

void usb_dma_eot(void)
{
	printf("usb_dma_eot\n");
}
