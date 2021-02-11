#ifndef UVC_LAMP_H
#define UVC_LAMP_H

#include <Arduino.h>
#include <Classes/UVC_Component.h>

class UVC_Lamp : public UVC_Component
{
private:
  uint16_t _currentLumen;

public:
  UVC_Lamp(uint8_t id, uint32_t operatingLife, uint16_t serviceLife, uint32_t lastService, uint16_t currentLumen, bool state);
  //~UVC_Lamp();
  void setCurrentLumen(uint16_t lumen);
  uint16_t getCurrentLumen();
};

#endif /*UVC_LAMP_H*/