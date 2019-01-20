#include "Storage.h"
#include "SD.h"

void Storage::init()
{
    Serial.print("Init SD-Card ... ");
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("not found!");
        return;
    }
    ready = true;
    Serial.println("ready.");
}

void Storage::read_text(const String& filename) {
    if (!ready) return;
    Serial.println(String("Reading ") + filename + "-->");
    File file = SD.open(filename);
    if (!file) {
        Serial.println("opening file failed!");
        return;
    }
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
    Serial.println("<--");
}

void Storage::write_text(const String& filename, const String& content) {
    if (!ready) return;
    Serial.print(String("Write ") + filename + " ... ");
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        Serial.println("opening file failed!");
        return;
    }
    file.print(content);
    file.close();
    if (!SD.exists(filename)) {
        Serial.println("creation failed!");
    }
    Serial.println("done.");
}

void Storage::remove(const String& filename) {
    if (!ready) return;
    Serial.print(String("Delete ") + filename + " ... ");
    SD.remove(filename);
    if (SD.exists(filename)) {
        Serial.println("failed!");
        return;
    }
    Serial.println("done.");
}

const bool Storage::is_ready() const { return ready; }
