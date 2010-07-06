#include "GUITextList.h"

using namespace GUIElements;

TextList::TextList(const Ogre::String& font, Ogre::uint fontSize, const Ogre::uint numLines, 
	const Ogre::uint width, bool isFlipped, bool doesFade) : font(font), fontSize(fontSize), 
	numLines(numLines), width(width), isFlipped(isFlipped), doesFade(doesFade), container(0), 
	height(100), fontShadowColor(0, 0, 0), fontShadowDist(1)
{
	assert(numLines > 0);
}

void TextList::addText(const Ogre::String& text)
{
	Ogre::String tempText;

	for(Ogre::String::const_iterator i = text.begin(); i != text.end(); ++i)
	{
		if(*i == '\n')
		{
			if(tempText.length())
			{
				addTextLine(tempText);
				tempText.clear();
			}
		}
		else
		{
			tempText.push_back(*i);
		}
	}

	if(tempText.length())
		addTextLine(tempText);
}

void TextList::clearText()
{
	flagDirty();
	textLines.clear();
}

void TextList::setPosition(const Position& position)
{
	flagDirty();
	this->position = position;
}

void TextList::setFontColor(const Ogre::ColourValue& color)
{
	flagDirty();
	fontColor = color;
}

void TextList::setFontShadowColor(const Ogre::ColourValue& color)
{
	flagDirty();
	fontShadowColor = color;
}

void TextList::setFontShadowDistance(int pixels)
{
	flagDirty();
	fontShadowDist = pixels;
}

void TextList::setContainer(GUICanvas* container)
{
	this->container = container;

	FontMetrics metrics = container->getAtlas()->getFontMetrics(font, fontSize);

	height = metrics.height * numLines;
}

void TextList::draw(Canvas* canvas, Atlas* atlas)
{
	int x, y;
	position.derivePosition(width, height, container->getViewport()->getActualWidth(),
		container->getViewport()->getActualHeight(), x, y);

	FontMetrics metrics = atlas->getFontMetrics(font, fontSize);
	GlyphInfo iInfo = atlas->getGlyphInfo(font, fontSize, 'i');

	Ogre::Real penX = x;
	Ogre::Real penY = isFlipped? y + height - metrics.height : y;

	Ogre::ColourValue lineFontColor = fontColor;
	Ogre::ColourValue lineFontShadowColor = fontShadowColor;
	Ogre::uint lineCount = 0;

	for(int i = isFlipped? (int)textLines.size() - 1 : 0; isFlipped? i >= 0 : i < (int)textLines.size(); 
		isFlipped? --i : ++i)
	{
		if(doesFade)
		{
			Ogre::Real opacity = (numLines - ++lineCount) / (Ogre::Real)numLines;
			lineFontColor.a = fontColor.a * opacity + 0.12;
			if(lineFontColor.a > 1)
				lineFontColor.a = 1;
			lineFontShadowColor.a = fontShadowColor.a * opacity + 0.12;
			if(lineFontShadowColor.a > 1)
				lineFontShadowColor.a = 1;
		}

		for(unsigned int j = 0; j < textLines[i].length(); ++j)
		{
			if(textLines[i][j] == ' ')
			{
				penX += iInfo.advance;
				continue;
			}

			GlyphInfo glyph = atlas->getGlyphInfo(font, fontSize, textLines[i][j]);
			canvas->drawGlyph(glyph, glyph.bearingX + penX + fontShadowDist, 
				penY - glyph.bearingY + fontShadowDist, glyph.texInfo.width, 
				glyph.texInfo.height, lineFontShadowColor);
			canvas->drawGlyph(glyph, glyph.bearingX + penX, penY - glyph.bearingY, glyph.texInfo.width, 
				glyph.texInfo.height, lineFontColor);

			penX += glyph.advance;
		}

		penX = x;
		penY += isFlipped? -metrics.height : metrics.height;
	}
}

void TextList::flagDirty()
{
	if(container)
		container->setDirty();
}

void TextList::addTextLine(const Ogre::String& text)
{
	flagDirty();
	if(isFlipped)
	{
		if(textLines.size() == numLines)
			textLines.pop_front();

		textLines.push_back(text);
	}
	else
	{
		if(textLines.size() == numLines)
			textLines.pop_back();

		textLines.push_front(text);
	}
}