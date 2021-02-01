#ifndef UVC_TIMER_ITEM_H
#define UVC_TIMER_ITEM_H

#include <Arduino.h>

class UVC_Timer_Item
{
private:
  uint8_t _id;
  uint8_t _weekDay;
  uint8_t _hour;
  uint8_t _minute;
  uint8_t _repeat;
  uint16_t _duration;

public:
  UVC_Timer_Item(uint8_t id, uint8_t weekDay, uint8_t hour, uint8_t minute, uint32_t duration, uint8_t repeat);
  String getWeekdayAsString();
  String getStartTimeAsString();
  int getDuration();
  String getRepeatAsString();
};

#endif /*UVC_TIMER_ITEM_H*/