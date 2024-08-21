#ifndef LISTBOX_H
#define LISTBOX_H

#include <graphics/ui.h>

struct ListBox : Element {
	public:
	bool	negative = false;

	ListBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, fontStruct _font);
	void	add_position(const char * text_ptr);
	void	remove_position(uint8_t _pos);
	void	draw(DrawArgs * args);
	void	set_active(uint8_t _pos);
	uint8_t	get_active();
	uint8_t	get_on_screen();
	bool	next();
	bool	prev();
	uint8_t	get_list_size();

	private:
	Coordinate pos;
	Coordinate size;
	fontStruct font;
	std::vector<TextBox> label;
	uint8_t active_label	= 0;
	uint8_t	active_inscreen	= 0;
	uint8_t max_inscreen	= 0;
	uint8_t max_active		= 0;
	Frame * box;

	const uint8_t borders	= 2;
	Coordinate	slider_size;
	Coordinate	slider_pos;

	void calc_slider(Coordinate abs);
};

#endif //LISTBOX_H