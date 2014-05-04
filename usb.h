#ifndef __USB_H
#define __USB_H

extern void handle_ep0_out(void);
extern void handle_ep0_in(void);

extern void usb_send_descriptor(void *desc, uint8_t size);
extern void usb_control_transfer(uint8_t ep, uint8_t *buf, uint8_t size);
extern void usb_send_zero_length_packet(void);

extern void usb_setup_request(void);
extern void usb_standard_request(struct setup_packet *setup);
extern void usb_get_descriptor(struct setup_packet *setup);
extern void usb_set_address(uint8_t addr);
extern void usb_set_configuration(uint8_t config);

extern void usb_set_device_state(enum usb_device_state new_state);
extern enum usb_device_state usb_get_device_state(void);

#endif /* __USB_H */
