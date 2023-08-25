
#include "fonthandler.h"


Font Fonts::_getFont(const char* fileName, const int fontSize)
{
    const char* index = TextFormat("%s%d", fileName, fontSize);

	if (_fonts.find(index) == _fonts.end()) {

		Font _font = LoadFontEx(fileName, fontSize, 0, 250);
		_fonts[index] = _font;

		return _fonts[index];
	}
	else {
		return _fonts[index];
	}
}


