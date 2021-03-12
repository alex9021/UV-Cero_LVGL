# UV-Cero_LVGL

## kompilieren

### TFT_eSPI

Um die TFT_eSPI Bibliothek zu kompilieren muss, nach dem herunterladen der Abhängigkeiten (libdeps), die Standard-Config "User_Setup.h" aus dem Ordner **/.pio/libdeps/esp32/TFT_eSPI/** entfernt werden.

### Seriennummer und Konfiguration

Im Ordner **/data** befinden sich JSON-Dateien die dem SPIFFS Dateisystem beim erzeugen hinzugefügt werden. Diese Dateien sind zum Betrieb erforderlich.

**config.tpl.json** enthält die Standardkonfiguration

**serial.json** enthält die Seriennummer

*Sidebar -> PlatformIO:*
*esp32 -> Platform ->* **Build Filesystem Image**
*esp32 -> Platform ->* **Upload Filesystem Image**
