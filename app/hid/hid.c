#include <stdint.h>

#include "ch9.h"
#include "d12.h"
#include "usb.h"
#include "hid.h"

static uint8_t this_device_protocol_code = 0;

static void get_protocol(void);
static void set_protocol(uint8_t protocol);
static void get_report(void);
static void set_report(void);
static void get_idle(void);
static void set_idle(void);

static void hid_class_request(struct setup_packet *setup)
{
	switch (setup->bRequest) {
	case GET_REPORT:
		get_report();
		break;
	case GET_IDLE:
		get_idle();
		break;
	case GET_PROTOCOL:
		get_protocol();
		break;
	case SET_REPORT:
		set_report();
		break;
	case SET_IDLE:
		set_idle();
		break;
	case SET_PROTOCOL:
		set_protocol(setup->wValue >> 8);
		break;
	default:
		break;
	}
}

static void get_protocol(void)
{
	d12_write_buffer(D12_EPINDEX_0_IN, &this_device_protocol_code, sizeof(this_device_protocol_code));
}

static void set_protocol(uint8_t protocol)
{
	this_device_protocol_code = protocol;
	usb_send_zero_length_packet();
}

static void get_report(void)
{
}

static void set_report(void)
{
}

static void get_idle(void)
{
}

static void set_idle(void)
{
	usb_send_zero_length_packet();
}

void hid_init(void)
{
	usb_register_class_driver(hid_class_request);
}
