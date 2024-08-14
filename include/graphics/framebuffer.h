#include <display/display.h>
#include <display/display_driver.h>
#include <graphics/ui.h>

class DisplayFramebuffer {
public:
	DisplayFramebuffer(DisplayDriver * disp, uiClass * _interface);
	void setup();
	void show();
	void test();
	void set_contrast(uint8_t contrast);
	void set_fps(uint8_t fps);
	void set_pixel(uint8_t x, uint8_t y, bool c);
private:
	DisplayDriver * display;
	uiClass * interface;
	uint8_t * buffer;
	size_t buffer_size;
	uint16_t period = 20;
};