
#ifndef UVC_GUI_H
#define UVC_GUI_H

#include <lvgl.h>
#include <uvcero.h>

void create_gui(void);
void showSplashScreen(void);

void updateFanSpeed(uint8_t fanSpeed);
void updateLampState(uint8_t lampsState);

#endif /*UVC_GUI_H*/