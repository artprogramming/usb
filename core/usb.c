#include <at89x52.h>
#include <stdint.h>

#include "printf.h"
#include "types.h"
#include "delay.h"
#include "d12.h"
#include "ch9.h"
#include "usb.h"

#define USB_DEBUG	1

static void (*class_request)(struct setup_packet *setup);
static uint8_t ep_0_out_buf[8];

extern int16_t get_descriptor(uint8_t type, uint8_t index, const void **p);

static void handle_ep0_out(void);
static void handle_ep0_in(void);
static void usb_send_descriptor(void *desc, uint8_t size);
static void usb_send_data(uint8_t ep, uint8_t *buf, uint8_t size);
static void usb_setup_request(void);
static void usb_standard_request(struct setup_packet *setup);
static void usb_class_request(struct setup_packet *setup);
static void usb_get_descriptor(struct setup_packet *setup);
static void usb_set_address(uint8_t addr);
static void usb_set_configuration(uint8_t config);
static void usb_get_configuration(void);
static void usb_set_device_state(enum usb_device_state new_state);

void usb_irq(void) __interrupt 0
{
	uint16_t status = d12_read_interrupt_register();

	if (status & D12_INT_EP0_OUT) {
		handle_ep0_out();
	} else if (status & D12_INT_EP0_IN) {
		handle_ep0_in();
	} else if (status & D12_INT_BUS_RESET) {
		usb_set_device_state(USB_STATE_DEFAULT);
	} else if (status & D12_INT_SUSPEND) {
	} else if (status & D12_INT_EP1_OUT) {
		d12_read_last_transaction_status(D12_EPINDEX_1_OUT);
	} else if (status & D12_INT_EP1_IN) {
		d12_read_last_transaction_status(D12_EPINDEX_1_IN);
	} else if (status & D12_INT_EP2_OUT) {
		d12_read_last_transaction_status(D12_EPINDEX_2_OUT);
	} else if (status & D12_INT_EP2_IN) {
		d12_read_last_transaction_status(D12_EPINDEX_2_IN);
	}else {
		printf("Unkown interrupt\n");
	}
}

static void handle_ep0_out(void)
{
	uint8_t status = d12_read_last_transaction_status(D12_EPINDEX_0_OUT);

	if (status & D12_LAST_TRANS_STATUS_SETUP_PACKET) {
		usb_setup_request();
	} else {
		d12_read_buffer(D12_EPINDEX_0_OUT, ep_0_out_buf, 8);
		d12_clear_buffer();
		usb_send_zero_length_packet();
	}
}

static uint8_t remaining;
static uint8_t *buffer;

static uint8_t need_zlp;
static void handle_ep0_in(void)
{
	d12_read_last_transaction_status(D12_EPINDEX_0_IN);

	if (remaining) {
		usb_send_data(D12_EPINDEX_0_IN, buffer, MIN(remaining, EP_0_IN_LEN));
	} else if (need_zlp) {
		usb_send_zero_length_packet();
		need_zlp = 0;
	}
}

static void usb_send_descriptor(void *desc, uint8_t size)
{
	buffer = (uint8_t *)desc;
	remaining = size;

	usb_send_data(D12_EPINDEX_0_IN, buffer, MIN(remaining, EP_0_IN_LEN));
}

static void usb_send_data(uint8_t ep, uint8_t *buf, uint8_t size)
{
	remaining -= size;
	buffer += size;

	if (remaining == 0 && size == EP_0_IN_LEN) {
		need_zlp = 1;
	}

	d12_write_buffer(ep, buf, size);
}

void usb_send_zero_length_packet(void)
{
	d12_write_buffer(D12_EPINDEX_0_IN, NULL, 0);
}

static void usb_setup_request(void)
{
	struct setup_packet *setup = (struct setup_packet *)ep_0_out_buf;

	d12_read_setup_packet(ep_0_out_buf, 8);

#ifdef USB_DEBUG
	printf("bmRequestType %x bRequest %x wValue %x wIndex %x wLength %x\n",
		setup->REQUEST.bmRequestType, setup->bRequest,
		setup->wValue, setup->wIndex, setup->wLength);
#endif

	switch (setup->REQUEST.type) {
	case TYPE_STANDARD:
		usb_standard_request(setup);
		break;
	case TYPE_CLASS:
		usb_class_request(setup);
		break;
	case TYPE_VENDOR:
		break;
	default:
		break;
	}
}

static void usb_standard_request(struct setup_packet *setup)
{
	switch (setup->bRequest) {
	case CLEAR_FEATURE:
		break;
	case GET_CONFIGURATION:
		usb_get_configuration();
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

static void usb_class_request(struct setup_packet *setup)
{
	class_request(setup);
}

static void usb_get_descriptor(struct setup_packet *setup)
{
	uint8_t type, index;
	uint16_t len;
	void *desc;

	type = setup->wValue >> 8;
	index = (uint8_t)setup->wValue & 0x00ff;

	len = get_descriptor(type, index, &desc);
	usb_send_descriptor(desc, MIN(len, (uint16_t)setup->wLength));
}

static void usb_set_address(uint8_t addr)
{
#ifdef USB_DEBUG
	printf("USB set address %d\n", addr);
#endif
	usb_set_device_state(USB_STATE_ADDRESS);

	d12_set_address_enable(addr);
	usb_send_zero_length_packet();
}

static uint8_t g_current_configuration = 0;
static void usb_set_configuration(uint8_t config)
{
#ifdef USB_DEBUG
	printf("USB set configuration\n");
#endif
	usb_set_device_state(USB_STATE_CONFIGURED);

	g_current_configuration = config;
	d12_set_endpoint_enable(config);
	usb_send_zero_length_packet();
}

static void usb_get_configuration(void)
{
#ifdef USB_DEBUG
	printf("USB get configuration\n");
#endif
	d12_write_buffer(D12_EPINDEX_0_IN, &g_current_configuration, 1);
}

static enum usb_device_state g_state = USB_STATE_NOTATTACHED;
void usb_set_device_state(enum usb_device_state new_state)
{
	g_state = new_state;
}

enum usb_device_state usb_get_device_state(void)
{
	return g_state;
}

void usb_register_class_driver(void (*handler)(struct setup_packet *setup))
{
	class_request = handler;
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

	usb_set_device_state(USB_STATE_ATTACHED);
	usb_set_device_state(USB_STATE_POWERED);

	return 0;
}
