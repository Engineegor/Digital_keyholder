#include <device_config.h>
#include <display/GMG12864-06D.h>
#include <esp32-hal-gpio.h>
#include <esp_err.h>
#include "driver/spi_master.h"
#include <U8g2lib.h>

//U8G2_ST7565_ZOLEN_128X64_F_4W_SW_SPI display(U8G2_R0, DISPL_SCL_PIN, DISPL_SI_PIN, DISPL_CS_PIN, DISPL_RS_PIN, DISPL_RSE_PIN);	// Worked, offseted entire display >> 4 and vertical mirroring
//U8G2_ST7565_EA_DOGM128_F_4W_SW_SPI display(U8G2_R0, DISPL_SCL_PIN, DISPL_SI_PIN, DISPL_CS_PIN, DISPL_RS_PIN, DISPL_RSE_PIN);		// Worked, offseted entire display >> 4
//U8G2_ST7565_64128N_F_4W_SW_SPI display(U8G2_R2, DISPL_SCL_PIN, DISPL_SI_PIN, DISPL_CS_PIN, DISPL_RS_PIN, DISPL_RSE_PIN);			// Worked, low brightness
//U8G2_ST7565_LM6059_F_4W_SW_SPI display(U8G2_R0, DISPL_SCL_PIN, DISPL_SI_PIN, DISPL_CS_PIN, DISPL_RS_PIN, DISPL_RSE_PIN);			// Worked, flipped upper and lower halfs, dots at left 
//U8G2_ST7565_LX12864_F_4W_SW_SPI display(U8G2_R0, DISPL_SCL_PIN, DISPL_SI_PIN, DISPL_CS_PIN, DISPL_RS_PIN, DISPL_RSE_PIN);			// Some horizontal offset on center, dots at left
//U8G2_ST7565_ERC12864_F_4W_SW_SPI display(U8G2_R0, DISPL_SCL_PIN, DISPL_SI_PIN, DISPL_CS_PIN, DISPL_RS_PIN, DISPL_RSE_PIN);		// Worked well
U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI display(U8G2_R0, DISPL_SCL_PIN, DISPL_SI_PIN, DISPL_CS_PIN, DISPL_RS_PIN, DISPL_RSE_PIN);	// Worked well
//U8G2_ST7565_NHD_C12864_F_4W_SW_SPI display(U8G2_R0, DISPL_SCL_PIN, DISPL_SI_PIN, DISPL_CS_PIN, DISPL_RS_PIN, DISPL_RSE_PIN);		// Worked, low brightness
//U8G2_ST7565_JLX12864_F_4W_SW_SPI display(U8G2_R2, DISPL_SCL_PIN, DISPL_SI_PIN, DISPL_CS_PIN, DISPL_RS_PIN, DISPL_RSE_PIN);		// Worked, low brightness

GMG12864_driver::GMG12864_driver(const gpio_num_t _cs, const gpio_num_t _rse, const gpio_num_t _rs, const gpio_num_t _scl, const gpio_num_t _si) {
/*	pin_cs	= _cs;
	pin_rse	= _rse;
	pin_rs	= _rs;
	pin_scl	= _scl;
	pin_si	= _si;*/
}

GMG12864_driver::GMG12864_driver(void*) {}

void GMG12864_driver::initialize() {
	display.begin();
	display.setContrast(70);
	delay(100);
}

void GMG12864_driver::show_frame(const uint8_t * frame, size_t len) {
	display.clearBuffer();
	display.enableUTF8Print(); 
	display.setFont(u8g2_font_6x13B_t_cyrillic);	
	display.setCursor(4, 22);
	display.print("Привет");
	display.setFont(u8g2_font_cu12_t_cyrillic);
	display.setCursor(4, 42);
	display.print("Тест 123 test");
	display.sendBuffer();
}

uint8_t * GMG12864_driver::get_buffer_ptr() {
	return display.getBufferPtr();
}

void GMG12864_driver::update_buffer() {
	display.sendBuffer();	
}

void GMG12864_driver::set_bright(uint8_t bright) {
}

void GMG12864_driver::set_contrast(uint8_t contrast) {
	display.setContrast(contrast);
}

size_t GMG12864_driver::get_buffer_size() {
	return display.getBufferTileHeight() * display.getBufferTileWidth() * 8;
}

displSize GMG12864_driver::get_size() {
	displSize ret_size;
	ret_size.width = display.getBufferTileWidth() * 8;
	ret_size.height = display.getBufferTileHeight() * 8;
	return ret_size;
}

void GMG12864_driver::set_pixel(uint8_t x, uint8_t y, bool c) {
	if (c)	display.getBufferPtr()[x + 128 * (y / 8)] |= 1U << (y % 8);
	else	display.getBufferPtr()[x + 128 * (y / 8)] |= ~(1U << (y % 8));
}