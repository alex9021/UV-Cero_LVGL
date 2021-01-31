#ifndef UVC_TASKS_H
#define UVC_TASKS_H

#include <Arduino.h>
#include <Wire.h>
#include <FS.h>
#include <SPIFFS.h>

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <PCF8574.h>
#include <uRTCLib.h>
#include <uvcero.h>
#include <uvc_gui.h>
#include <uvc_filesystem.h>

extern TaskHandle_t thGUI;
extern TaskHandle_t thSensors;
extern TaskHandle_t thFanController;
extern TaskHandle_t thLampController;
extern TaskHandle_t thRTCUpdater;

extern QueueHandle_t xFAN_SPEED;

extern PCF8574 gpio_expander;
extern uRTCLib rtc;

/*!
    @brief gui task loop
*/
void guiTaskLoop(void *parameter);

/*!
    @brief sensors task loop
*/
void sensorsTaskLoop(void *parameter);

/*!
    @brief fan controller task loop
*/
void fanControllerTaskLoop(void *parameter);

/*!
    @brief lamp controller task loop
*/
void lampControllerTaskLoop(void *parameter);

/*!
    @brief lamp controller task loop
*/
void rtcUpdaterTaskLoop(void *parameter);

#endif /*UVC_TASKS_H*/