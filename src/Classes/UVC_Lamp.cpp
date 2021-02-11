#include <Classes/UVC_Lamp.h>
UVC_Lamp::UVC_Lamp(
    uint8_t id,
    uint32_t operatingLife,
    uint16_t serviceLife,
    uint32_t lastService,
    uint16_t currentLumen,
    bool state) : UVC_Component(id, operatingLife, serviceLife, lastService, state)
{
    setCurrentLumen(currentLumen);
}

void UVC_Lamp::setCurrentLumen(uint16_t lumen)
{
    _currentLumen = lumen;
}


uint16_t UVC_Lamp::getCurrentLumen()
{
    return _currentLumen;
}