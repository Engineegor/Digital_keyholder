
//#include <U8g2lib.h>
#include <display/display_driver.h>
#include <driver/spi_master.h>
#include <device_config.h>
#include <U8g2lib.h>

class GMG12864_driver: public DisplayDriver {
	public:
	GMG12864_driver(const gpio_num_t _cs, const gpio_num_t _rse, const gpio_num_t _rs, const gpio_num_t _scl, const gpio_num_t _si);
	GMG12864_driver(void*);
    void initialize();
	void show_frame(const uint8_t * frame, size_t len);
	void set_bright(uint8_t bright);
	void set_contrast(uint8_t contrast);
	size_t get_buffer_size();
	uint8_t * get_buffer_ptr();
	void update_buffer();
	displSize get_size();
	void set_pixel(uint8_t x, uint8_t y, bool c);
};