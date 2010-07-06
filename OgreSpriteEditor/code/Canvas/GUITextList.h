#ifndef __GUITEXTLIST_H__
#define __GUITEXTLIST_H__

#include "GUICanvas.h"
#include <deque>

namespace GUIElements
{

class TextList : public GUIElement
{
public:
	TextList(const Ogre::String& font, Ogre::uint fontSize, const Ogre::uint numLines, const Ogre::uint width,
		bool isFlipped, bool doesFade);

	void addText(const Ogre::String& text);
	void clearText();

	void setPosition(const Position& position);
	void setFontColor(const Ogre::ColourValue& color);
	void setFontShadowColor(const Ogre::ColourValue& color);
	void setFontShadowDistance(int pixels);

	void setContainer(GUICanvas* container);
	void draw(Canvas* canvas, Atlas* atlas);
protected:

	void addTextLine(const Ogre::String& text);
	void flagDirty();

	bool isFlipped, doesFade;
	GUICanvas* container;
	Ogre::String value;
	Ogre::String font;
	Ogre::uint fontSize, numLines;
	Ogre::uint width, height;
	Position position;
	Ogre::ColourValue fontColor, fontShadowColor;
	int fontShadowDist;
	std::deque<Ogre::String> textLines;
};

}

#endif