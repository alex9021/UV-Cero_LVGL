#include <Arduino.h>
#include <uvcero.h>

void setup()
{
    digitalWrite(TFT_BL, LOW);
    digitalWrite(TFT_CS, HIGH);
    digitalWrite(TOUCH_CS, HIGH);
    //digitalWrite(SD_CS, HIGH);
    pinMode(LAMP_SWITCH_PIN, OUTPUT);
    pinMode(FAN_PWM_PIN, OUTPUT);

    Serial.begin(115200);
    Serial.println("");

    xFAN_SPEED = xQueueCreate(1, sizeof(uint8_t));

    init();

    xTaskCreatePinnedToCore(guiTaskLoop, "GUI Task", 4096, NULL, 0, &thGUI, 1);
    xTaskCreatePinnedToCore(sensorsTaskLoop, "Sensors Task", 4096, NULL, 0, &thSensors, 1);
    xTaskCreatePinnedToCore(fanControllerTaskLoop, "Fan Controller Task", 4096, NULL, 0, &thFanController, 1);
    xTaskCreatePinnedToCore(rtcUpdaterTaskLoop, "RTC Updater Task", 4096, NULL, 0, &thRTCUpdater, 1);
}

void loop()
{
    delay(1000);
}
