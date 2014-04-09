#ifndef __USB_H
#define __USB_H

void usb_ep0_out(void);
void usb_ep0_in(void);
void usb_ep1_out(void);
void usb_ep1_in(void);
void usb_ep2_out(void);
void usb_ep2_in(void);
void usb_bus_reset(void);
void usb_suspend(void);
void usb_dma_eot(void);

#endif /* __USB_H */
