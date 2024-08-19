#include <Arduino.h>
#include "esp32-hal-gpio.h"

#include <display/display.h>
#include <graphics/framebuffer.h>
#include <graphics/ui.h>
#include <manager/view_manager.h>
#include <common.h>

DisplayFramebuffer * fb;
uiClass * ui;

void setup() {
	Serial.begin(115200);
	Serial.println();

	ui = new uiClass();
	fb = new DisplayFramebuffer(&display_driver, ui);

	fb->setup();

	keypad_start();

	views_setup(ui);
	
}

void loop() {
	views_task(ui);
}
