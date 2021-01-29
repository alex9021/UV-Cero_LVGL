#include <Classes/UVC_Component.h>

UVC_Component::UVC_Component(uint8_t id, uint32_t operatingLife, uint16_t serviceLife, uint32_t lastService)
{
    setId(id);
    setOperatingLife(operatingLife);
    setServiceLife(serviceLife);
    setlastService(lastService);
}

UVC_Component::~UVC_Component()
{
    //deconstructer
}

uint8_t UVC_Component::getId()
{
    return _id;
}

uint32_t UVC_Component::getOperatingLife()
{
    return _operatingLife;
}

uint16_t UVC_Component::getServiceLife()
{
    return _serviceLife;
}

uint32_t UVC_Component::getlastService()
{
    return _serviceLife;
}

uint32_t UVC_Component::service(uint32_t currentTimeStamp)
{
    setOperatingLife(0);
    setlastService(currentTimeStamp);
    return getlastService();
}

void UVC_Component::setId(uint8_t id)
{
    _id = id;
}

void UVC_Component::setOperatingLife(uint32_t operatingLife)
{
    _operatingLife = operatingLife;
}

void UVC_Component::setServiceLife(uint16_t serviceLife)
{
    _serviceLife = serviceLife;
}

void UVC_Component::setlastService(uint32_t lastService)
{
    _lastService = lastService;
}