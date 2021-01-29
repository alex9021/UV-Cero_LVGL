#ifndef UVC_COMPONENT_H
#define UVC_COMPONENT_H

#include <Arduino.h>

class UVC_Component
{
private:
  uint8_t _id;
  uint32_t _operatingLife;
  uint16_t _serviceLife;
  uint32_t _lastService;

protected:
  void setId(uint8_t id);
  void setOperatingLife(uint32_t operatingLife);
  void setServiceLife(uint16_t serviceLife);
  void setlastService(uint32_t lastService);

public:
  UVC_Component(uint8_t id, uint32_t operatingLife, uint16_t serviceLife, uint32_t lastService);
  ~UVC_Component();
  uint8_t getId();
  uint32_t getOperatingLife();
  uint16_t getServiceLife();
  uint32_t getlastService();
  uint32_t service(uint32_t currentTimeStamp);
};

#endif /*UVC_COMPONENT_H*/