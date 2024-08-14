#pragma onse
#ifndef DISPLAY_H
#define DISPLAY_H

#include <display/GMG12864-06D.h>
#include <device_config.h>

/*static GMG12864_driver display_driver(
	DISPL_CS_PIN,
	DISPL_RSE_PIN,
	DISPL_RS_PIN,
	DISPL_SCL_PIN,
	DISPL_SI_PIN
);/**/

static GMG12864_driver display_driver(nullptr);

#endif //DISPLAY_H