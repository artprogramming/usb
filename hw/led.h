#ifndef __LED_H
#define __LED_H

enum led_index {
	LED_1 = 0,
	LED_2,
	LED_3,
	LED_4,
	LED_5,
	LED_6,
	LED_7,
	LED_8
};


extern void led_on(enum led_index i);
extern void led_off(enum led_index i);
extern void all_on(void);
extern void all_off(void);

#endif /* __LED_H */
