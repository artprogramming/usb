#include <at89x52.h>

#include "led.h"

#define led1	P2_0
#define led2	P2_1
#define led3	P2_2
#define led4	P2_3
#define led5	P2_4
#define led6	P2_5
#define led7	P2_6
#define led8	P2_7

#define ON	0
#define OFF	1

void led_on(enum led_index i)
{
	switch (i) {
	case LED_1:
		led1 = ON;
		break;
	case LED_2:
		led2 = ON;
		break;
	case LED_3:
		led3 = ON;
		break;
	case LED_4:
		led4 = ON;
		break;
	case LED_5:
		led5 = ON;
		break;
	case LED_6:
		led6 = ON;
		break;
	case LED_7:
		led7 = ON;
		break;
	case LED_8:
		led8 = ON;
		break;
	default:
		break;
	}
}

void led_off(enum led_index i)
{
	switch (i) {
	case LED_1:
		led1 = OFF;
		break;
	case LED_2:
		led2 = OFF;
		break;
	case LED_3:
		led3 = OFF;
		break;
	case LED_4:
		led4 = OFF;
		break;
	case LED_5:
		led5 = OFF;
		break;
	case LED_6:
		led6 = OFF;
		break;
	case LED_7:
		led7 = OFF;
		break;
	case LED_8:
		led8 = OFF;
		break;
	default:
		break;
	}
}

void all_on(void)
{
	P2 = 0x00;
}

void all_off(void)
{
	P2 = 0xFF;
}
