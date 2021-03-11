#include <uvcero.h>

#define ENABLE_I2C_SCANNER 1

UVC_Lamp *lamps[4];
UVC_Fan *fans[4];
UVC_Timer_Item *timerItems[MAX_TIMER_COUNT];
int currentTimerItemCount;

int fanFreq = 25000;
int fanChannel = 0;
int fanResolution = 8;

bool SD_CARD_AVAILABLE = false;

const char *configPath = "/config.json";
const char *configTemplatePath = "/config.tpl.json";
const char *serialPath = "/serial.json";

const char *serialNo;

DynamicJsonDocument configDoc(2048);
DynamicJsonDocument serialDoc(128);
File configFile;
File serialFile;
JsonObject config;

void init()
{

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (ENABLE_I2C_SCANNER)
    {
        Serial.printf("========= I²C SCANNER ==========\n");
        byte error, address;
        int nDevices;
        nDevices = 0;
        for (address = 1; address < 127; address++)
        {
            Wire.beginTransmission(address);
            error = Wire.endTransmission();

            switch (error)
            {
            case 0:
                Serial.printf("Device found at address 0x%2X\n", address);
                break;

            case 4:
                Serial.printf("Unknown error at address 0x%2X\n", address);
                break;

            default:
                break;
            }
            nDevices++;
        }

        if (nDevices == 0)
        {
            Serial.println("No I2C devices found\n");
        }
        Serial.printf("================================\n\n");
    }

    gpio_expander.write8(LOW);

    // mount internal fs
    if (!SPIFFS.begin())
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    //deleteFile(SPIFFS, configPath);

    listDir(SPIFFS, "/", 2);

    // check for serial
    if (!SPIFFS.exists(serialPath))
    {
        Serial.println("no serial found!");
        return;
    }

    // read serial
    serialFile = SPIFFS.open(serialPath, FILE_READ);
    DeserializationError err = deserializeJson(serialDoc, serialFile);

    serialFile.close();

    if (err)
    {
        Serial.print("read serial failed: ");
        Serial.println(err.f_str());
        return;
    }

    serialNo = serialDoc["serial"];
    Serial.println(serialNo);

    // check for config
    if (!SPIFFS.exists(configPath))
    {
        Serial.println("no config file found - creating one");

        // check for config template
        if (!SPIFFS.exists(configTemplatePath))
        {
            Serial.println("no config template found!");
            return;
        }

        configFile = SPIFFS.open(configTemplatePath, FILE_READ);
        DeserializationError error = deserializeJson(configDoc, configFile);
        configFile.close();

        if (error)
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.f_str());
            return;
        }

        char outputData[2048];
        serializeJson(configDoc, outputData);

        configFile = SPIFFS.open(configPath, FILE_WRITE);
        configFile.print(outputData);
        configFile.close();
    }

    configFile = SPIFFS.open(configPath, "r");
    DeserializationError error = deserializeJson(configDoc, configFile);
    configFile.close();

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
    }

    config = configDoc["config"];

    // fans config
    for (JsonObject elem : configDoc["components"]["fans"].as<JsonArray>())
    {
        int id = elem["id"];                       // 0, 1, 2, 3
        int operatingLife = elem["operatingLife"]; // 5000, 5000, 5000, 5000
        long serviceLife = elem["serviceLife"];    // 50000, 50000, 50000, 50000
        long lastService = elem["lastService"];    // 1613234089, 1613234089, 1613234089, 1613234089

        fans[id] = new UVC_Fan(id, operatingLife, serviceLife, lastService, 0, false);
    }

    // lamps config
    for (JsonObject elem : configDoc["components"]["lamps"].as<JsonArray>())
    {
        int id = elem["id"];
        int operatingLife = elem["operatingLife"];
        int serviceLife = elem["serviceLife"];
        long lastService = elem["lastService"];

        lamps[id] = new UVC_Lamp(id, operatingLife, serviceLife, lastService, 0, false);
    }

    config = configDoc["config"];

    int config_lastState = config["lastState"];
    switch (config_lastState)
    {
    case FAN_SPEED_LOW:
        currentFanSpeed = FAN_SPEED_LOW;
        currentLampState = LAMPS_ON;
        break;

    case FAN_SPEED_MEDIUM:
        currentFanSpeed = FAN_SPEED_MEDIUM;
        currentLampState = LAMPS_ON;
        break;

    case FAN_SPEED_HIGH:
        currentFanSpeed = FAN_SPEED_HIGH;
        currentLampState = LAMPS_ON;
        break;

    default:
        currentFanSpeed = FAN_SPEED_OFF;
        currentLampState = LAMPS_OFF;
        break;
    }

    // timer items
    currentTimerItemCount = 0;
    for (JsonObject elem : config["timers"].as<JsonArray>())
    {
        int id = elem["id"];
        int weekday = elem["weekday"];
        int hour = elem["hour"];
        int minute = elem["minute"];
        int duration = elem["duration"];
        int speed = elem["speed"];
        int repeat = elem["repeat"];

        timerItems[currentTimerItemCount] = new UVC_Timer_Item(id, weekday, hour, minute, duration, repeat, speed);
        currentTimerItemCount++;
    }

    void saveConfig()
    {
    }
}