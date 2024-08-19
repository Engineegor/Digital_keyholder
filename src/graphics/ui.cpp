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

Label::Label(uint16_t x, uint16_t y, const char * text_ptr, fontStruct _font) {
	pos.x	= x;
	pos.y	= y;
	text	= (char*)malloc(strlen(text_ptr));
	strcpy(text, text_ptr);
	lenght	= strlen(text);
	font	= _font;
	size.x	= (font.width + font.spacing) * lenght;
	size.y	= font.height;
}
void		Label::draw(DrawArgs * args) {
	if (redraw) {
		uint16_t	abs_x	= pos.x + args->offset.x;
		uint16_t	abs_y	= pos.y + args->offset.y;
		uint8_t		glyph_w	= font.width + font.spacing;
		bool		neg		= negative ^ args->negative;
		for (uint8_t i = 0; i < lenght; i++) {
			uint16_t ch_offset = font.width * (text[i] - font.start);
			for (uint16_t x = 0; x < glyph_w; x++) {
				uint8_t row_bitmap = (x < font.width) ? font.bitmap[ch_offset + x] : 0;
				for (uint8_t y = 0; y < font.height; y++) {
					bool pix = row_bitmap & 1U << y;
					args->tgt_ui->draw_pix(abs_x + x + i * glyph_w, abs_y + y, neg ^ pix);
				}
			}
		}
		redraw = false;
	}
}
void		Label::set_text(char * _text) {
	if (strlen(_text) <= lenght) {
		strcpy(text, _text);
	} else strncpy(text, _text, lenght);
	redraw = true;
}
char *		Label::get_text()	{return text;}
uint8_t		Label::get_lenght()	{return lenght;}
Coordinate	Label::get_size()	{return size;}
Label::~Label() {free(text);}

Frame::Frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool f) {
	pos.x	= x;
	pos.y	= y;
	size.x	= w;
	size.y	= h;
	filled	= f;
	this->children[0] = nullptr;
}
void Frame::add_child(Element * child) {
	if (!initiated) {
		initiated = true;
		for (uint8_t i = 0; i < FRAME_CHILD_MAX; i++) {children[i] = nullptr;}
	}

	for (uint8_t i = 0; i < FRAME_CHILD_MAX; i++) {
		if (children[i] == child) {break;}
		if (children[i] == nullptr) {
			children[i]	= child;
			redraw		= true;
			break;
		}
	}
}
void Frame::remove_child(Element * child) {
	uint8_t i = 0;
	for (; i < FRAME_CHILD_MAX; i++) {
		if (children[i] == child) {
			break;
		}
	}
	i++;
	for (; i < FRAME_CHILD_MAX; i++) {
		if (children[i] == nullptr) {break;}
		children[i - 1] = children[i];
	}
	children[i - 1]	= nullptr;
	redraw			= true;
}
void Frame::draw(DrawArgs * args) {
	if (redraw) {
		uint16_t	abs_x	= pos.x		+ args->offset.x;
		uint16_t	abs_y	= pos.y		+ args->offset.y;
		bool		neg		= negative ^ args->negative;

		args->tgt_ui->draw_rect(abs_x, abs_y, size.x, size.y, !neg);
		if (size.x > 2 && size.y > 2) args->tgt_ui->fill_rect(abs_x + 1, abs_y + 1, size.x - 2, size.y - 2, filled ^ neg);

		DrawArgs cArgs = DrawArgs{args->tgt_ui, Coordinate{abs_x, abs_y}, filled ^ neg};
		for (uint8_t i = 0; i < SCREEN_CHILD_MAX; i++) {
			if (this->children[i] == nullptr) break;
			else {
				this->children[i]->redraw = true;
				this->children[i]->draw(&cArgs);
			}
		}
		redraw = false;
	}
}
Coordinate	Frame::get_size() {return size;}

TextBox::TextBox(uint16_t x, uint16_t y, const char * text_ptr, fontStruct _font, uint16_t _lenght, uint8_t _borders_h, uint8_t _borders_v) {
	pos.x		= x;
	pos.y		= y;
	borders.x	= _borders_h;
	borders.y	= _borders_v;
	uint16_t text_size_h = strlen(text_ptr) * (_font.width + _font.spacing);
	if (_lenght < text_size_h) _lenght = text_size_h;
	label 		= new Label(x + _borders_h, y + _borders_v, text_ptr, _font);
	size.x		= _lenght + borders.x * 2;
	size.y		= label->get_size().y + borders.y * 2;
	box			= new Frame(x, y, size.x, size.y, false);
	box->negative	= false;
	label->negative	= false;
}
void 		TextBox::draw(DrawArgs * args)	{
	if (redraw = true) {
		box->redraw = true;
		box->draw(args);
		DrawArgs Largs = DrawArgs{args->tgt_ui, args->offset, args->negative};
		uint8_t delta = (box->get_size().x - 2 * borders.x) - label->get_size().x;
		if (allign == ALLIGN_RIGHT)		{Largs.offset.x += delta;}
		else if (allign == ALLIGN_EDGE)	{Largs.offset.x += delta / 2;}
		label->redraw = true;
		label->draw(&Largs);
	}
}
void 		TextBox::set_text(char * _text)	{label->set_text(_text);}
char *		TextBox::get_text()				{return label->get_text();}
Coordinate	TextBox::get_size()				{return size;}

ListBox::ListBox(uint16_t x, uint16_t y, uint16_t h, uint16_t v, fontStruct _font) {
	pos.x	= x;
	pos.y	= y;
	size.x	= h;
	size.y	= v;
	font	= _font;
	active_label = 0;
	max_inscreen = size.y / (font.height + 3);
	slider.x = 2;
	slider.y = size.y - 2;
}
void ListBox::add_position(const char * text_ptr) {
	TextBox new_label(0, 0, text_ptr, font, size.x - slider.x - 2, 1, 1);
	label.push_back(new_label);
}
uint8_t	ListBox::next() {
	if (active_label < label.size() - 1) {
		active_label++;
		if (active_inscreen < max_inscreen) active_inscreen++;
	}
	return active_label;
}
uint8_t	ListBox::prev() {
	if (active_label > 0) {
		active_label--;
		if (active_inscreen > 0) active_inscreen++;
	}
	return active_label;
}
void	ListBox::set_active(uint8_t _pos)	{if (_pos < label.size()) active_label = _pos;}
uint8_t ListBox::get_active()				{return active_label;}
void	ListBox::draw(DrawArgs * args) {
	if (redraw) {
 		bool neg = negative ^ args->negative;
		args->tgt_ui->draw_rect(pos.x, pos.y, size.x, size.y, !neg);
		args->tgt_ui->draw_line(pos.x + size.x - slider.x - 2, pos.y, pos.x + size.x - slider.x - 2, pos.y + size.y, !neg);

		

		redraw = false;
	}
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
	DrawArgs cArgs = DrawArgs{ui_ptr, Coordinate{0, 0}, negative};
	ui_ptr->draw(START);
	if (negative) ui_ptr->fill_rect(0, 0, ui_ptr->size.width, ui_ptr->size.height, true);
	for (uint8_t i = 0; i < SCREEN_CHILD_MAX; i++) {
		if (this->children[i] == nullptr) break;
		else this->children[i]->draw(&cArgs);
	}
	ui_ptr->draw(END);
}
uint8_t Screen::get_child_num() {
	uint8_t ret_num = 0;
	for (uint8_t i = 0; SCREEN_CHILD_MAX; i++) {
		if (children[i] == nullptr) break;
		else ret_num++;
	}
	return ret_num;	
}