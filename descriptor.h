#ifndef __DESCRIPTOR_H
#define __DESCRIPTOR_H

#include <stdint.h>

extern int16_t usb_get_device_descriptor(const void **p);
extern int16_t usb_get_configuration_descriptor(const void **p);
extern int16_t usb_get_hid_descriptor(const void **p);
extern int16_t usb_get_report_descriptor(const void **p);

#endif /* __DESCRIPTOR_H */
