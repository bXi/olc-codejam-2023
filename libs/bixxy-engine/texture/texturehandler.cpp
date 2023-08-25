
#include "texturehandler.h"


Texture2D Textures::_getTexture(const char* fileName)
{
	if (_textures.find(fileName) == _textures.end()) {
		Texture2D _tex = LoadTexture(fileName);
		_textures[fileName] = _tex;

		return _textures[fileName];
	}
	else {
		return _textures[fileName];
	}
}

Rectangle Textures::_getRectangle(int x, int y)
{

	const Rectangle rect = { static_cast<float>(x * Configuration::tileWidth), static_cast<float>(y * Configuration::tileHeight),  static_cast<float>(Configuration::tileWidth),  static_cast<float>(Configuration::tileHeight) };
	return rect;
};

Rectangle Textures::_getTile(int tileId)
{
	return getRectangle(tileId % 16, (int)tileId / 16);
};

Rectangle Textures::_getTile(int tileId, bool doubleHeight)
{
	Rectangle r = getRectangle(tileId % 16, (int)tileId / 16);
	if (doubleHeight) {
		r.y -= r.height;
		r.height *= 2;
	}
	return r;
}

void Textures::_setTexture(const char* fileName, const Texture2D& _tex)
{
	_textures[fileName] = _tex;
}

void Textures::_setTextureFromImage(const char* fileName, const Image& _image)
{
	_textures[fileName] = LoadTextureFromImage(_image);
	UnloadImage(_image);
}

