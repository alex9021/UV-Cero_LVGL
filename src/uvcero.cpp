#include <uvcero.h>

UVC_Lamp *lamps[4];
UVC_Fan *fans[4];
UVC_Timer_Item* timerItems[MAX_TIMER_COUNT];
int currentTimerItemCount;

int fanFreq = 20000;
int fanChannel = 0;
int fanResolution = 8;

bool SD_CARD_AVAILABLE = false;

void init()
{
    /**
      @ToDo: read data from sd card
    **/

    // init lamps
    for (int i = 0; i < 4; i++)
    {
        lamps[i] = new UVC_Lamp(i + 1, random(0, 10000), random(0, 10000), random(0, 10000), random(0, 1));
    }

    // init fans
    for (int i = 0; i < 4; i++)
    {
        fans[i] = new UVC_Fan(i + 1, random(0, 50000), random(0, 50000), random(0, 50000), random(0, 10000));
    }

    currentTimerItemCount = 0;

    timerItems[currentTimerItemCount] = new UVC_Timer_Item(currentTimerItemCount, TIMER_ITEM_WEEKDAY_MONDAY, 10, 0, 60, TIMER_ITEM_REPEAT_DAILY);
    currentTimerItemCount++;
    timerItems[currentTimerItemCount] = new UVC_Timer_Item(currentTimerItemCount, TIMER_ITEM_WEEKDAY_TUESDAY, 12, 0, 120, TIMER_ITEM_REPEAT_WEEKLY);
    currentTimerItemCount++;
    timerItems[currentTimerItemCount] = new UVC_Timer_Item(currentTimerItemCount, TIMER_ITEM_WEEKDAY_FRIDAY, 16, 45, 165, TIMER_ITEM_REPEAT_NONE);
    currentTimerItemCount++;

    Serial.println(currentTimerItemCount);
}