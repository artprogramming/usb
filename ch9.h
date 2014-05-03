#ifndef __USB_CH9_H
#define __USB_CH9_H

#include <stdint.h>

enum {
	TYPE_STANDARD	= 0,
	TYPE_CLASS	= 1,
	TYPE_VENDOR	= 2,
	TYPE_RESERVED	= 3,
};

enum {
	RECIP_DEVICE	= 0,
	RECIP_INTERFACE	= 1,
	RECIP_ENDPOINT	= 2,
	RECIP_OTHER	= 3,
};

struct setup_packet {
	union {
		struct {
			uint8_t recipient : 5;
			uint8_t type : 2;
			uint8_t direction : 1;
		};
		uint8_t	bmRequestType;
	} REQUEST;
	uint8_t	bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
};

enum {
	GET_STATUS = 0x00,
	CLEAR_FEATURE = 0x01,
	SET_FEATURE = 0x03,
	SET_ADDRESS = 0x05,
	GET_DESCRIPTOR = 0x06,
	SET_DESCRIPTOR = 0x07,
	GET_CONFIGURATION = 0x08,
	SET_CONFIGURATION = 0x09,
	GET_INTERFACE = 0x0A,
	SET_INTERFACE = 0x0B,
	SYNCH_FRAME = 0x0C,
};

enum {
	DESC_DEVICE = 0x01,
	DESC_CONFIGURATION = 0x02,
	DESC_STRING = 0x03,
	DESC_INTERFACE = 0x04,
	DESC_ENDPOINT = 0x05,
	DESC_DEVICE_QUALIFIER = 0x06,
	DESC_OTHER_SPEED_CONFIGURATION = 0x07,
	DESC_INTERFACE_POWER = 0x08,
};

struct device_descriptor {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint16_t	bcdUSB;
	uint8_t		bDeviceClass;
	uint8_t		bDeviceSubClass;
	uint8_t		bDeviceProtocol;
	uint8_t		bMaxPacketSize0;
	uint16_t	idVendor;
	uint16_t	idProduct;
	uint16_t	bcdDevice;
	uint8_t		iManufacturer;
	uint8_t		iProduct;
	uint8_t		iSerialNumber;
	uint8_t		bNumConfigurations;
};

struct configuration_descriptor {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint16_t	wTotalLength;
	uint8_t		bNumInterfaces;
	uint8_t		bConfigurationValue;
	uint8_t		iConfiguration;
	uint8_t		bmAttributes;
	uint8_t		bMaxPower;
};

struct interface_descriptor {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint8_t		bInterfaceNumber;
	uint8_t		bAlternateSetting;
	uint8_t		bNumEndpoints;
	uint8_t		bInterfaceClass;
	uint8_t		bInterfaceSubClass;
	uint8_t		bInterfaceProtocol;
	uint8_t		iInterface;
};

struct endpoint_descriptor {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint8_t		bEndpointAddress;
	uint8_t		bmAttributes;
	uint16_t	wMaxPacketSize;
	uint8_t		bInterval;	
};

#endif /* __USB_CH9_H */
