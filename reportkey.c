#include <stdint.h>

#include "d12.h"
#include "key.h"
#include "keycodes.h"

static uint8_t old_report[8];

int reportkey(uint8_t key)
{
	uint8_t new_report[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	uint8_t i, j;

	switch (key) {
	case KEY1:
		new_report[2] = KEY_1;
		break;
	case KEY2:
		new_report[2] = KEY_2;
		break;
	case KEY3:
		new_report[2] = KEY_3;
		break;
	case KEY4:
		new_report[2] = KEY_4;
		break;
	case KEY5:
		new_report[2] = KEY_5;
		break;
	case KEY6:
		new_report[2] = KEY_6;
		break;
	case KEY7:
		new_report[2] = KEY_7;
		break;
	case KEY8:
		new_report[2] = KEY_8;
		break;
	default:
		new_report[2] = 0x00;
		break;
	}

	for (i = 0; i < 8; i++) {
		if (new_report[i] != old_report[i]) {
			d12_write_buffer(D12_EPINDEX_1_IN, new_report, 8);

			for (j = 0; j < 8; j++) {
				old_report[j] = new_report[j];
			}

			break;
		}
	}

	return 0;
}
