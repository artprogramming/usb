/* Copy from book of Serial Port Complete at Table 16-3: Example descriptor for a full-speed CDC device */
#include <stdint.h>

#include "types.h"

//////////////////////////////////////////////////////////////////////////////////
//						Device		                //
//					    Configuration			//
//		Interface(Communication)		Interface(Data)	        //
//		Header Functional			Endpoint(Bulk IN)       //
//	 	Abstract Control Model Functional	Endpoint(Bulk OUT)      //
//		Union Functional						//
//	 	Call Management Functional					//
//		Endpoint(Interrupt IN)						//
//////////////////////////////////////////////////////////////////////////////////

__code static uint8_t device_descriptor[] = {
	0x12,		/* bLength */
	0x01,		/* bDescriptorType */
	0x00, 0x02,	/* bcdUSB */
	0x02,		/* bDeviceClass */
	0x00,		/* bDeviceSubClass */
	0x00,		/* bDeviceProtocol */
	0x10,		/* bMaxPacketSize0 */
	0x25, 0x09,	/* idVendor */
	0x50, 0x90,	/* idProduct */
	0x00, 0x01,	/* bcdDevice */
	0x00,//0x01,		/* iManufacturer */
	0x00,//0x02,		/* iProduct */
	0x00,//0x03,		/* iSerialNumber */
	0x01,		/* bNumConfigurations */
}; 

__code static uint8_t config_descriptor[] = {
	/* Configuration Descriptor */
	0x09,		/* bLength */
	0x02,		/* bDescriptorType */
	0x43, 0x00,	/* wTotalLength */
	0x02,		/* bNumInterfaces */
	0x01,		/* bConfigurationValue */
	0x00,		/* iConfiguration */
	0x00,		/* bmAttributes */
	0x32,		/* bMaxPower */
	
	/* Interface Descriptor */
	0x09,		/* bLength */
	0x04,		/* bDescriptorType */
	0x00,		/* bInterfaceNumber */
	0x00,		/* bAlternateSetting */
	0x01,		/* bNumEndpoints */
	0x02,		/* bInterfaceClass */
	0x02,		/* bInterfaceSubClass */
	0x01,		/* bInterfaceProtocol */
	0x00,		/* iInterface */
	
	/* Header Function Descriptor */
	0x05,		/* bFunctionLength */
	0x24,		/* bDescriptorType */
	0x00,		/* bDescriptorSubtype */
	0x10, 0x01,	/* bcdCDC */
	
	/* Abstract Control Management Functional Descriptor */
	0x04,		/* bFunctionLength */
	0x24,		/* bDescriptorType */
	0x02,		/* bDescriptorSubtype */
	0x02,		/* bmCapabilities */
	
	/* Union Functional Descriptor */
	0x05,		/* bFunctionLength */
	0x24,		/* bDescriptorType */
	0x06,		/* bDescriptorSubtype */
	0x00,		/* bMasterInterface */
	0x01,		/* bSlaveInterface0 */
	
	/* Call Management Functional Descriptor */
	0x05,		/* bFunctionLength */
	0x24,		/* bDescriptorType */
	0x01,		/* bDescriptorSubtype */
	0x00,		/* bmCapabilities */
	0x01,		/* dDataInterface */
	
	/* Endpoint Descriptor */
	0x07,		/* bLength */
	0x05,		/* bDescriptorType */
	0x81,		/* bEndpointAddress */
	0x03,		/* bmAttributes */
	0x08, 0x00,	/* wMaxPacketSize */
	0x02,		/* bInterval */
	
	/* Interface Descriptor */
	0x09,		/* bLength */
	0x04,		/* bDescriptorType */
	0x01,		/* bInterfaceNumber */
	0x00,		/* bAlternateSetting */
	0x02,		/* bNumEndpoints */
	0x0A,		/* bInterfaceClass */
	0x00,		/* bInterfaceSubClass */
	0x00,		/* bInterfaceProtocol */
	0x00,		/* iInterface */
	
	/* Endpoint Descriptor */
	0x07,		/* bLength */
	0x05,		/* bDescriptorType */
	0x82,		/* bEndpointAddress */
	0x02,		/* bmAttributes */
	0x40, 0x00,	/* wMaxPacketSize */
	0x00,		/* bInterval */
	
	/* Endpoint Descriptor */
	0x07,		/* bLength */
	0x05,		/* bDescriptorType */
	0x02,		/* bEndpointAddress */
	0x02,		/* bmAttributes */
	0x40, 0x00,	/* wMaxPacketSize */
	0x00,		/* bInterval */
};

__code static uint8_t language_id[] = {
	0x04,		/* bLength */
	0x03,		/* bDescriptorType */
	0x09, 0x04,	/* wLANGID */
};

__code static uint8_t manufacturer_string[] = {
	0x2C,		/* bLength */
	0x03,		/* bDescriptorType */
	'L', 0x00,
	'a', 0x00,
	'k', 0x00,
	'e', 0x00,
	'v', 0x00,
	'i', 0x00,
	'e', 0x00,
	'w', 0x00,
	' ', 0x00,
	'R', 0x00,
	'e', 0x00,
	's', 0x00,
	'e', 0x00,
	'a', 0x00,
	'r', 0x00,
	'c', 0x00,
	'h', 0x00,
	' ', 0x00,
	'L', 0x00,
	'L', 0x00,
	'C', 0x00,
};

__code static uint8_t product_string[]= {
	0x20,		/* bLength */
	0x03,		/* bDescriptorType */
	'C', 0x00,
	'O', 0x00,
	'M', 0x00,
	' ', 0x00,
	'P', 0x00,
	'o', 0x00,
	'r', 0x00,
	't', 0x00,
	' ', 0x00,
	'D', 0x00,
	'e', 0x00,
	'v', 0x00,
	'i', 0x00,
	'c', 0x00,
	'e', 0x00,
};

__code static uint8_t serial_number_string[] = {
	0x1A,		/* bLength */
	0x03,		/* bDescriptorType */
	'1', 0x00,
	'2', 0x00,
	'3', 0x00,
	'4', 0x00,
	'5', 0x00,
	'6', 0x00,
	'7', 0x00,
	'8', 0x00,
	'9', 0x00,
	'A', 0x00,
	'B', 0x00,
	'C', 0x00,
};

int16_t usb_get_device_descriptor(const void **p)
{
	*p = &device_descriptor;
	
	return sizeof(device_descriptor);
}

int16_t usb_get_config_descriptor(const void **p)
{
	*p = &config_descriptor;
	
	return sizeof(config_descriptor);
}

int16_t usb_get_string_descriptor(const void **p, uint8_t index)
{
	switch (index) {
	case 0:
		*p = &language_id;
		return sizeof(language_id);
	case 1:
		*p = &manufacturer_string;
		return sizeof(manufacturer_string);
	case 2:
		*p = &product_string;
		return sizeof(product_string);
	case 3:
		*p = &serial_number_string;
		return sizeof(serial_number_string);
	default:
		*p = NULL;
		return 0;
	}
}
