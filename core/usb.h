#ifndef __USB_H
#define __USB_H

extern void usb_send_zero_length_packet(void);
extern void usb_set_device_state(enum usb_device_state new_state);
extern enum usb_device_state usb_get_device_state(void);
extern void usb_register_class_driver(void (*handler)(struct setup_packet *setup));

#endif /* __USB_H */
