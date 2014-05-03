#include <at89x52.h>
#include <stdint.h>

#include "d12.h"
#include "printf.h"
#include "delay.h"
#include "ch9.h"
#include "usb.h"
#include "hid.h"
#include "types.h"

#define EP_0_IN_LEN 16

extern int16_t usb_get_device_descriptor(const void **p);
extern int16_t usb_get_configuration_descriptor(const void **p);
extern int16_t usb_get_report_descriptor(uint8_t interface, const void **p);

void usb_irq(void) __interrupt 0
{
	uint16_t status = d12_read_interrupt_register();

	switch (status) {
	case D12_INT_EP0_OUT:
		handle_ep0_out();
		break;
	case D12_INT_EP0_IN:
		handle_ep0_in();
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

void handle_ep0_out(void)
{
	uint8_t status = d12_read_last_transaction_status(D12_EPINDEX_0_OUT);

	if (status & D12_LAST_TRANS_STATUS_SETUP_PACKET) {
		usb_setup_request();
	}
}

static uint8_t remaining;
static uint8_t *buffer;

static uint8_t need_zlp;
void handle_ep0_in(void)
{
	d12_read_last_transaction_status(D12_EPINDEX_0_IN);

	if (remaining) {
		usb_control_transfer(D12_EPINDEX_0_IN, buffer, MIN(remaining, EP_0_IN_LEN));
	} else if (need_zlp) {
		usb_send_zero_length_packet();
		need_zlp = 0;
	}
}

void usb_send_descriptor(void *desc, uint8_t size)
{
	buffer = (uint8_t *)desc;
	remaining = size;

	usb_control_transfer(D12_EPINDEX_0_IN, buffer, MIN(remaining, EP_0_IN_LEN));
}

void usb_control_transfer(uint8_t ep, uint8_t *buf, uint8_t size)
{
	buffer += size;
	remaining -= size;

	if (remaining == 0 && size == EP_0_IN_LEN) {
		need_zlp = 1;
	}

	d12_write_buffer(ep, buf, size);
}

void usb_send_zero_length_packet(void)
{
	d12_write_buffer(D12_EPINDEX_0_IN, NULL, 0);
}

void usb_setup_request(void)
{
	uint8_t buf[8];
	struct setup_packet *setup = (struct setup_packet *)buf;

	d12_read_setup_packet(buf, 8);
	
	printf("bmRequestType %x bRequest %x wValue %x wIndex %x wLength %x\n",
		setup->REQUEST.bmRequestType, setup->bRequest,
		setup->wValue, setup->wIndex, setup->wLength);

	switch (setup->REQUEST.type) {
	case TYPE_STANDARD:
		usb_standard_request(setup);
		break;
	case TYPE_CLASS:
		break;
	case TYPE_VENDOR:
		break;
	default:
		break;
	}
}

void usb_standard_request(struct setup_packet *setup)
{
	switch (setup->bRequest) {
	case CLEAR_FEATURE:
		break;
	case GET_CONFIGURATION:
		break;
	case GET_DESCRIPTOR:
		usb_get_descriptor(setup);
		break;
	case GET_INTERFACE:
		break;
	case GET_STATUS:
		break;
	case SET_ADDRESS:
		usb_set_address(setup->wValue & 0x00ff);
		break;
	case SET_CONFIGURATION:
		usb_set_configuration(setup->wValue & 0x00ff);
		break;
	case SET_DESCRIPTOR:
		break;
	case SET_FEATURE:
		break;
	case SYNCH_FRAME:
		break;
	default:
		break;
	}
}

void usb_get_descriptor(struct setup_packet *setup)
{
	uint8_t type, index;
	uint16_t len;
	void *desc;

	type = setup->wValue >> 8;
	index = (uint8_t)setup->wValue & 0x00ff;

	switch (type) {
	case DESC_DEVICE:
		len = usb_get_device_descriptor(&desc);
		usb_send_descriptor(desc, MIN(len, (uint8_t)setup->wLength));
		break;
	case DESC_CONFIGURATION:
		len = usb_get_configuration_descriptor(&desc);
		usb_send_descriptor(desc, MIN(len, (uint8_t)setup->wLength));
		break;
	case DESC_STRING:
		break;
	case DESC_HID:
		break;
	case DESC_REPORT:
		len = usb_get_report_descriptor(0, &desc);
		usb_send_descriptor(desc, MIN(len, (uint8_t)setup->wLength));
		break;
	default:
		break;
	}
}

void usb_set_address(uint8_t addr)
{
	d12_set_address_enable(addr);
	usb_send_zero_length_packet();
}

void usb_set_configuration(uint8_t config)
{
	d12_set_endpoint_enable(config);
	usb_send_zero_length_packet();
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
