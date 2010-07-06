#ifndef __GUILABEL_H__
#define __GUILABEL_H__

#include "GUICanvas.h"
#include "GUIBox.h"
#include <vector>

namespace GUIElements
{

class Label : public Box
{
public:
	Label(const Ogre::String& font, Ogre::uint fontSize, const Ogre::String& value, bool isCentered = false);

	void setValue(const Ogre::String& value);
	void setFontColor(const Ogre::ColourValue& color);
	void setFontShadowColor(const Ogre::ColourValue& color);

	void setContainer(GUICanvas* container);
	void draw(Canvas* canvas, Atlas* atlas);
protected:

	void calculateTextWidth();

	bool isCentered;
	Ogre::String value;
	Ogre::String font;
	Ogre::uint fontSize;
	Ogre::ColourValue fontColor, fontShadowColor;
	Ogre::Real textWidth;
};

}

#endif