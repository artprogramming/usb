#include <stdint.h>

#include "types.h"
#include "printf.h"
#include "ch9.h"
#include "hid.h"


extern int16_t usb_get_hid_descriptor(uint8_t interface, const void **p);
extern int16_t usb_get_hid_report_descriptor(uint8_t interface, const void **p);


extern void usb_send_data_stage(char *buffer, uint16_t len);


#define USB_HID_DESCRIPTOR_FUNC		usb_get_hid_descriptor
#define USB_HID_REPORT_DESCRIPTOR_FUNC	usb_get_hid_report_descriptor


#define HID_GET_IDLE_CALLBACK		hid_get_idle_callback
#define HID_SET_IDLE_CALLBACK		hid_set_idle_callback
#define HID_GET_PROTOCOL_CALLBACK	hid_get_protocol_callback
#define HID_SET_PROTOCOL_CALLBACK	hid_set_protocol_callback


#ifdef HID_GET_IDLE_CALLBACK
int8_t hid_get_idle_callback(uint8_t interface, uint8_t report_id)
{
	return 0;
}
#endif

#ifdef HID_SET_IDLE_CALLBACK
int8_t hid_set_idle_callback(uint8_t interface, uint8_t report_id,
			     uint8_t idle_rate)
{
	return -1;
}
#endif

#ifdef HID_GET_PROTOCOL_CALLBACK
int8_t hid_get_protocol_callback(uint8_t interface)
{
	return -1;
}
#endif

#ifdef HID_SET_PROTOCOL_CALLBACK
int8_t hid_set_protocol_callback(uint8_t interface, uint8_t report_id)
{
	return -1;
}
#endif

int8_t class_get_descriptor_request(struct setup_packet *setup)
{
	uint8_t interface = setup->wIndex;

	if (setup->bRequest == GET_DESCRIPTOR &&
	    setup->REQUEST.bmRequestType == 0x81) {
		uint8_t descriptor = ((setup->wValue >> 8) & 0x00ff);

		const void *desc;
		int16_t len = -1;

		if (descriptor == DESC_HID) {
			len = USB_HID_DESCRIPTOR_FUNC(interface, &desc);
		}
		else if (descriptor == DESC_REPORT) {
			len = USB_HID_REPORT_DESCRIPTOR_FUNC(interface, &desc);
		}
#ifdef USB_HID_PHYSICAL_DESCRIPTOR_FUNC
		else if (descriptor == DESC_PHYSICAL) {
			uint8_t descriptor_index = setup->wValue & 0x00ff;
			len = USB_HID_PHYSICAL_DESCRIPTOR_FUNC(interface, descriptor_index, &desc);
		}
#endif
		if (len < 0)
			return -1;

		usb_send_data_stage((void *)desc, MIN(len, setup->wLength));

		return 0;
	}	
}

int8_t class_setup_request(struct setup_packet *setup)
{
	uint8_t interface = setup->wIndex;

#ifdef HID_GET_REPORT_CALLBACK
	const void *desc;
	int16_t len = -1;
	if (setup->bRequest == GET_REPORT &&
	    setup->REQUEST.bmRequestType == 0xa1) {
		uint8_t report_type = (setup->wValue >> 8) & 0x00ff;
		uint8_t report_id = setup->wValue & 0x00ff;
		len = HID_GET_REPORT_CALLBACK(interface,
					      report_type, report_id,
					      &desc);
		if (len < 0)
			return -1;

		usb_send_data_stage((void*)desc, MIN(len, setup->wLength));
		return 0;
	}
#endif

#ifdef HID_SET_REPORT_CALLBACK
	if (setup->bRequest == SET_REPORT &&
	    setup->REQUEST.bmRequestType == 0x21) {
		uint8_t report_type = (setup->wValue >> 8) & 0x00ff;
		uint8_t report_id = setup->wValue & 0x00ff;
		int8_t res = HID_SET_REPORT_CALLBACK(interface,
						     report_type, report_id);
		return res;
	}
#endif

#ifdef HID_GET_IDLE_CALLBACK
	if (setup->bRequest == GET_IDLE &&
	    setup->REQUEST.bmRequestType == 0xa1) {
		uint8_t report_id = setup->wValue & 0x00ff;
		uint8_t res = HID_GET_IDLE_CALLBACK(interface, report_id);

		usb_send_data_stage((char *)&res, 1);
	}
#endif

#ifdef HID_SET_IDLE_CALLBACK
	if (setup->bRequest == SET_IDLE &&
	    setup->REQUEST.bmRequestType == 0x21) {
		uint8_t duration = (setup->wValue >> 8) & 0x00ff;
		uint8_t report_id = setup->wValue & 0x00ff;
		uint8_t res = HID_SET_IDLE_CALLBACK(interface, report_id,
						    duration);
		return res;
	}
#endif

#ifdef HID_GET_PROTOCOL_CALLBACK
	if (setup->bRequest == GET_PROTOCOL &&
	    setup->REQUEST.bmRequestType == 0xa1) {
		int8_t res = HID_GET_PROTOCOL_CALLBACK(interface);
		if (res < 0)
			return -1;

		usb_send_data_stage((char *)&res, 1);
		return 0;
	}
#endif

#ifdef HID_SET_PROTOCOL_CALLBACK
	if (setup->bRequest == SET_PROTOCOL &&
	    setup->REQUEST.bmRequestType == 0x21) {
		int8_t res = HID_SET_PROTOCOL_CALLBACK(interface,
						       setup->wValue);

		return res;
	}
#endif

	return -1;
}
