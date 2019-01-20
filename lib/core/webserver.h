#ifndef CORE_WEBSERVER_H
#define CORE_WEBSERVER_H

#include "Arduino.h"
#include <ESP8266WebServer.h>
#include "internal_file_system.h"
#include "serial.h"
#include "wifi.h"

namespace webserver {
    namespace {
        bool ready = false;
    }

    ESP8266WebServer server(80);

    bool sendFileContent(String path);

    void init() {
        if (ready) return;
        serial::init();
        spiffs::init();
        wifi::init();
        server.onNotFound([]() {
            if (!sendFileContent(server.uri()))
                server.send(404, "text/plain", "404: Not Found");
        });
        ready = true;
    }

    String getContentType(const String& filename) { // convert the file extension to the MIME type
        if (filename.endsWith(".html")) return "text/html";
        else if (filename.endsWith(".css")) return "text/css";
        else if (filename.endsWith(".jpg")) return "image/jpeg";
        return "text/plain";
    }

    bool sendFileContent(String path) {
        if (path.endsWith("/")) path += "index.html"; // If a folder is requested, send the index file
        const String contentType = getContentType(path);
        if (SPIFFS.exists(path)) {
            File file = SPIFFS.open(path, "r");
            server.streamFile(file, contentType);
            file.close();
            return true;
        }
        Serial.print("\tFile Not Found: ");
        Serial.println(path);
        return false;
    }
}

#endif
