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

    void handlePics() {
        sendFileContent("/");
    }

    void handleSubmit() {
      // String LEDvalue;
      //
      // LEDvalue = server.arg("PINS");
      // if (LEDvalue == "1") {
      //   writeLED(true);
      //   server.send(200, "text/html", INDEX_HTML);
      // }
      // else if (LEDvalue == "0") {
      //   writeLED(false);
      //   server.send(200, "text/html", INDEX_HTML);
      // }
      // else {
      //   returnFail("Bad LED value");
      // }
      sendFileContent("/");
    }

    void handlePrefs() {
        if (server.hasArg("PIN01"))
          handleSubmit();
        sendFileContent("/prefs.html");
    }

    void handleStats() {
        sendFileContent("/stats.html");
    }

    void handleNotFound() {
        if (!sendFileContent(server.uri())) {
            String message = "File Not Found\n\nURI: ";
            message += server.uri();
            message += "\nMethod: ";
            message += (server.method() == HTTP_GET) ? "GET" : "POST";
            message += "\nArguments: ";
            message += server.args();
            message += "\n";
            for (uint8_t i = 0; i < server.args(); i++) {
              message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
            }
            server.send(404, "text/plain", message);
        }
    }

    void init() {
        if (ready) return;
        serial::init();
        spiffs::init();
        wifi::init();
        server.on("/", handlePics);
        server.on("/prefs", handlePrefs);
        server.on("/stats", handleStats);
        server.onNotFound(handleNotFound);
        ready = true;
    }
}

#endif
