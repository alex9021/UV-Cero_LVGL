#include <Classes/UVC_Timer_Item.h>
UVC_Timer_Item::UVC_Timer_Item(uint8_t id, uint8_t weekDay, uint8_t hour, uint8_t minute, uint32_t duration, uint8_t repeat)
{
    _id = id;
    _weekDay = weekDay;
    _hour = hour;
    _minute = minute;
    _duration = duration;
    _repeat = repeat;
}

String UVC_Timer_Item::getWeekdayAsString()
{
    String weekDay;

    if (_repeat == 1)
    {
        weekDay = "Hourly @";
    }
    else if (_repeat == 2)
    {
        weekDay = "Daily @";
    }
    else
    {
        switch (_weekDay)
        {
        case 0:
            if (_repeat == 3)
            {
                weekDay = "Mondays @";
            }
            else
            {
                weekDay = "Monday @";
            }
            break;
        case 1:
            if (_repeat == 3)
            {
                weekDay = "Tuesdays @";
            }
            else
            {
                weekDay = "Tuesday @";
            }
            break;
        case 2:
            if (_repeat == 3)
            {
                weekDay = "Wednesdays @";
            }
            else
            {
                weekDay = "Wednesday @";
            }
            break;
        case 3:
            if (_repeat == 3)
            {
                weekDay = "Thursdays @";
            }
            else
            {
                weekDay = "Thursday @";
            }
            break;
        case 4:
            if (_repeat == 3)
            {
                weekDay = "Fridays @";
            }
            else
            {
                weekDay = "Friday @";
            }
            break;
        case 5:
            if (_repeat == 3)
            {
                weekDay = "Saturdays @";
            }
            else
            {
                weekDay = "Saturday @";
            }
            break;
        case 6:
            if (_repeat == 3)
            {
                weekDay = "Sundays @";
            }
            else
            {
                weekDay = "Sunday @";
            }
            break;

        default:
            break;
        }
    }
    return weekDay;
}

String UVC_Timer_Item::getStartTimeAsString()
{
    char startTimeText[10];
    sprintf(startTimeText, "%02d:%02d", _hour, _minute);

    return String(startTimeText);
}

int UVC_Timer_Item::getWeekday()
{
    return _weekDay;
}

int UVC_Timer_Item::getDuration()
{
    return _duration;
}

int UVC_Timer_Item::getRepeat()
{
    return _repeat;
}

int UVC_Timer_Item::getHour()
{
    return _hour;
}

int UVC_Timer_Item::getMinute()
{
    return _minute;
}

String UVC_Timer_Item::getRepeatAsString()
{
    return "";
}