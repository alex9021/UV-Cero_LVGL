#include <uvcero.h>

UVC_Lamp *lamps[4];
UVC_Fan *fans[4];
UVC_Timer_Item* timerItems[MAX_TIMER_COUNT];
int currentTimerItemCount;

int fanFreq = 25000;
int fanChannel = 0;
int fanResolution = 8;

bool SD_CARD_AVAILABLE = false;

void init()
{
    Wire.begin(I2C_SDA_PIN,I2C_SCL_PIN);

    gpio_expander.write8(LOW);
    gpio_expander.write(4, HIGH);
    gpio_expander.write(5, HIGH);
    gpio_expander.write(6, HIGH);
    gpio_expander.write(7, HIGH);
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

    timerItems[currentTimerItemCount] = new UVC_Timer_Item(currentTimerItemCount, TIMER_ITEM_WEEKDAY_MONDAY, 10, 0, 60, TIMER_ITEM_REPEAT_DAILY, FAN_SPEED_HIGH);
    currentTimerItemCount++;
    timerItems[currentTimerItemCount] = new UVC_Timer_Item(currentTimerItemCount, TIMER_ITEM_WEEKDAY_TUESDAY, 12, 0, 120, TIMER_ITEM_REPEAT_WEEKLY, FAN_SPEED_MEDIUM);
    currentTimerItemCount++;
    timerItems[currentTimerItemCount] = new UVC_Timer_Item(currentTimerItemCount, TIMER_ITEM_WEEKDAY_FRIDAY, 16, 45, 165, TIMER_ITEM_REPEAT_NONE, FAN_SPEED_LOW);
    currentTimerItemCount++;
}