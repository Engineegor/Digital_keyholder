#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <graphics/ui.h>

struct TextBox : Element{
	public:
	bool			negative	= false;
	Coordinate		pos;
	allignType		allign		= ALLIGN_LEFT;

	TextBox(uint16_t x, uint16_t y, const char * text_ptr, fontStruct _font, uint16_t _lenght, uint8_t _borders_h, uint8_t _borders_v, bool no_borders = false);
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

#endif //TEXTBOX_H