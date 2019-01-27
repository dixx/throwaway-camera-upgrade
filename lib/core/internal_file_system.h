#ifndef CORE_INTERNAL_FILE_SYSTEM_H
#define CORE_INTERNAL_FILE_SYSTEM_H

#include "FS.h"
#include "serial.h"

namespace spiffs {
    namespace {
        bool ready = false;
    }

    String readTextFile(const String& filename) {
        if (!ready) return String();
        File file = SPIFFS.open(filename, "r");
        if (!file) {
            Serial.println(String("opening file failed: ") + filename);
            return String();
        }
        String content = String();
        while (file.available()) {
            content += char(file.read());
        }
        file.close();
        return content;
    }

    void init() {
        if (ready) return;
        serial::init();
        SPIFFS.begin();
        Dir dir = SPIFFS.openDir("/");
        Serial.println("Files in SPIFFS:");
        while (dir.next()) {
            Serial.print(String("  ") + dir.fileName() + " (");
            File f = dir.openFile("r");
            Serial.println(String() + f.size() + " Byte)");
        }
        ready = true;
    }
}

#endif
