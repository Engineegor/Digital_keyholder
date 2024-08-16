#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <display/display_driver.h>
#include <fonts/fonts.h>
#include <string.h>

#define SCREEN_CHILD_MAX	30
#define FRAME_CHILD_MAX		10

enum drawState{START, END};
enum allignType{LEFT, EDGE, RIGHT};

struct Element;
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

struct Coordinate	{uint16_t x, y;};
struct DrawArgs {
	uiClass * tgt_ui;
	Coordinate offset;
	bool negative;
};
struct Element		{virtual void draw(DrawArgs * args) = 0;};

struct Label : public Element {
	public:
	bool			redraw = false;
	bool			negative;
	Coordinate		pos;
	char *			text;
	Label(uint16_t x, uint16_t y, const char * text_ptr, fontStruct _font);
	void 	draw(DrawArgs * args);
	uint8_t		get_lenght();
	Coordinate	get_size();

	private:
	fontStruct		font;
	uint8_t			lenght;
	Coordinate		size;
};

struct Frame : public Element {
	public:
	Coordinate	pos;
	bool		filled		= false;
	bool		negative;
	Frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool f = false);
	void add_child(Element * child);
	void remove_child(Element * child);
	void draw(DrawArgs * args);
	Coordinate	get_size();

	private:
	bool		initiated	= false;
	Coordinate	size;
	Element *	children[FRAME_CHILD_MAX];
};

struct TextBox : Element{
	public:
	bool			negative;
	Coordinate		pos;
	allignType		allign = LEFT;
	const char *	text;

	TextBox(uint16_t x, uint16_t y, const char * text_ptr, fontStruct _font, uint8_t _lenght, uint8_t _borders_h, uint8_t _borders_v);
	void		draw(DrawArgs * args);
	Coordinate	get_size();

	private:
	Coordinate borders;
	Coordinate size;
	Label * label;
	Frame * box;
};

struct Screen {
	public:
	bool redraw = false;
	bool negative;
	void add_child(Element * child);
	void remove_child(Element * child);
	void draw(uiClass * ui_ptr);
	
	private:
	bool		initiated = false;
	Element *	children[SCREEN_CHILD_MAX];
};


#endif //UI_H
