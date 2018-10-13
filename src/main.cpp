#include "Arduino.h"
#include "Storage.h"

void setup() {
    Serial.begin(115200);
    Storage::init();

    Serial.print("Creating example.txt...");
    File myFile = SD.open("example.txt", FILE_WRITE);
    if (myFile) {
        myFile.println("This");
        myFile.println("Is");
        myFile.println("A (äß)");
        myFile.println("Test.");
        myFile.close();
        if (!SD.exists("example.txt")) {
            Serial.println("creation failed!");
            return;
        }
    } else {
        Serial.println("opening file failed!");
        return;
    }
    Serial.println("done.");

    Serial.println("Reading example.txt...\n---");
    myFile = SD.open("example.txt");
    if (myFile) {
        while (myFile.available()) {
            Serial.write(myFile.read());
        }
        myFile.close();
    } else {
        Serial.println("opening file failed!");
        return;
    }
    Serial.println("---\ndone.");

    Serial.print("Removing example.txt...");
    SD.remove("example.txt");
    if (SD.exists("example.txt")) {
        Serial.println("failed!");
        return;
    }
    Serial.println("done.");
}

void loop() {
}
