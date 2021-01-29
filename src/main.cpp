#include <Arduino.h>
#include <uvcero.h>

void setup()
{
    Serial.begin(115200);
    
    xFAN_SPEED = xQueueCreate(1, sizeof(uint8_t));
    
    xTaskCreatePinnedToCore(guiTaskLoop, "GUI Task", 4096, NULL, 0, &thGUI, 1);
    xTaskCreatePinnedToCore(sensorsTaskLoop, "Sensors Task", 4096, NULL, 0, &thSensors, 1);
    xTaskCreatePinnedToCore(fanControllerTaskLoop, "Fan Controller Task", 4096, NULL, 0, &thFanController, 1);
    xTaskCreatePinnedToCore(lampControllerTaskLoop, "Lamp Controller Task", 4096, NULL, 0, &thLampController, 1);

    for(int i = 0; i < 4; i++){
        Serial.println(lamps[i]->getId());
    }
    
}

void loop()
{
    delay(1000);
}
