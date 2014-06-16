#ifndef __USB_H
#define __USB_H

extern void usb_send_zero_length_packet(void);
extern void usb_register_class_driver(void (*handler)(struct setup_packet *setup));
#endif /* __USB_H */
