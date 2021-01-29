#include <uvcero.h>

UVC_Lamp *lamps[4];
UVC_Fan *fans[4];

int fanFreq = 20000;
int fanChannel = 0;
int fanResolution = 8;

void init()
{

    /**
      @ToDo: read data from sd card
    **/

    // init lamps
    for (int i = 0; i < 4; i++)
    {
        lamps[i] = new UVC_Lamp(i + 1, random(0, 10000), random(0, 10000), random(0, 10000), random(0, 1000));
    }

    // init fans
    for (int i = 0; i < 4; i++)
    {
        fans[i] = new UVC_Fan(i + 1, random(0, 50000), random(0, 50000), random(0, 50000), random(0, 10000));
    }
}