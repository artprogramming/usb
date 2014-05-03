#ifndef __USB_HID_H
#define __USB_HID_H

#include <stdint.h>

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

#define DESC_HID	0x21
#define DESC_REPORT	0x22

#endif /* __USB_HID_H */
