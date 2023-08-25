#pragma once

#include <unordered_map>

#include "raylib.h"
#include "configuration/configuration.h"



class Textures {
public:
    static Texture2D getTexture(const char* fileName)
    {
        return get()._getTexture(fileName);
    }

    static Rectangle getRectangle(int x, int y)
    {
        return get()._getRectangle(x, y);
    }

    static Rectangle getTile(int tileId)
    {
        return get()._getTile(tileId);
    }
    static Rectangle getTile(int tileId, bool doubleHeight)
    {
        return get()._getTile(tileId, doubleHeight);
    }

    static void setTexture(const char* fileName, const Texture2D& _tex)
    {
        get()._setTexture(fileName, _tex);
    }
    static void setTextureFromImage(const char* fileName, const Image& _image)
    {
        get()._setTextureFromImage(fileName, _image);
    }

private:
    std::unordered_map<const char*, Texture2D> _textures;

    Texture2D _getTexture(const char* fileName);

    Rectangle _getRectangle(int x, int y);

    Rectangle _getTile(int tileId);
    Rectangle _getTile(int tileId, bool doubleHeight);

    void _setTexture(const char* fileName, const Texture2D& _tex);
    void _setTextureFromImage(const char* fileName, const Image& _image);

public:
    Textures(const Textures&) = delete;
    static Textures& get() { static Textures instance; return instance; }
private:
    Textures() {}
    ;
};

