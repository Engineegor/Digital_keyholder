#include <os_config.h>
#include <manager/view_manager.h>
#include <controls/keys.h>
#include <graphics/ui.h>

TaskHandle_t 	hTask;

Coordinate screen_size{128, 64};

Screen main_screen;
TextBox acc_label(0, 0, "ListBox test 0", font5x7, screen_size.x, 2, 2);
ListBox acc_list(0, acc_label.get_size().y, screen_size.x, screen_size.y - acc_label.get_size().y, font5x7);

void views_setup(uiClass * _ui) {

	for (int i = 0; i < 9; i++) {
		String acc_name = "Account " + String(i);
		acc_list.add_position(acc_name.c_str());
	}
	acc_label.allign = ALLIGN_EDGE;
	main_screen.add_child(&acc_label);
	main_screen.add_child(&acc_list);

	_ui->set_screen(&main_screen);

	Serial.printf("Screen have %u elements\n\r", main_screen.get_child_num());
}

void views_task(uiClass * user_interface) {
	if (hid_check_key_state(KEY_UP) == KEYSTATE_HIT) {
		if (acc_list.next()) {
			Serial.printf("Pressed key %u\n\r", KEY_UP);
			String acc_label_text = "ListBox test " + String(acc_list.get_active());
			user_interface->update_screen();
		}
	}

	if (hid_check_key_state(KEY_DOWN) == KEYSTATE_HIT) {
		if (acc_list.prev()) {
			Serial.printf("Pressed key %u\n\r", KEY_DOWN);
			String acc_label_text = "ListBox test " + String(acc_list.get_active());
			user_interface->update_screen();
		}
	}
}