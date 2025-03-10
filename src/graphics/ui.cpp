#include <vector>

#include <graphics/ui.h>

void uiClass::init(uint8_t * _buffer, size_t _buffer_size, displSize _size) {
	buffer		= _buffer;
	buffer_size = _buffer_size;
	size		= _size;
}

void uiClass::draw_pix(uint8_t x, uint8_t y, bool c) {
	if (x < size.width && y < size.height) {
		if (c)	buffer[x + size.width * (y / 8)] |= 1U << (y % 8);
		else	buffer[x + size.width * (y / 8)] &= ~(1U << (y % 8));
	}
}
void uiClass::draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool c) {
	uint16_t dx = abs(x2 - x1);
	uint16_t dy = abs(y2 - y1);
	short int sx = x1 < x2 ? 1 : -1;
	short int sy = y1 < y2 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;

	while (true) {
		draw_pix(x1, y1, c);

		if (x1 == x2 && y1 == y2) {break;}

		int e2 = err;

		if (e2 > -dx) {
			err -= dy;
			x1 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y1 += sy;
		}
	}
}
void uiClass::draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool c) {
	for (int i = x; i < x + w; i++) draw_pix(i, 		y,			c);			// upper
	for (int i = x; i < x + w; i++) draw_pix(i, 		y + h - 1,	c);	// lower
	for (int i = y; i < y + h; i++) draw_pix(x,			i,			c);			// left
	for (int i = y; i < y + h; i++) draw_pix(x + w - 1,	i,			c);			// right	
}
void uiClass::fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool c) {
	for (int i = x; i < x + w; i++) {
		for (int j = y; j < y + h; j++) draw_pix(i, j, c);
	}
}

bool uiClass::ready()			{return is_ready;}
void uiClass::clean()			{memset(buffer, 0, buffer_size);}
void uiClass::update_screen()	{screen_ptr->draw(this);}
void uiClass::draw(drawState state) {
	if (state == START) {
		is_ready = false;
		clean();
	} else if	(state == END) is_ready = true;
}
void uiClass::set_screen(Screen * _screen_ptr) {
	screen_ptr = _screen_ptr;
	update_screen();
}

void	Screen::draw(uiClass * ui_ptr) {
	DrawArgs cArgs = DrawArgs{ui_ptr, Coordinate{0, 0}, negative};
	ui_ptr->draw(START);
	if (negative) ui_ptr->fill_rect(0, 0, ui_ptr->size.width, ui_ptr->size.height, true);
	for (auto i : ch) {i->draw(&cArgs);}
	ui_ptr->draw(END);
}
void	Screen::add_child(Element * child)		{ch.push_back(child);}
void	Screen::remove_child(Element * child)	{for (int i = 0; i < ch.size(); i++) {if (ch[i] == child) ch.erase(ch.begin() + i);}}
uint8_t	Screen::get_child_num() 				{return ch.size();}