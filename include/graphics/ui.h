#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <string.h>

#include <display/display_driver.h>
#include <fonts/fonts.h>
#include <common.h>

enum drawState{START, END};
enum allignType{ALLIGN_LEFT, ALLIGN_EDGE, ALLIGN_RIGHT};

struct Screen;

class uiClass {
	public:
	void init(uint8_t * _buffer, size_t _buffer_size, displSize _size);
	void draw_pix(uint8_t x, uint8_t y, bool c = true);
	void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool c = true);
	void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool c = true);
	void fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool c);
	bool ready();
	void clean();
	void draw(drawState state);
	void set_screen(Screen * _screen_ptr);
	void update_screen();
	displSize size;

	private:
	Screen *	screen_ptr;
	uint8_t *	buffer;
	size_t		buffer_size;
	bool		is_ready = false;
};

struct DrawArgs {
	uiClass * tgt_ui;
	Coordinate offset;
	bool negative;
};

struct Element		{virtual void draw(DrawArgs * args) = 0; bool redraw = true;};

struct Screen {
	public:
	bool negative = false;
	void add_child(Element * child);
	void remove_child(Element * child);
	void draw(uiClass * ui_ptr);
	uint8_t get_child_num();
	
	private:
	std::vector<Element*> ch;
};

#endif //UI_H
