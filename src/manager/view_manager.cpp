#include <os_config.h>
#include <manager/view_manager.h>
#include <controls/keys.h>

TaskHandle_t 	hTask;

Screen main_screen;
TextBox tbox[] = {
	TextBox(0, 10,	"Account 0", font5x7, 104, 2, 2),
	TextBox(0, 22,	"Account 1", font5x7, 104, 2, 2),
	TextBox(0, 34,	"Account 2", font5x7, 104, 2, 2),
	TextBox(0, 46,	"Account 3", font5x7, 104, 2, 2)
};

void views_setup(uiClass * _ui) {

	uint8_t tbox_len = sizeof(tbox) / sizeof(TextBox);
	for (uint8_t i = 0; i < tbox_len; i++) main_screen.add_child(&tbox[i]);

	_ui->set_screen(&main_screen);

	Serial.printf("Screen have %u elements\n\r", main_screen.get_child_num());
}

void views_task(uiClass * user_interface) {
	if (hid_check_key_state(KEY_UP) == KEYSTATE_HIT) {
		Serial.printf("Pressed key %u\n\r", KEY_UP);
		if (tbox[1].allign == ALLIGN_LEFT)		tbox[1].allign = ALLIGN_EDGE;
		else if (tbox[1].allign == ALLIGN_EDGE)	tbox[1].allign = ALLIGN_RIGHT;
		user_interface->update_screen();
	}

	if (hid_check_key_state(KEY_DOWN) == KEYSTATE_HIT) {
		Serial.printf("Pressed key %u\n\r", KEY_DOWN);
		if (tbox[1].allign == ALLIGN_RIGHT)		tbox[1].allign = ALLIGN_EDGE;
		else if (tbox[1].allign == ALLIGN_EDGE)	tbox[1].allign = ALLIGN_LEFT;
		user_interface->update_screen();
	}
}