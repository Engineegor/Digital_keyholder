#include <graphics/elements/frame.h>

Frame::Frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool f, bool b) {
	pos.x		= x;
	pos.y		= y;
	size.x		= w;
	size.y		= h;
	filled		= f;
	no_borders	= b;
}

void Frame::add_child(Element * child)		{ch.push_back(child);}
void Frame::remove_child(Element * child)	{for (int i = 0; i < ch.size(); i++) {if (ch[i] == child) ch.erase(ch.begin() + i);}}

void Frame::draw(DrawArgs * args) {
	uint16_t	abs_x	= pos.x		+ args->offset.x;
	uint16_t	abs_y	= pos.y		+ args->offset.y;
	bool		neg		= negative ^ args->negative;

	args->tgt_ui->draw_rect(abs_x, abs_y, size.x, size.y, !neg ^ no_borders);

	if (size.x > 2 && size.y > 2) args->tgt_ui->fill_rect(abs_x + 1, abs_y + 1, size.x - 2, size.y - 2, filled ^ neg);

	DrawArgs cArgs = DrawArgs{args->tgt_ui, Coordinate{abs_x, abs_y}, filled ^ neg};

	for (auto i : ch) {i->draw(&cArgs);}
}

Coordinate	Frame::get_size() {return size;}
