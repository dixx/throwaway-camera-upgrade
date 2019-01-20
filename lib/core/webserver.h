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
        if (filename.endsWith(".htm")) return "text/html";
        else if (filename.endsWith(".html")) return "text/html";
        else if (filename.endsWith(".css")) return "text/css";
        else if (filename.endsWith(".js")) return "application/javascript";
        else if (filename.endsWith(".png")) return "image/png";
        else if (filename.endsWith(".gif")) return "image/gif";
        else if (filename.endsWith(".jpg")) return "image/jpeg";
        else if (filename.endsWith(".ico")) return "image/x-icon";
        else if (filename.endsWith(".xml")) return "text/xml";
        else if (filename.endsWith(".pdf")) return "application/x-pdf";
        else if (filename.endsWith(".zip")) return "application/x-zip";
        else if (filename.endsWith(".gz")) return "application/x-gzip";
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
