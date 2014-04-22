#ifndef __KEY_H
#define __KEY_H

#include <stdint.h>

#define KEY1		(1 << 0)
#define KEY2		(1 << 1)
#define KEY3		(1 << 2)
#define KEY4		(1 << 3)
#define KEY5		(1 << 4)
#define KEY6		(1 << 5)
#define KEY7		(1 << 6)
#define KEY8		(1 << 7)

extern uint8_t keyscan(void);

#endif /* __KEY_H */
