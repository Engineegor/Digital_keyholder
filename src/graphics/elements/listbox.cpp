#include <graphics/elements/listbox.h>

ListBox::ListBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, fontStruct _font) {
	pos.x	= x;
	pos.y	= y;
	size.x	= w;
	size.y	= h;
	font	= _font;
	active_label = 0;
	max_active = (size.y - 2) / (font.height + 2);
	max_inscreen = max_active;
	if ((size.y - 2) % (font.height + 2) > 2) max_inscreen++; 
	slider_size.x = 2;
	slider_size.y = size.y - 2;
}

void	ListBox::draw(DrawArgs * args) {
	Coordinate abs;
	abs.x = pos.x + args->offset.x;
	abs.y = pos.y + args->offset.y;

	bool neg = negative ^ args->negative;

	uint16_t label_start 	= active_label - active_inscreen;
	uint16_t Label_end		= active_label + max_inscreen - active_inscreen;
	for (int i = label_start; i < Label_end; i++) {
		uint16_t curr_label_offset = i - active_label + active_inscreen;
		Coordinate curr_label_coord;
		curr_label_coord.x = abs.x + 1;
		curr_label_coord.y = abs.y + curr_label_offset * label[i].get_size().y + 1;
		DrawArgs cArgs = DrawArgs{args->tgt_ui, curr_label_coord, (i == active_label) ^ neg};
		if (0 <= i && i < label.size()) {
			label[i].draw(&cArgs);
		}
	}

	args->tgt_ui->draw_rect(abs.x, abs.y, size.x, size.y, !neg);

	if (label.size() >= max_inscreen) {
		args->tgt_ui->draw_line(abs.x + size.x - slider_size.x - 3, abs.y, abs.x + size.x - slider_size.x - 3, abs.y + size.y - 2, !neg);
		this->calc_slider(abs);
		args->tgt_ui->draw_rect(slider_pos.x, slider_pos.y, slider_size.x, slider_size.y, !neg);
	}
}

void	ListBox::add_position(const char * text_ptr) {
	uint16_t labels_len = size.x - slider_size.x - 6;
	TextBox new_label(1, 1, text_ptr, font, labels_len, 1, 1, true);
	label.push_back(new_label);
}
void	ListBox::remove_position(uint8_t _pos) {
	if (_pos < label.size()) {
		label.erase(label.begin() + _pos);
		if (active_label >= label.size()) {
			prev();
			active_inscreen = max_active - 2;
		}
	}
}

bool	ListBox::next() {
	if (active_label < label.size() - 1) {
		active_label++;
		if (active_inscreen < max_active - 1) active_inscreen++;
		return true;
	}
	return false;
}
bool	ListBox::prev() {
	if (active_label > 0) {
		active_label--;
		if (active_inscreen > 0) active_inscreen--;
		return true;
	}
	return false;
}

void	ListBox::set_active(uint8_t _pos)	{if (_pos < label.size()) active_label = _pos;}
uint8_t ListBox::get_active()				{return active_label;}
uint8_t	ListBox::get_on_screen()			{return active_inscreen;}
uint8_t	ListBox::get_list_size()			{return label.size();}

void	ListBox::calc_slider(Coordinate abs) {
	uint16_t delta = (size.y - 4) / label.size();
	slider_pos.y		= abs.y + 2 + delta * active_label;
	uint16_t pos_y_e	= abs.y + size.y - 2 - delta * (label.size() - active_label - 1);
	slider_size.y		= pos_y_e - slider_pos.y;
	slider_pos.x 		= size.x - slider_size.x + abs.x - 1;
}
