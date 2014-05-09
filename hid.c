#include <stdint.h>

#include "ch9.h"
#include "hid.h"
#include "d12.h"
#include "usb.h"
#include "printf.h"


#define HID_DEBUG	1

static uint8_t this_protocol = 0;

void hid_class_request(struct setup_packet *setup)
{
	switch (setup->bRequest) {
	case GET_REPORT:
		hid_get_report();
		break;
	case GET_IDLE:
		hid_get_idle();
		break;
	case GET_PROTOCOL:
		hid_get_protocol();
		break;
	case SET_REPORT:
		hid_set_report();
		break;
	case SET_IDLE:
		hid_set_idle();
		break;
	case SET_PROTOCOL:
		hid_set_protocol(setup->wValue >> 8);
		break;
	default:
		break;
	}
}

void hid_get_protocol(void)
{
#ifdef HID_DEBUG
	printf("Get_Protocol Request\n");
#endif
	d12_write_buffer(D12_EPINDEX_0_IN, &this_protocol, 1);
}

void hid_set_protocol(uint8_t protocol)
{
#ifdef HID_DEBUG
	printf("Set_Protocol Request\n");
#endif
	this_protocol = protocol;
	
	usb_send_zero_length_packet();
}

void hid_get_report(void)
{
#ifdef HID_DEBUG
	printf("Get_Report Request\n");
#endif
}

void hid_set_report(void)
{
#ifdef HID_DEBUG
	printf("Set_Report Request\n");
#endif
}

void hid_get_idle(void)
{
#ifdef HID_DEBUG
	printf("Get_Idle Request\n");
#endif
}

void hid_set_idle(void)
{
#ifdef HID_DEBUG
	printf("Set_Idle Request\n");
#endif
	usb_send_zero_length_packet();
}
