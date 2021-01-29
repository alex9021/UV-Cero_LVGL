#include <Classes/UVC_Fan.h>
UVC_Fan::UVC_Fan(
    uint8_t id,
    uint32_t operatingLife,
    uint16_t serviceLife,
    uint32_t lastService,
    uint16_t currentRPM) : UVC_Component(id, operatingLife, serviceLife, lastService)
{
    setCurrentRPM(currentRPM);
}

void UVC_Fan::setCurrentRPM(uint16_t rpm)
{
    _currentRPM = rpm;
}

uint16_t UVC_Fan::getCurrentRPM()
{
    return _currentRPM;
}