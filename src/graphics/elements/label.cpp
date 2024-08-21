#include <graphics/elements/label.h>

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

void Label::draw(DrawArgs * args) {
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
}

void Label::set_text(const char * _text) {
	if (strlen(_text) <= lenght) {
		strcpy(text, _text);
	} else strncpy(text, _text, lenght);
}

char *		Label::get_text()	{return text;}
uint8_t		Label::get_lenght()	{return lenght;}
Coordinate	Label::get_size()	{return size;}
Label::~Label() 				{free(text);}