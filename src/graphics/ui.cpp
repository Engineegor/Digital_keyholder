#include <graphics/ui.h>

void uiClass::init(uint8_t * _buffer, size_t _buffer_size, displSize _size) {
	buffer		= _buffer;
	buffer_size = _buffer_size;
	size		= _size;
}

void uiClass::draw_pix(uint8_t x, uint8_t y, bool c) {
	if (x < size.width && y < size.height) {
		if (c)	buffer[x + 128 * (y / 8)] |= 1U << (y % 8);
		else	buffer[x + 128 * (y / 8)] &= ~(1U << (y % 8));
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
	draw_line(x,		y,		x + w,	y);		// upper
	draw_line(x,		y + h,	x + w,	y + h); // lower
	draw_line(x,		y,		x,		y + h);	// left
	draw_line(x + w,	y,		x + w,	y + h);	// right
}

bool uiClass::ready() {return is_ready;}
void uiClass::clean() {memset(buffer, 0, buffer_size);}
void uiClass::draw(drawState state) {
	if (state == START) {
		is_ready = false;
		clean();
	} else if	(state == END) is_ready = true;
}
void uiClass::update_screen() {screen_ptr->draw(this);}
void uiClass::set_screen(Screen * _screen_ptr) {
	screen_ptr = _screen_ptr;
	update_screen();
}

void Screen::add_child(Element * child) {
	if (!initiated) {
		initiated = true;
		for (uint8_t i = 0; i < SCREEN_CHILD_MAX; i++) {children[i] = nullptr;}
	}

	for (uint8_t i = 0; i < SCREEN_CHILD_MAX; i++) {
		if (children[i] == child) {break;}
		if (children[i] == nullptr) {
			children[i] = child;
			redraw = true;
			break;
		}
	}
}
void Screen::remove_child(Element * child) {
	uint8_t i = 0;
	for (; i < SCREEN_CHILD_MAX; i++) {
		if (children[i] == child) {
			redraw = true;
			break;
		}
	}
	i++;
	for (; i < SCREEN_CHILD_MAX; i++) {
		if (children[i] == nullptr) {break;}
		children[i - 1] = children[i];
	}
	children[i - 1] = nullptr;
}
void Screen::draw(uiClass * ui_ptr) {
	DrawArgs cArgs = DrawArgs{ui_ptr, Coordinate{0, 0}};
	ui_ptr->draw(START);
	for (uint8_t i = 0; i < SCREEN_CHILD_MAX; i++) {
		if (this->children[i] == nullptr) break;
		else this->children[i]->draw(&cArgs);
	}
	ui_ptr->draw(END);
	redraw = false;
}

Frame::Frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	pos.x	= x;
	pos.y	= y;
	size.x	= w;
	size.y	= h;
}
void Frame::draw(DrawArgs * args) {
	uint16_t abs_x = pos.x + args->offset.x;
	uint16_t abs_y = pos.y + args->offset.y;
	args->tgt_ui->draw_rect(abs_x, abs_y, size.x, size.y, true);
}