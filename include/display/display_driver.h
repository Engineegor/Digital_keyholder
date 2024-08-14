#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <device_config.h>

struct displSize {
	uint16_t width;
	uint16_t height;
};

class DisplayDriver {
public:
	virtual void initialize();
	virtual void show_frame(const uint8_t * frame, size_t len);
	virtual void set_bright(uint8_t bright);
	virtual void set_contrast(uint8_t contrast);
	virtual size_t get_buffer_size();
	virtual uint8_t * get_buffer_ptr();
	virtual void update_buffer();
	virtual displSize get_size();
	virtual void set_pixel(uint8_t x, uint8_t y, bool c);
};

#endif //DISPLAY_DRIVER_H