#ifndef STORAGE_H
#define STORAGE_H

#include "Arduino.h"

#define SD_CS_PIN D8

class Storage {
public:
    Storage() {}

    void init();
    void read_text(const String& filename);
    void write_text(const String& filename, const String& content);
    void remove(const String& filename);
    const bool is_ready() const;

private:
    bool ready = false;
};

#endif
