/*
 * This discriptor copy from HID Specification v 1.11 Appendix E.
 */

#include <stdint.h>

#include "types.h"
#include "ch9.h"
#include "hid.h"

__code static uint8_t device_descriptor[] = {
	0x12,		/* bLength */
	0x01,		/* bDescriptorType */
	0x00, 0x01,	/* bcdUSB */
	0x00,		/* bDeviceClass */
	0x00,		/* bDeviceSubClass */
	0x00,		/* bDeviceProtocol */
	0x10,		/* bMaxPacketSize0 */
	0xFF, 0xFF,	/* idVendor */
	0x01, 0x00,	/* idProduct */
	0x00, 0x01,	/* bcdDevice */
	0x00,		/* iManufacturer */
	0x00,		/* iProduct */
	0x00,		/* iSerialNumber */
	0x01,		/* bNumConfigurations */
};

__code static uint8_t config_descriptor[] = {
	/* Configuration Descriptor */
	0x09,		/* bLength */
	0x02,		/* bDescriptorType */
	0x22, 0x00,	/* wTotalLength */
	0x01,		/* bNumInterfaces */
	0x01,		/* bConfigurationValue */
	0x00,		/* iConfiguration */
	0xA0,		/* bmAttributes */
	0x32,		/* bMaxPower */

	/* Interface Descriptor */
	0x09,		/* bLength */
	0x04,		/* bDescriptorType */
	0x00,		/* bInterfaceNumber */
	0x00,		/* bAlternateSetting */
	0x01,		/* bNumEndpoints */
	0x03,		/* bInterfaceClass */
	0x01,		/* bInterfaceSubClass */
	0x01,		/* bInterfaceProtocol */
	0x00,		/* iInterface */

	/* HID Descriptor */
	0x09,		/* bLength */
	0x21,		/* bDescriptorType */
	0x01, 0x01,	/* bcdHID */
	0x00,		/* bCountryCode */
	0x01,		/* bNumDescriptors */
	0x22,		/* bDescriptorType2 */
	0x3F, 0x00,	/* wDescriptorLength */

	/* Endpoint Descriptor */
	0x07,		/* bLength */
	0x05,		/* bDescriptorType */
	0x81,		/* bEndpointAddress */
	0x03,		/* bmAttributes */
	0x08, 0x00,	/* wMaxPacketSize */
	0x00,		/* bInterval */
};

__code static uint8_t report_descriptor[] = {
	0x05, 0x01,	// Usage Page (Generic Desktop)
	0x09, 0x06,	// Usage (Keyboard)
	0xA1, 0x01,	// Collection (Application)
	0x05, 0x07,	//   Usage Page (Key Codes)
	0x19, 0xE0,	//   Usage Minimum (224)
	0x29, 0xE7,	//   Usage Maximum (231)
	0x15, 0x00,	//   Logical Minimum (0)
	0x25, 0x01,	//   Logical Maximum (1)
	0x75, 0x01,	//   Report Size (1)
	0x95, 0x08,	//   Report Count (8)
	0x81, 0x02,	//   Input (Data, Variable, Absolute)
	0x95, 0x01,	//   Report Count (1)
	0x75, 0x08,	//   Report Size (8)
	0x81, 0x01,	//   Input (Constant)
	0x95, 0x05,	//   Report Count (5)
	0x75, 0x01,	//   Report Size (1)
	0x05, 0x08,	//   Usage Page (Page# for LEDs)
	0x19, 0x01,	//   Usage Minimum (1)
	0x29, 0x05,	//   Usage Maximum (5)
	0x91, 0x02,	//   Output (Data, Variable, Absolute)
	0x95, 0x01,	//   Report Count (1)
	0x75, 0x03,	//   Report Size (3)
	0x91, 0x01,	//   Output (Constant)
	0x95, 0x06,	//   Report Count (6)
	0x75, 0x08,	//   Report Size (8)
	0x15, 0x00,	//   Logical Minimum (0)
	0x25, 0x65,	//   Logical Maximum (101)
	0x05, 0x07,	//   Usage Page (Key Codes)
	0x19, 0x00,	//   Usage Minimum (0),
	0x29, 0x65,	//   Usage Maximum (101)
	0x81, 0x00,	//   Input (Data, Array)
	0xC0,		// End Collection
};

static int16_t get_device_descriptor(const void **p)
{
	*p = &device_descriptor;

	return sizeof(device_descriptor);
}

static int16_t get_config_descriptor(const void **p)
{
	*p = &config_descriptor;

	return sizeof(config_descriptor);
}

static int16_t get_report_descriptor(const void **p)
{
	*p = &report_descriptor;

	return sizeof(report_descriptor);
}

int16_t get_descriptor(uint8_t type, uint8_t index, const void **p)
{
	int16_t size = 0;

	switch (type) {
	case DESC_DEVICE:
		size = get_device_descriptor(p);
		break;
	case DESC_CONFIGURATION:
		size = get_config_descriptor(p);
		break;
	case DESC_STRING:
		break;
	case DESC_REPORT:
		size = get_report_descriptor(p);
		break;
	default:
		*p = NULL;
		break;
	}

	return size;
}
