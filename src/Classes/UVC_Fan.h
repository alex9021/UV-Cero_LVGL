#ifndef UVC_FAN_H
#define UVC_FAN_H

#include <Arduino.h>
#include <Classes/UVC_Component.h>

class UVC_Fan : public UVC_Component
{
private:
  uint16_t _currentRPM;

public:
  UVC_Fan(uint8_t id, uint32_t operatingLife, uint16_t serviceLife, uint32_t lastService, uint16_t currentRPM, bool state);
  //~UVC_Fan();
  void setCurrentRPM(uint16_t rpm);
  uint16_t getCurrentRPM();
};

#endif /*UVC_FAN_H*/