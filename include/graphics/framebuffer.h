#include <display/display.h>
#include <display/display_driver.h>
#include <graphics/ui.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/event_groups.h>

class DisplayFramebuffer {
public:
	DisplayFramebuffer(DisplayDriver * disp, uiClass * _interface);
	void setup();
	bool show();
	void set_contrast(uint8_t contrast);
	void set_fps(uint8_t fps);
	uint16_t get_period();
	void set_pixel(uint8_t x, uint8_t y, bool c);
private:

	DisplayDriver *	display;
	uiClass *		interface;
	uint8_t *		buffer;
	size_t			buffer_size;
	uint16_t		period = 50; // (1000 / 20fps)
	TaskHandle_t 	hTask;
};