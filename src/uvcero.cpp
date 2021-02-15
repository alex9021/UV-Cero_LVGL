#include <uvcero.h>

UVC_Lamp *lamps[4];
UVC_Fan *fans[4];
UVC_Timer_Item *timerItems[MAX_TIMER_COUNT];
int currentTimerItemCount;

int fanFreq = 25000;
int fanChannel = 0;
int fanResolution = 8;

bool SD_CARD_AVAILABLE = false;

const char *configPath = "/config.json";
const char *configTemplate = "{\"components\":{\"fans\":[{\"id\":0,\"operatingLife\":5000,\"serviceLife\":50000,\"lastService\":1613234089},{\"id\":1,\"operatingLife\":5000,\"serviceLife\":50000,\"lastService\":1613234089},{\"id\":2,\"operatingLife\":5000,\"serviceLife\":50000,\"lastService\":1613234089},{\"id\":3,\"operatingLife\":5000,\"serviceLife\":50000,\"lastService\":1613234089}],\"lamps\":[{\"id\":0,\"operatingLife\":1000,\"serviceLife\":20000,\"lastService\":1613234089},{\"id\":1,\"operatingLife\":1000,\"serviceLife\":20000,\"lastService\":1613234089},{\"id\":2,\"operatingLife\":1000,\"serviceLife\":20000,\"lastService\":1613234089},{\"id\":3,\"operatingLife\":1000,\"serviceLife\":20000,\"lastService\":1613234089}]},\"config\":{\"operatingLife\":100,\"lastService\":1613234089,\"lastState\":3,\"serial\":\"UVC-000-0000-001\",\"timers\":[{\"id\":0,\"weekday\":0,\"hour\":10,\"minute\":0,\"duration\":60,\"speed\":3,\"repeat\":2},{\"id\":1,\"weekday\":1,\"hour\":12,\"minute\":0,\"duration\":120,\"speed\":2,\"repeat\":3},{\"id\":2,\"weekday\":4,\"hour\":16,\"minute\":45,\"duration\":165,\"speed\":1,\"repeat\":0}]}}";

DynamicJsonDocument doc(2048);
File configFile;
JsonObject config;

void init()
{
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    gpio_expander.write8(LOW);

    // read config from file
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    if (!SPIFFS.exists(configPath))
    {
        Serial.println("no config file found - creating one");
        //deleteFile(SPIFFS, configPath); /* reset config by deleting config file */
        writeFile(SPIFFS, configPath, configTemplate);
    }

    configFile = SPIFFS.open(configPath, "r");
    DeserializationError error = deserializeJson(doc, configFile);
    configFile.close();

    listDir(SPIFFS, "/", 0);

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    }

    // fans config
    for (JsonObject elem : doc["components"]["fans"].as<JsonArray>())
    {
        int id = elem["id"];                       // 0, 1, 2, 3
        int operatingLife = elem["operatingLife"]; // 5000, 5000, 5000, 5000
        long serviceLife = elem["serviceLife"];    // 50000, 50000, 50000, 50000
        long lastService = elem["lastService"];    // 1613234089, 1613234089, 1613234089, 1613234089

        fans[id] = new UVC_Fan(id, operatingLife, serviceLife, lastService, 0, false);
    }

    // lamps config
    for (JsonObject elem : doc["components"]["lamps"].as<JsonArray>())
    {
        int id = elem["id"];
        int operatingLife = elem["operatingLife"];
        int serviceLife = elem["serviceLife"];
        long lastService = elem["lastService"];

        lamps[id] = new UVC_Lamp(id, operatingLife, serviceLife, lastService, 0, false);
    }

    config = doc["config"];

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
}