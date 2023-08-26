#include "levelmanager.h"


LevelManager::LevelManager()
{

}

void LevelManager::loadLevel(const char *levelnumber) {
    SetTraceLogLevel(LOG_INFO);

    using json = nlohmann::json;

    auto filename = TextFormat("assets/levels/level%s.tmj", levelnumber);
    TraceLog(LOG_INFO, TextFormat("Loading file: %s",filename));
    std::ifstream f(filename);

    json data = json::parse(f);

    LevelData newData;

    newData.filename = filename;

    newData.width = data["width"];
    newData.height = data["height"];

    TraceLog(LOG_INFO, TextFormat("Set width to %d and height to %d for level%s", newData.width, newData.height, levelnumber));

    for (auto& layer : data["layers"]) {
        auto jsonname = layer["name"];

        auto name = jsonname.template get<std::string>();

        if (name == "floor") {
            newData.floorTiles = layer["data"].get<std::vector<int>>();
            TraceLog(LOG_INFO, TextFormat("Loading floor data for level%s",levelnumber));
        } else if (name == "decor") {
            newData.decorTiles = layer["data"].get<std::vector<int>>();
            TraceLog(LOG_INFO, TextFormat("Loading decor data for level%s",levelnumber));
        }


    }
    SetTraceLogLevel(LOG_WARNING);

    currentlevel = newData;

}




