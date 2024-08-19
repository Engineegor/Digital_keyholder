#pragma once
#include "esp32-hal-gpio.h"

typedef enum key_id {
	KEY_UP		= (1 << 0),
	KEY_DOWN	= (1 << 1),
	KEY_NEXT	= (1 << 2),
	KEY_PREV	= (1 << 3),
	KEY_OK		= (1 << 4),

	KEY_MAX_INVALID = 0xFF
} key_id_t;

typedef enum key_state {
	KEYSTATE_RELEASED,
	KEYSTATE_HIT,
	KEYSTATE_PRESSED,
	KEYSTATE_HOLDING
} key_state_t;

typedef uint8_t key_bitmask_t;

const key_bitmask_t KEYMASK_ALL = 0xFF;

void hid_set_key_state(key_id_t key, bool state);
key_state_t hid_check_key_state(key_id_t key);
key_state_t hid_check_key_all(key_bitmask_t keys);
key_state_t hid_check_key_any(key_bitmask_t keys = KEYMASK_ALL);
key_state_t hid_peek_key_any(key_bitmask_t keys = KEYMASK_ALL);
key_state_t hid_test_key_state_repetition(key_id_t key);

