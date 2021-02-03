#include <uvc_tasks.h>

TaskHandle_t thGUI;
TaskHandle_t thSensors;
TaskHandle_t thFanController;
TaskHandle_t thLampController;
TaskHandle_t thRTCUpdater;

QueueHandle_t xFAN_SPEED;

uint8_t currentFanSpeed = FAN_SPEED_HIGH;
uint8_t currentLampState = LAMPS_OFF;

PCF8574 gpio_expander(I2C_ADDR_PCF);
uRTCLib rtc(I2C_ADDR_RTC);

TFT_eSPI tft = TFT_eSPI();
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
bool my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch(&touchX, &touchY, 600);

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;

        /*
        Serial.print("Data x");
        Serial.println(touchX);

        Serial.print("Data y");
        Serial.println(touchY);
        */
    }

    return false; /*Return `false` because we are not buffering and no more data to read*/
}

void guiTaskLoop(void *parameter)
{
    lv_init();

    tft.begin();        /* TFT init */
    tft.setRotation(1); /* Landscape orientation */

    uint16_t calData[5] = {370, 3438, 303, 3430, 7};
    tft.setTouch(calData);

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 240;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    /* create the drive */

    create_gui();

    for (;;)
    {
        lv_task_handler(); /* let the GUI do its work */
        delay(5);
    }
}

void sensorsTaskLoop(void *parameter)
{
    //gpio_expander.begin(I2C_ADDR_PCF);

    for (;;)
    {
        //gpio_expander.read8();

        fans[0]->setCurrentRPM(random(2450 * currentFanSpeed, 2500 * currentFanSpeed));
        fans[1]->setCurrentRPM(random(2450 * currentFanSpeed, 2500 * currentFanSpeed));
        fans[2]->setCurrentRPM(random(2450 * currentFanSpeed, 2500 * currentFanSpeed));
        fans[3]->setCurrentRPM(random(2450 * currentFanSpeed, 2500 * currentFanSpeed));

        lamps[0]->setCurrentLumen(random(0, 1));
        lamps[1]->setCurrentLumen(random(0, 1));
        lamps[2]->setCurrentLumen(random(0, 1));
        lamps[3]->setCurrentLumen(random(0, 1));

        delay(200);
    }
}

void fanControllerTaskLoop(void *parameter)
{
    ledcSetup(fanChannel, fanFreq, fanResolution);
    ledcAttachPin(FAN_PWM_PIN, fanChannel);
    ledcWrite(fanChannel, FAN_CALIBRATED_HIGH);

    fan_speed_t newFanSpeed;

    for (;;)
    {
        if (xQueueReceive(xFAN_SPEED, &newFanSpeed, portMAX_DELAY))
        {
            if (newFanSpeed != currentFanSpeed)
            {
                switch (newFanSpeed)
                {
                case FAN_SPEED_OFF:
                    currentFanSpeed = FAN_SPEED_OFF;
                    ledcWrite(fanChannel, FAN_CALIBRATED_OFF);
                    Serial.println("Fan speed changed to OFF.");
                    break;

                case FAN_SPEED_LOW:
                    currentFanSpeed = FAN_SPEED_LOW;
                    ledcWrite(fanChannel, FAN_CALIBRATED_LOW);
                    Serial.println("Fan speed changed to LOW.");
                    break;

                case FAN_SPEED_MEDIUM:
                    currentFanSpeed = FAN_SPEED_MEDIUM;
                    ledcWrite(fanChannel, FAN_CALIBRATED_MID);
                    Serial.println("Fan speed changed to MEDIUM.");
                    break;

                case FAN_SPEED_HIGH:
                    currentFanSpeed = FAN_SPEED_HIGH;
                    ledcWrite(fanChannel, FAN_CALIBRATED_HIGH);
                    Serial.println("Fan speed changed to HIGH.");
                    break;
                }
            }
        }
        delay(5);
    }
}

void lampControllerTaskLoop(void *parameter)
{
    for (;;)
    {
        delay(1000);
    }
}

void rtcUpdaterTaskLoop(void *parameter)
{
    rtc.set_rtc_address(I2C_ADDR_RTC);
    rtc.set_model(URTCLIB_MODEL_DS3231);

    if (rtc.enableBattery())
    {
        Serial.println("Battery activated correctly.");
    }
    else
    {
        Serial.println("ERROR activating battery.");
    }

    Serial.print("Lost power status: ");
    if (rtc.lostPower())
    {
        Serial.print("POWER FAILED. Clearing flag...");
        rtc.set(42, 37, 13, URTCLIB_WEEKDAY_TUESDAY, 21, 1, 81);
        rtc.lostPowerClear();
        Serial.println(" done.");
    }
    else
    {
        Serial.println("POWER OK");
    }

    for (;;)
    {
        rtc.refresh();
        delay(500);
    }
}
