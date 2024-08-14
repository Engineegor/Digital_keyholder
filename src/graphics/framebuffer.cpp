#include <graphics/framebuffer.h>

DisplayFramebuffer::DisplayFramebuffer(DisplayDriver * disp, uiClass * _interface) {
	display		= disp;
	interface	= _interface;
}

void DisplayFramebuffer::setup() {
	display->initialize();
	buffer_size	= display->get_buffer_size();
	buffer		= display->get_buffer_ptr();
	interface->init(buffer, buffer_size, display->get_size());
}

void DisplayFramebuffer::set_fps(uint8_t fps) {period = 1000 / fps;}

void DisplayFramebuffer::show() {
	unsigned long millis_current = millis();
	static unsigned long millis_update_last = millis();
	static uint8_t counter = 0;
	if (millis_current - millis_update_last > period) {
		millis_update_last = millis_current;
		if (interface->ready()) display->update_buffer();
	}
}

void DisplayFramebuffer::test() {
	display->show_frame(nullptr, 0);
}

void DisplayFramebuffer::set_contrast(uint8_t contrast)				{display->set_contrast(contrast);}
void DisplayFramebuffer::set_pixel(uint8_t x, uint8_t y, bool c)	{display->set_pixel(x, y, c);}
