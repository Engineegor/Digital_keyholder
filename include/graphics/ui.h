#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <string.h>
#include <vector>

#include <display/display_driver.h>
#include <fonts/fonts.h>

#define SCREEN_CHILD_MAX	30
#define FRAME_CHILD_MAX		10
#define LISTBOX_CHILD_MAX	64

enum drawState{START, END};
enum allignType{ALLIGN_LEFT, ALLIGN_EDGE, ALLIGN_RIGHT};

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
	bool force;
};
struct Element		{virtual void draw(DrawArgs * args) = 0; bool redraw = true;};

struct Label : public Element {
	public:
	bool			redraw		= true;
	bool			negative	= false;
	Coordinate		pos;
	Label(uint16_t x, uint16_t y, const char * text_ptr, fontStruct _font);
	~Label();
	void 	draw(DrawArgs * args);
	uint8_t		get_lenght();
	Coordinate	get_size();
	void		set_text(char * _text);
	char *		get_text();

	private:
	char *			text;
	fontStruct		font;
	uint8_t			lenght;
	Coordinate		size;
};

struct Frame : public Element {
	public:
	Coordinate	pos;
	bool		redraw		= true;
	bool		filled		= false;
	bool		negative 	= false;
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
	bool			redraw		= true;
	bool			negative	= false;
	Coordinate		pos;
	allignType		allign		= ALLIGN_LEFT;

	TextBox(uint16_t x, uint16_t y, const char * text_ptr, fontStruct _font, uint16_t _lenght, uint8_t _borders_h, uint8_t _borders_v);
	void		draw(DrawArgs * args);
	void		set_text(char * _text);
	char *		get_text();
	Coordinate	get_size();

	private:
	Coordinate borders;
	Coordinate size;
	Label * label;
	Frame * box;
};

struct ListBox : Element {
	public:
	bool	negative = false;

	ListBox(uint16_t x, uint16_t y, uint16_t h, uint16_t v, fontStruct _font);
	void	add_position(const char * text_ptr);
	void	remove_position(uint8_t _pos);
	void	draw(DrawArgs * args);
	void	set_active(uint8_t _pos);
	uint8_t	get_active();
	uint8_t	next();
	uint8_t	prev();
	uint8_t	get_list_size();

	private:
	Coordinate pos;
	Coordinate size;
	fontStruct font;
	std::vector<TextBox> label;
	uint8_t active_label	= 0;
	uint8_t	active_inscreen	= 0;
	uint8_t max_inscreen	= 0;
	Frame * box;

	const uint8_t borders	= 2;
	Coordinate	slider;
};

struct Screen {
	public:
	bool redraw = false;
	bool negative = false;
	void add_child(Element * child);
	void remove_child(Element * child);
	void draw(uiClass * ui_ptr);
	uint8_t get_child_num();
	
	private:
	bool		initiated = false;
	Element *	children[SCREEN_CHILD_MAX];
};


#endif //UI_H
