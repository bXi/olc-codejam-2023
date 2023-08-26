#pragma once

#include <unordered_map>

#include "raylib.h"
#include "configuration/configuration.h"

class Log {
public:
    static void addLine(const char *line1, const char *line2 = "")
    {
        get()._addLine(line1, line2);
    }

        static void addLine(const char *line1, std::string line2)
    {
        get()._addLine(line1, line2.c_str());
    }

    static std::vector<std::pair<std::string, std::string>> getLines()
    {
        return get().lines;
    }

    static int getHeaderWidth()
    {
        return get().headerWidth;
    }

    static int getLongestLineWidth()
    {
        return get().longestLineWidth;
    }

    static void resetLog()
    {
        get().headerWidth = 0;
        get().longestLineWidth = 0;
        get().lines.clear();
    }
private:
    std::vector<std::pair<std::string, std::string>> lines;



    void _addLine(const char *line1, const char * line2);

    int headerWidth = 0;
    int longestLineWidth = 0;

public:
    Log(const Log&) = delete;
    static Log& get() { static Log instance; return instance; }
private:
    Log() {}
    ;
};

