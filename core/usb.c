#include <at89x52.h>
#include <stdint.h>

#include "d12.h"
#include "printf.h"
#include "delay.h"
#include "ch9.h"
#include "usb.h"
//#include "hid.h"
#include "types.h"

extern int16_t usb_get_device_descriptor(const void **p);
extern int16_t usb_get_config_descriptor(const void **p);
#define USB_DEBUG	1

/* for class driver */
struct class_driver {
	void (*class_request_handler)(struct setup_packet *setup);
};

struct class_driver class_driver;
struct class_driver *get_class_driver(void)
{
	return &class_driver;
}

void usb_register_class_driver(void (*class_request_handler)(struct setup_packet *setup))
{
	struct class_driver *drv;

	drv = get_class_driver();
	drv->class_request_handler = class_request_handler;
}

uint8_t outbuf[8];
void usb_irq(void) __interrupt 0
{
	uint16_t status = d12_read_interrupt_register();

	//printf("int status %x\n", status);
#if 0
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
#endif
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
		d12_read_buffer(D12_EPINDEX_2_OUT, outbuf, 8);
		printf("recv %x %x %x %x %x %x %x %x", outbuf[0], outbuf[1],
			outbuf[2], outbuf[3], outbuf[4], outbuf[5],
			outbuf[6], outbuf[7]);
		d12_clear_buffer();
	} else if (status & D12_INT_EP2_IN) {
		d12_read_last_transaction_status(D12_EPINDEX_2_IN);
	}else {
		printf("Unkown interrupt\n");
	}
}

uint8_t flag = 0;
void handle_ep0_out(void)
{
	int i = 0;
	uint8_t status = d12_read_last_transaction_status(D12_EPINDEX_0_OUT);

	//printf("status %x\n", status);
	if (status & D12_LAST_TRANS_STATUS_SETUP_PACKET) {
		usb_setup_request();
	} else {
		if (flag == 1) {
			d12_read_buffer(D12_EPINDEX_0_OUT, outbuf, 8);
			/*printf("aaa %x %x %x %x %x %x %x\n", outbuf[0], outbuf[1], outbuf[2],
				outbuf[3], outbuf[4], outbuf[5], outbuf[6]);*/
			d12_clear_buffer();
			usb_send_zero_length_packet();
			flag = 0;
			/*outbuf[0] = outbuf[1] = outbuf[2] = outbuf[3] = outbuf[4] = outbuf[5] = oubuf[6] = outbuf[7] = 0x00;*/
			for (i = 0; i < 8; i++)
				outbuf[i] = 0x00;
		}
	}
}

static uint8_t remaining;
static uint8_t *buffer;

static uint8_t need_zlp;
void handle_ep0_in(void)
{
	d12_read_last_transaction_status(D12_EPINDEX_0_IN);

	if (remaining) {
		//usb_control_transfer(D12_EPINDEX_0_IN, buffer, MIN(remaining, EP_0_IN_LEN));
		usb_send_data(D12_EPINDEX_0_IN, buffer, MIN(remaining, EP_0_IN_LEN));
	} else if (need_zlp) {
		usb_send_zero_length_packet();
		need_zlp = 0;
	}
}

void usb_send_descriptor(void *desc, uint8_t size)
{
	buffer = (uint8_t *)desc;
	remaining = size;

	//usb_control_transfer(D12_EPINDEX_0_IN, buffer, MIN(remaining, EP_0_IN_LEN));
	usb_send_data(D12_EPINDEX_0_IN, buffer, MIN(remaining, EP_0_IN_LEN));
}

/*void usb_control_transfer(uint8_t ep, uint8_t *buf, uint8_t size)
{
	buffer += size;
	remaining -= size;

	if (remaining == 0 && size == EP_0_IN_LEN) {
		need_zlp = 1;
	}

	d12_write_buffer(ep, buf, size);
}*/
void usb_send_data(uint8_t ep, uint8_t *buf, uint8_t size)
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

void usb_setup_request(void)
{
	uint8_t buf[8];
	struct setup_packet *setup = (struct setup_packet *)buf;

	d12_read_setup_packet(buf, 8);
	
	/*printf("bmRequestType %x bRequest %x wValue %x wIndex %x wLength %x\n",
		setup->REQUEST.bmRequestType, setup->bRequest,
		setup->wValue, setup->wIndex, setup->wLength);*/

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

void usb_standard_request(struct setup_packet *setup)
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

void usb_class_request(struct setup_packet *setup)
{
	//hid_class_request(setup);
	struct class_driver *drv;

	drv = get_class_driver();
	drv->class_request_handler(setup);
}

static const char *desc_string[] = {
	"device descriptor",
	"configuration descriptor",
	"string descriptor",
	"interface descriptor",
	"endpoint descriptor",};
void usb_get_descriptor(struct setup_packet *setup)
{
	uint8_t type, index;
	uint16_t len;
	void *desc;

	type = setup->wValue >> 8;
	index = (uint8_t)setup->wValue & 0x00ff;

#ifdef USB_DEBUG
	printf("USB get descriptor type: %d index: %d\n", type, index);
#endif

	switch (type) {
	case DESC_DEVICE:
		printf("get device descriptor\n");
		len = usb_get_device_descriptor(&desc);
		usb_send_descriptor(desc, MIN(len, (uint8_t)setup->wLength));
		break;
	case DESC_CONFIGURATION:
		len = usb_get_config_descriptor(&desc);
		usb_send_descriptor(desc, MIN(len, (uint16_t)setup->wLength));
		break;
	case DESC_STRING:
		break;
	//case DESC_HID:
	//	break;
	//case DESC_REPORT:
		//len = usb_get_report_descriptor(&desc);
		//usb_send_descriptor(desc, MIN(len, (uint8_t)setup->wLength));
	//	break;
	default:
		break;
	}
}

void usb_set_address(uint8_t addr)
{
#ifdef USB_DEBUG
	printf("USB set address %d\n", addr);
#endif
	//usb_set_device_state(USB_STATE_ADDRESS);

	d12_set_address_enable(addr);
	usb_send_zero_length_packet();
}

static uint8_t g_current_configuration = 0;
void usb_set_configuration(uint8_t config)
{
#ifdef USB_DEBUG
	printf("USB set configuration\n");
#endif
	usb_set_device_state(USB_STATE_CONFIGURED);

	g_current_configuration = config;
	d12_set_endpoint_enable(config);
	usb_send_zero_length_packet();
}

void usb_get_configuration(void)
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
