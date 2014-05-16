/*
 * This discriptor copy from HID Specification v 1.11 Appendix E.
 */

#include <stdint.h>

#include "ch9.h"
#include "hid.h"

struct usb_keyboard_configuration {
	struct configuration_descriptor	config;
	struct interface_descriptor	interface;
	struct hid_descriptor		hid;
	struct endpoint_descriptor	ep;
};

__code static uint8_t report_descriptor[] = {
	0x05, 0x01,		// Usage Page (Generic Desktop)
	0x09, 0x06,		// Usage (Keyboard)
	0xA1, 0x01,		// Collection (Application)
	0x05, 0x07,		//   Usage Page (Key Codes)
	0x19, 0xE0,		//   Usage Minimum (224)
	0x29, 0xE7,		//   Usage Maximum (231)
	0x15, 0x00,		//   Logical Minimum (0)
	0x25, 0x01,		//   Logical Maximum (1)
	0x75, 0x01,		//   Report Size (1)
	0x95, 0x08,		//   Report Count (8)
	0x81, 0x02,		//   Input (Data, Variable, Absolute)
	0x95, 0x01,		//   Report Count (1)
	0x75, 0x08,		//   Report Size (8)
	0x81, 0x01,		//   Input (Constant)
	0x95, 0x05,		//   Report Count (5)
	0x75, 0x01,		//   Report Size (1)
	0x05, 0x08,		//   Usage Page (Page# for LEDs)
	0x19, 0x01,		//   Usage Minimum (1)
	0x29, 0x05,		//   Usage Maximum (5)
	0x91, 0x02,		//   Output (Data, Variable, Absolute)
	0x95, 0x01,		//   Report Count (1)
	0x75, 0x03,		//   Report Size (3)
	0x91, 0x01,		//   Output (Constant)
	0x95, 0x06,		//   Report Count (6)
	0x75, 0x08,		//   Report Size (8)
	0x15, 0x00,		//   Logical Minimum (0)
	0x25, 0x65,		//   Logical Maximum (101)
	0x05, 0x07,		//   Usage Page (Key Codes)
	0x19, 0x00,		//   Usage Minimum (0),
	0x29, 0x65,		//   Usage Maximum (101)
	0x81, 0x00,		//   Input (Data, Array)
	0xC0,			// End Collection
};

__code static struct device_descriptor device_descriptor = {
	.bLength		= sizeof(struct device_descriptor),
	.bDescriptorType	= DESC_DEVICE,
	.bcdUSB			= 0x0110,
	.bDeviceClass		= 0x00,
	.bDeviceSubClass	= 0x00,
	.bDeviceProtocol	= 0x00,
	.bMaxPacketSize0	= 0x10,
	.idVendor		= 0xFFFF,
	.idProduct		= 0x0001,
	.bcdDevice		= 0x0100,
	.iManufacturer		= 0x00,
	.iProduct		= 0x00,
	.iSerialNumber		= 0x00,
	.bNumConfigurations	= 0x01,
};

__code static struct usb_keyboard_configuration configuration_descriptor = {
       {.bLength		= sizeof(struct configuration_descriptor),
	.bDescriptorType	= DESC_CONFIGURATION,
	.wTotalLength		= sizeof(struct usb_keyboard_configuration),
	.bNumInterfaces		= 0x01,
	.bConfigurationValue	= 0x01,
	.iConfiguration		= 0x00,
	.bmAttributes		= 0xA0,
	.bMaxPower		= 0x32,},

       {.bLength		= sizeof(struct interface_descriptor),
	.bDescriptorType	= DESC_INTERFACE,
	.bInterfaceNumber	= 0x00,
	.bAlternateSetting	= 0x00,
	.bNumEndpoints		= 0x01,
	.bInterfaceClass	= 0x03,
	.bInterfaceSubClass	= 0x01,
	.bInterfaceProtocol	= 0x01,
	.iInterface		= 0x00,},

       {.bLength		= sizeof(struct hid_descriptor),
	.bDescriptorType	= DESC_HID,
	.bcdHID			= 0x0101,
	.bCountryCode		= 0x00,
	.bNumDescriptors	= 0x01,
	.bDescriptorType2	= DESC_REPORT,
	.wDescriptorLength	= sizeof(report_descriptor),},

       {.bLength		= sizeof(struct endpoint_descriptor),
	.bDescriptorType	= DESC_ENDPOINT,
	.bEndpointAddress	= 0x81,
	.bmAttributes		= 0x03,
	.wMaxPacketSize		= 0x0008,
	.bInterval		= 0x0A,},
};

int16_t usb_get_device_descriptor(const void **p)
{
	*p = &device_descriptor;

	return sizeof(device_descriptor);
}

int16_t usb_get_config_descriptor(const void **p)
{
	*p = &configuration_descriptor;

	return sizeof(configuration_descriptor);
}

int16_t usb_get_hid_descriptor(const void **p)
{
	*p = &configuration_descriptor.hid;

	return sizeof(configuration_descriptor.hid);
}

int16_t usb_get_report_descriptor(const void **p)
{
	*p = report_descriptor;

	return sizeof(report_descriptor);
}
