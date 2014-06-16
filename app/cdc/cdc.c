#include <stdint.h>

#include "types.h"
#include "ch9.h"
#include "d12.h"
#include "usb.h"
#include "cdc.h"

static uint8_t this_device_line_coding[] = {0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08};

extern int16_t get_descriptor(uint8_t type, uint8_t index, const void **p);

static void set_line_coding(void);
static void get_line_coding(uint8_t bytes_send);
static void set_control_line_state(void);

static void cdc_class_request(struct setup_packet *setup)
{
	switch (setup->bRequest) {
	case SET_LINE_CODING:
		set_line_coding();
		break;
	case GET_LINE_CODING:
		get_line_coding(MIN(sizeof(this_device_line_coding), setup->wLength));
		break;
	case SET_CONTROL_LINE_STATE:
		set_control_line_state();
		break;
	default:
		break;
	}
}

static void set_line_coding(void)
{
}

static void get_line_coding(uint8_t bytes_send)
{
	d12_write_buffer(D12_EPINDEX_0_IN, (uint8_t *)&this_device_line_coding, bytes_send);
}

static void set_control_line_state(void)
{
	usb_send_zero_length_packet();
}

struct class_driver cdc_driver = {
	.get_descriptor = get_descriptor,
	.req_handler = cdc_class_request,
};

void cdc_init(void)
{
	usb_register_class_driver(&cdc_driver);
}
