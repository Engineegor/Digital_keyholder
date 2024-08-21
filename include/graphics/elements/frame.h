#ifndef FRAME_H
#define FRAME_H

#include <graphics/ui.h>

struct Frame : public Element {
	public:
	Coordinate	pos;
	bool		no_borders	= false;
	bool		filled		= false;
	bool		negative 	= false;
	Frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool f = false, bool b = false);
	void add_child(Element * child);
	void remove_child(Element * child);
	void draw(DrawArgs * args);
	Coordinate	get_size();

	private:
	bool		initiated	= false;
	Coordinate	size;
	Element *	children[FRAME_CHILD_MAX];
};

#endif //FRAME_H