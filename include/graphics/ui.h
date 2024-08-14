#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <display/display_driver.h>

#define SCREEN_CHILD_MAX 30

enum drawState{START, END};

struct Element;
struct Screen;

class uiClass {
	public:
	void init(uint8_t * _buffer, size_t _buffer_size, displSize _size);
	void draw_pix(uint8_t x, uint8_t y, bool c = true);
	void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool c = true);
	void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool c = true);
	bool ready();
	void clean();
	void draw(drawState state);
	void set_screen(Screen * _screen_ptr);
	void update_screen();
	displSize size;

	private:
	Screen * screen_ptr;
	uint8_t * buffer;
	size_t buffer_size;
	bool is_ready = false;
};

struct Coordinate {uint16_t x, y;};
struct DrawArgs {
	uiClass * tgt_ui;
	Coordinate offset;
};

struct Element {
	virtual void draw(DrawArgs * args) = 0;
};

struct Frame : public Element {
	Coordinate pos;
	Coordinate size;
	Frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	void draw(DrawArgs * args);
};

struct Screen {
	public:
	bool redraw = false;
	void add_child(Element * child);
	void remove_child(Element * child);
	void draw(uiClass * ui_ptr);
	private:
	bool initiated = false;
	Element * children[SCREEN_CHILD_MAX];
};


#endif //UI_H
