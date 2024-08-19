#include <esp32-hal-log.h>
#include <map>
#include <Arduino.h>

#include <device_config.h>
#include <os_config.h>
#include <controls/keypad.h>

static char LOG_TAG[] = "KEYP";

static TaskHandle_t hTask;
static std::map<gpio_num_t, bool> oldStates = {};

static void keypad_task(void*) {
	while(1) {
		for(auto i: HWCONF_KEYPAD) {
			bool lvl = gpio_get_level(i.first) > 0;
			if(oldStates[i.first] != lvl) {
				oldStates[i.first] = lvl;
				hid_set_key_state(i.second, lvl);
			}
		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void keypad_start() {
	gpio_config_t io_conf = {
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = gpio_pullup_t::GPIO_PULLUP_ENABLE,
		.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE,
	};

	for(auto i: HWCONF_KEYPAD) {
		io_conf.pin_bit_mask |= 1ULL << i.first;
		oldStates[i.first] = false;
	}

	ESP_ERROR_CHECK(gpio_config(&io_conf));

	ESP_LOGI(LOG_TAG, "Creating task");
		xTaskCreate(
			keypad_task,
			"KEYPAD",
			4096,
			nullptr,
			OS_TASK_PRIORITY_KEYPAD,
			&hTask
		);
}