#pragma once

#include <unordered_map>

#include "raylib.h"
#include "configuration/configuration.h"



class Fonts {
public:
    static Font getFont(const char* fileName, const int fontSize)
    {
        return get()._getFont(fileName, fontSize);
    }

private:
    std::unordered_map<const char*, Font> _fonts;

    Font _getFont(const char* fileName, const int fontSize);

public:
    Fonts(const Fonts&) = delete;
    static Fonts& get() { static Fonts instance; return instance; }
private:
    Fonts() {}
    ;
};

