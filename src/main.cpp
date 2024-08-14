#include <Arduino.h>
#include <display/display.h>
#include <graphics/framebuffer.h>
#include "esp32-hal-gpio.h"
#include <graphics/ui.h>

#include <common.h>

DisplayFramebuffer * fb;
uiClass * ui;

paramClass * brig;
paramClass * speed;

Screen main_screen;
Frame test_frame(20, 10, 64, 16);
Frame test_frame2(0, 0, 5, 5);

Screen second_screen;
Frame test_frame3(32, 32, 16, 16);

bool trigger_inc = false;
bool trigger_dec = false;
bool trigger_switch = false;

void setup() {
	Serial.begin(115200);
	Serial.println();

	ui = new uiClass();
	fb = new DisplayFramebuffer(&display_driver, ui);

	brig = new paramClass(0, 200, 5, 70);
	speed = new paramClass(5, 60, 5, 20);
	fb->setup();
	delay(2000);

	gpio_config_t io_conf = {
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = gpio_pullup_t::GPIO_PULLUP_ENABLE,
		.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE,
	};

	io_conf.pin_bit_mask |= 1ULL << BUTTON_INC_PIN;
	io_conf.pin_bit_mask |= 1ULL << BUTTON_DEC_PIN;
	io_conf.pin_bit_mask |= 1ULL << SWITCH_PIN;

	ESP_ERROR_CHECK(gpio_config(&io_conf));

	
	main_screen.add_child(&test_frame);
	main_screen.add_child(&test_frame2);

	second_screen.add_child(&test_frame3);

	ui->set_screen(&main_screen);
}

void loop() {

	unsigned long millis_current = millis();
	static unsigned long millis_last = millis();
	if (millis_current - millis_last > 20) {
		millis_last = millis_current;

		bool state_inc = gpio_get_level(BUTTON_INC_PIN);
		bool state_dec = gpio_get_level(BUTTON_DEC_PIN);
		bool state_switch = gpio_get_level(SWITCH_PIN);

		/*if (state_inc) {
			if (!trigger_inc) {
				trigger_inc = true;
				if (!trigger_switch) {
					brig->inc();
					Serial.printf("Contrast: %u\n\r", uint8_t(brig->get()));
					fb->set_contrast(brig->get());
				} else {
					speed->inc();
					Serial.printf("Fps: %u\n\r", speed->get());
					fb->set_fps(speed->get());                    
				}
			}
		} else if (trigger_inc) trigger_inc = false;/**/

		/*if (state_dec) {
			if (!trigger_dec) {
				trigger_dec = true;
				if (!trigger_switch) {
					brig->dec();
					Serial.printf("Contrast: %u\n\r", uint8_t(brig->get()));
					fb->set_contrast(brig->get());
				} else {
					speed->dec();
					Serial.printf("Fps: %u\n\r", speed->get());
					fb->set_fps(speed->get());
				}
			}
		} else if (trigger_dec) trigger_dec = false;/**/

		if ( state_switch && !trigger_switch) {
			trigger_switch = true;
			ui->set_screen(&main_screen);
			Serial.printf("main screen\n\r");
		} else if (!state_switch &&  trigger_switch) {
			trigger_switch = false;
			Serial.printf("second screen\n\r");
			ui->set_screen(&second_screen);
		}

	}

	fb->show();
}
