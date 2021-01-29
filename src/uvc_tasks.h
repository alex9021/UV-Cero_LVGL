#ifndef UVC_TASKS_H
#define UVC_TASKS_H

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <uvc_gui.h>
#include <uvcero.h>

extern TaskHandle_t thGUI;
extern TaskHandle_t thSensors;
extern TaskHandle_t thFanController;
extern TaskHandle_t thLampController;

extern QueueHandle_t xFAN_SPEED;

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

#endif /*UVC_TASKS_H*/