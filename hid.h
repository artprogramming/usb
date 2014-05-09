#ifndef __USB_HID_H
#define __USB_HID_H

#include <stdint.h>

enum {
	DESC_HID = 0x21,
	DESC_REPORT = 0x22,
	DESC_PHYSICAL = 0x23,
};

enum {
	GET_REPORT = 0x01,
	GET_IDLE = 0x02,
	GET_PROTOCOL = 0x03,
	SET_REPORT = 0x09,
	SET_IDLE = 0x0A,
	SET_PROTOCOL = 0x0B,
};

enum {
	PROTO_BOOT = 0,
	PROTO_REPORT = 1,
};

struct hid_descriptor {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint16_t	bcdHID;
	uint8_t		bCountryCode;
	uint8_t		bNumDescriptors;
	uint8_t		bDescriptorType2;	/* Report descriptor... */
	uint16_t	wDescriptorLength;
};

struct hid_optional_descriptor {
	uint8_t		bDescriptorType;
	uint16_t	wDescriptorLength;
};

extern void hid_class_request(struct setup_packet *setup);
extern void hid_get_report(void);
extern void hid_set_report(void);
extern void hid_get_idle(void);
extern void hid_set_idle(void);
extern void hid_get_protocol(void);
extern void hid_set_protocol(uint8_t protocol);

#endif /* __USB_HID_H */
