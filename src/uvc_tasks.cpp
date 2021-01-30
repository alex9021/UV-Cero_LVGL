#include <uvc_tasks.h>

TaskHandle_t thGUI;
TaskHandle_t thSensors;
TaskHandle_t thFanController;
TaskHandle_t thLampController;

QueueHandle_t xFAN_SPEED;

uint8_t currentFanSpeed = FAN_SPEED_HIGH;
uint8_t currentLampState = LAMPS_OFF;

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

#if USE_LV_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char *file, uint32_t line, const char *dsc)
{

    Serial.printf("%s@%d->%s\r\n", file, line, dsc);
    Serial.flush();
}
#endif

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

/* SD CARD */
/*lv_fs_res_t sd_open_cb(struct _lv_fs_drv_t *drv, void *file_p, const char *path, lv_fs_mode_t mode)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    File f;
    char buf[256];

    sprintf(buf, "/%s", path);
    Serial.print("path : ");
    Serial.println(buf);

    if (mode == LV_FS_MODE_WR)
    {
        f = SD.open(buf, FILE_WRITE);
        res = LV_FS_RES_OK;
    }
    else if (mode == LV_FS_MODE_RD)
    {
        f = SD.open(buf);
        res = LV_FS_RES_OK;
    }
    else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
    {
        f = SD.open(buf, FILE_WRITE);
        res = LV_FS_RES_OK;
    }
    File *fp = (File *)file_p;
    *fp = f; // where the probleme come

    return res;
}

lv_fs_res_t sd_read_cb(struct _lv_fs_drv_t *drv, void *file_p, void *buf, uint32_t btr, uint32_t *br)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    res = LV_FS_RES_OK;
    return res;
}

lv_fs_res_t sd_close_cb(struct _lv_fs_drv_t *drv, void *file_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    return res;
}*/

void guiTaskLoop(void *parameter)
{
    lv_init();

#if USE_LV_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

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
    /*if (SD_CARD_AVAILABLE)
    {
        lv_fs_drv_t sd_drv;
        lv_fs_drv_init(&sd_drv);
        sd_drv.file_size = sizeof(File);
        sd_drv.letter = 'S';
        sd_drv.open_cb = sd_open_cb;
        sd_drv.close_cb = sd_close_cb;
        sd_drv.read_cb = sd_read_cb;
        lv_fs_drv_register(&sd_drv);
    }*/

    create_gui();

    for (;;)
    {
        lv_task_handler(); /* let the GUI do its work */
        delay(5);
    }
}

void sensorsTaskLoop(void *parameter)
{
    for (;;)
    {
        for (int i = 0; i < 4; i++)
        {
            lamps[i]->setCurrentLumen(random(0, 1000));
        }

        for (int i = 0; i < 4; i++)
        {
            fans[i]->setCurrentRPM(random(0, 10000));
        }
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
    }
}

void lampControllerTaskLoop(void *parameter)
{
    for (;;)
    {
        delay(1000);
    }
}