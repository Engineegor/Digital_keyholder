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
Frame test_frame(20, 10, 20, 10);
Frame test_frame2(10, 10, 5, 5);

Screen second_screen;
Label label3(5, 5, "Hello world! 123", font5x7);
TextBox tbox(2, 10, "Text", font5x7, 10, 2, 3);
Frame test_frame3(0, 32, 128, 16);

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

	
	test_frame2.negative = true;
	main_screen.add_child(&test_frame);
	main_screen.add_child(&test_frame2);

	test_frame3.add_child(&label3);
	//second_screen.add_child(&tbox);
	second_screen.add_child(&test_frame3);

	Label label4(5, 5, "Hello world! 123", font5x7);
	Serial.println(label4.get_text());

	ui->set_screen(&second_screen);
}

void loop() {

	unsigned long millis_current = millis();
	static unsigned long millis_last = millis();
	if (millis_current - millis_last > 20) {
		millis_last = millis_current;

		bool state_inc = gpio_get_level(BUTTON_INC_PIN);
		bool state_dec = gpio_get_level(BUTTON_DEC_PIN);
		bool state_switch = gpio_get_level(SWITCH_PIN);

		if (state_inc) {
			if (!trigger_inc) {
				trigger_inc = true;
				test_frame3.filled = !test_frame3.filled;
				if (tbox.allign == LEFT)		{tbox.allign = EDGE;}
				else if (tbox.allign == EDGE)	{tbox.allign = RIGHT;}
				ui->update_screen();
			}
		} else if (trigger_inc) trigger_inc = false;/**/

		if (state_dec) {
			if (!trigger_dec) {
				trigger_dec = true;
				test_frame3.negative = !test_frame3.negative;
				if (tbox.allign == RIGHT)		{tbox.allign = EDGE;}
				else if (tbox.allign == EDGE)	{tbox.allign = LEFT;}
				ui->update_screen();
			}
		} else if (trigger_dec) trigger_dec = false;/**/

		if ( state_switch && !trigger_switch) {
			trigger_switch = true;			
			second_screen.negative = true;
			ui->update_screen();
		} else if (!state_switch &&  trigger_switch) {
			trigger_switch = false;
			second_screen.negative = false;
			ui->update_screen();
		}

	}

	fb->show();
}
