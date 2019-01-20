#include "Arduino.h"
#include "core.h"
#include "Storage.h"

Storage storage;

void setup() {
    webupdater::init();
    storage.init();
    if (storage.is_ready()) {
        storage.write_text("example.txt", "This is a test.\näöüß\n\tè^°");
        storage.read_text("example.txt");
        storage.remove("example.txt");
    }
}

void loop() {
    webserver::server.handleClient();
}
