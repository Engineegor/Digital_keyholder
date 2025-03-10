#include <graphics/framebuffer.h>
#include <freertos/task.h>
#include <os_config.h>
#include <esp32-hal-log.h>

static char LOG_TAG[] = "FB";

DisplayFramebuffer::DisplayFramebuffer(DisplayDriver * disp, uiClass * _interface) {
	display		= disp;
	interface	= _interface;
}

extern "C" void FbTaskFunc( void * pvParameter );

void FbTaskFunc(void * pvParameter) {
	ESP_LOGV(LOG_TAG, "Running task");
	 DisplayFramebuffer * fb = static_cast<DisplayFramebuffer*> ( pvParameter );

	 while(1) {
		if (fb->show())	vTaskDelay(pdMS_TO_TICKS(fb->get_period()));
	 }
}

void DisplayFramebuffer::setup() {
	display->initialize();
	buffer_size	= display->get_buffer_size();
	buffer		= display->get_buffer_ptr();
	interface->init(buffer, buffer_size, display->get_size());

	ESP_LOGV(LOG_TAG, "Creating task");
	if(xTaskCreate(
		FbTaskFunc,
		"FBuf",
		4096,
		this,
		OS_TASK_PRIORITY_FRAMEBUFFER,
		&hTask
	) != pdPASS) {
		ESP_LOGE(LOG_TAG, "Task creation failed!");
	}
}

void 		DisplayFramebuffer::set_fps(uint8_t fps)					{period = 1000 / fps;}
uint16_t	DisplayFramebuffer::get_period()							{return period;}
bool		DisplayFramebuffer::show() {
	if (interface->ready()) {
		display->update_buffer();
		return true;
	} else return false;
}
void		DisplayFramebuffer::set_contrast(uint8_t contrast)			{display->set_contrast(contrast);}
void		DisplayFramebuffer::set_pixel(uint8_t x, uint8_t y, bool c)	{display->set_pixel(x, y, c);}
