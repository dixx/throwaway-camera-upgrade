#ifndef CORE_WEBSERVER_H
#define CORE_WEBSERVER_H

#include "Arduino.h"
#include "ESPAsyncWebServer.h"
#include "ESP8266mDNS.h"
#include "internal_file_system.h"
#include "serial.h"
#include "wifi.h"
#include "my_secrets.h" // TODO: add login page

namespace webserver {
    namespace {
        bool ready = false;
        String active_page = "Fotos";
    }

    AsyncWebServer server(80);

    String processor(const String& var)
    {
        if (var == "PAGE_NAME")
            return active_page;
        if (
            (var == "PICS_ACTIVE" && active_page == "Fotos") ||
            (var == "PREFS_ACTIVE" && active_page == "Einstellungen") ||
            (var == "UPDATE_ACTIVE" && active_page == "Update")
        )
            return " class=\"active\"";
        if (
            (var == "PICS_MARKED" && active_page == "Fotos") ||
            (var == "PREFS_MARKED" && active_page == "Einstellungen") ||
            (var == "UPDATE_MARKED" && active_page == "Update")
        )
            return "<span class=\"sr-only\">(current)</span>";
        if (var == "CONTENT") {
            if (active_page == "Einstellungen")
                return spiffs::readTextFile("/_prefs.html");
        }
        return String();
    }

    void handleGetPics(AsyncWebServerRequest *request) {
        active_page = "Fotos";
        request->send(SPIFFS, "/index.html", String(), false, processor);
    }

    void handleUpdatePrefs() {
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
    }

    void handleGetPrefs(AsyncWebServerRequest *request) {
        // if (server.hasArg("PIN01"))
        //   handleUpdatePrefs();
        active_page = "Einstellungen";
        request->send(SPIFFS, "/index.html", String(), false, processor);
    }

    void handleGetUpdate(AsyncWebServerRequest *request) {
        active_page = "Update";
        request->send(SPIFFS, "/index.html", String(), false, processor);
    }

    void handleGetStyle(AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css", String(), false, processor);
    }

    void handleNotFound(AsyncWebServerRequest *request) {
        Serial.println("Web Request not found.");
        String message = "-- File Not Found: ";
        message += request->url();
        message += "\n   Method: ";
        message += (request->method() == HTTP_GET) ? "GET" : "POST";
        message += "\n   Arguments: ";
        message += request->args();
        message += "\n";
        for (uint8_t i = 0; i < request->args(); i++) {
            message += "   - " + request->argName(i) + ": " + request->arg(i) + "\n";
        }
        Serial.println(message);
        // request->send(404, "text/plain", message); // TODO: add 404 page
    }

    void addHandles() {
        server.on("/", HTTP_GET, handleGetPics);
        server.on("/index.html", HTTP_GET, handleGetPics);
        server.on("/prefs", HTTP_GET, handleGetPrefs);
        server.on("/update", HTTP_GET, handleGetUpdate);
        server.on("/style.css", HTTP_GET, handleGetStyle);
        server.onNotFound(handleNotFound);
    }

    void init() {
        if (ready) return;
        serial::init();
        spiffs::init();
        wifi::init();
        MDNS.addService("http", "tcp", 80);
        addHandles();
        server.begin();
        Serial.println("Web Server ready.");
        ready = true;
    }
}

#endif
