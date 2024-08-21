#include <graphics/elements/textbox.h>

TextBox::TextBox(uint16_t x, uint16_t y, const char * text_ptr, fontStruct _font, uint16_t _lenght, uint8_t _borders_h, uint8_t _borders_v, bool no_borders) {
	pos.x		= x;
	pos.y		= y;
	borders.x	= _borders_h;
	borders.y	= _borders_v;

	uint16_t text_size_h = strlen(text_ptr) * (_font.width + _font.spacing);
	if (_lenght < text_size_h) _lenght = text_size_h;
	label 		= new Label(x + _borders_h, y + _borders_v, text_ptr, _font);

	size.x		= _lenght;
	size.y		= label->get_size().y + borders.y * 2;

	box			= new Frame(x, y, size.x, size.y, false, no_borders);
	
	box->negative	= false;
	label->negative	= false;
}
void 		TextBox::draw(DrawArgs * args)	{
	box->draw(args);
	DrawArgs Largs = DrawArgs{args->tgt_ui, args->offset, args->negative};
	uint8_t delta = (box->get_size().x - 2 * borders.x) - label->get_size().x;
	if (allign == ALLIGN_RIGHT)		{Largs.offset.x += delta;}
	else if (allign == ALLIGN_EDGE)	{Largs.offset.x += delta / 2;}
	label->draw(&Largs);
}
void 		TextBox::set_text(char * _text)	{label->set_text(_text);}
char *		TextBox::get_text()				{return label->get_text();}
Coordinate	TextBox::get_size()				{return size;}
