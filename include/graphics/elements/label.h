#ifndef LABEL_H
#define LABEL_H

#include <graphics/ui.h>

struct Label : public Element {
	public:
	bool 		negative = false;
	Coordinate	pos;
	Label(uint16_t x, uint16_t y, const char * text_ptr, fontStruct _font);
	~Label();
	void 		draw(DrawArgs * args);
	uint8_t		get_lenght();
	Coordinate	get_size();
	void		set_text(const char * _text);
	char *		get_text();

	private:
	char *			text;
	fontStruct		font;
	uint8_t			lenght;
	Coordinate		size;
};

#endif //LABEL_H