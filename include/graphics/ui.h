#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <display/display_driver.h>

#define SCREEN_CHILD_MAX	30
#define FRAME_CHILD_MAX		10

enum drawState{START, END};

struct Element;
struct Screen;

class uiClass {
	public:
	void init(uint8_t * _buffer, size_t _buffer_size, displSize _size);
	void draw_pix(uint8_t x, uint8_t y, bool c = true);
	void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool c = true);
	void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool c = true);
	void fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool c);
	void put_byte(uint8_t x, uint8_t y, uint8_t b);
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

struct Coordinate	{uint16_t x, y;};
struct DrawArgs		{uiClass * tgt_ui; Coordinate offset; bool negative;};
struct Element		{virtual void draw(DrawArgs * args) = 0;};

struct Label : public Element {
	public:
	bool			redraw = false;
	bool			negative;
	Coordinate		pos;
	const char *	text;
	size_t			lenght;
	Label(uint16_t x, uint16_t y, const char * text_ptr, size_t text_len);
	void draw(DrawArgs * args);

	private:

};

struct Frame : public Element {
	public:
	bool		redraw = false;
	Coordinate	pos;
	Coordinate	size;
	bool		negative;
	Frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	void add_child(Element * child);
	void remove_child(Element * child);
	void draw(DrawArgs * args);

	private:
	bool		initiated = false;
	Element *	children[FRAME_CHILD_MAX];
};

struct Screen {
	public:
	bool redraw = false;
	void add_child(Element * child);
	void remove_child(Element * child);
	void draw(uiClass * ui_ptr);
	
	private:
	bool		initiated = false;
	Element *	children[SCREEN_CHILD_MAX];
};


#endif //UI_H
