#ifndef UVCERO_H
#define UVCERO_H

#include <uvc_tasks.h>
#include <Classes\UVC_Lamp.h>
#include <Classes\UVC_Fan.h>
#include <Classes\UVC_Timer_Item.h>
#include <pt_sans_regular_14_subpx.h>
#include <uvc_filesystem.h>
#include <ArduinoJson.h>

#define LAMP_SWITCH_PIN 13
#define FAN_PWM_PIN 15

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

#define I2C_ADDR_RTC 0x68
#define I2C_ADDR_PCF 0x38 //0x20 // 0x71

#define MAX_TIMER_COUNT 21

#define FAN_CALIBRATED_OFF 0                /* 0/6 */
#define FAN_CALIBRATED_LOW 119      //43    /* 1/6 */
#define FAN_CALIBRATED_MID 135      //85    /* 2/6 */
#define FAN_CALIBRATED_HIGH 157     //128   /* 3/6 */
#define FAN_CALIBRATED_FULL 255             /* 6/6 */

enum {
    FAN_SPEED_OFF,
    FAN_SPEED_LOW,
    FAN_SPEED_MEDIUM,
    FAN_SPEED_HIGH,
    FAN_SPEED_FULL,
    _FAN_SPEED_LAST
};
typedef uint8_t fan_speed_t;

enum {
    LAMPS_OFF,
    LAMPS_ON
};
typedef uint8_t lamp_state_t;

enum {
    TIMER_ITEM_WEEKDAY_MONDAY,
    TIMER_ITEM_WEEKDAY_TUESDAY,
    TIMER_ITEM_WEEKDAY_WEDNESDAY,
    TIMER_ITEM_WEEKDAY_THURSDAY,
    TIMER_ITEM_WEEKDAY_FRIDAY,
    TIMER_ITEM_WEEKDAY_SATURDAY,
    TIMER_ITEM_WEEKDAY_SUNDAY,
    _TIMER_ITEM_WEEKDAY_LAST
};
typedef uint8_t timer_item_weekday_t;

enum {
    TIMER_ITEM_REPEAT_NONE,
    TIMER_ITEM_REPEAT_HOURLY,
    TIMER_ITEM_REPEAT_DAILY,
    TIMER_ITEM_REPEAT_WEEKLY,
    _TIMER_ITEM_REPEAT_LAST
};
typedef uint8_t timer_item_repeat_t;

extern UVC_Lamp* lamps[4];
extern UVC_Fan *fans[4];

extern UVC_Timer_Item* timerItems[MAX_TIMER_COUNT];
extern int currentTimerItemCount;

extern int fanFreq;
extern int fanChannel;
extern int fanResolution;

extern fan_speed_t currentFanSpeed;
extern lamp_state_t currentLampState;

extern bool SD_CARD_AVAILABLE;

extern DynamicJsonDocument configDoc;
extern JsonObject config;

extern const char* serialNo;

void updateConfig();
void readConfig();

#endif /*UVCERO_H*/