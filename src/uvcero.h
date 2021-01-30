#ifndef UVCERO_H
#define UVCERO_H

#include <uvc_tasks.h>
#include <Classes\UVC_Lamp.h>
#include <Classes\UVC_Fan.h>

#define LAMP_SWITCH_PIN 13
#define FAN_PWM_PIN 15

#define FAN_CALIBRATED_OFF 0       /* 0/3 */
#define FAN_CALIBRATED_LOW 85      /* 1/3 */
#define FAN_CALIBRATED_MID 170     /* 2/3 */
#define FAN_CALIBRATED_HIGH 255    /* 3/3 */

enum {
    FAN_SPEED_OFF,
    FAN_SPEED_LOW,
    FAN_SPEED_MEDIUM,
    FAN_SPEED_HIGH,
    _FAN_SPEED_LAST
};
typedef uint8_t fan_speed_t;

enum {
    LAMPS_OFF,
    LAMPS_ON
};
typedef uint8_t lamp_state_t;

extern UVC_Lamp* lamps[4];
extern UVC_Fan *fans[4];

extern int fanFreq;
extern int fanChannel;
extern int fanResolution;

extern fan_speed_t currentFanSpeed;
extern lamp_state_t currentLampState;

extern bool SD_CARD_AVAILABLE;

#endif /*UVCERO_H*/
