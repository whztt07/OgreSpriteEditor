#include "GUILabel.h"

using namespace GUIElements;

Label::Label(const Ogre::String& font, Ogre::uint fontSize, const Ogre::String& value, bool isCentered) 
: Box(100, 30), font(font), fontSize(fontSize), value(value), textWidth(50), isCentered(isCentered), fontShadowColor(0, 0, 0)
{
}

void Label::setValue(const Ogre::String& value)
{
	if(value != this->value)
	{
		flagDirty();
		this->value = value;
		if(isCentered)
			calculateTextWidth();
	}
}

void Label::setFontColor(const Ogre::ColourValue& color)
{
	flagDirty();
	fontColor = color;
}

void Label::setFontShadowColor(const Ogre::ColourValue& color)
{
	flagDirty();
	fontShadowColor = color;
}

void Label::setContainer(GUICanvas* container)
{
	flagDirty();
	this->container = container;
	if(isCentered)
		calculateTextWidth();
}

void Label::draw(Canvas* canvas, Atlas* atlas)
{
	Box::draw(canvas, atlas);

	int x, y;
	position.derivePosition(width, height, container->getViewport()->getActualWidth(),
		container->getViewport()->getActualHeight(), x, y);

	FontMetrics metrics = atlas->getFontMetrics(font, fontSize);
	GlyphInfo iInfo = atlas->getGlyphInfo(font, fontSize, 'i');

	y += (height / 2) + (metrics.height * 0.4);

	Ogre::Real pen;
	if(isCentered)
		pen = x = x + width / 2 - textWidth / 2;
	else
		pen = x = x + iInfo.advance * 0.6 + 6;

	for(unsigned int i = 0; i < value.length(); ++i)
	{
		if(value[i] == ' ')
		{
			pen += iInfo.advance;
			continue;
		}
		else if(value[i] == '\n')
		{
			pen = x;
			y += metrics.height;
			continue;
		}

		GlyphInfo glyph = atlas->getGlyphInfo(font, fontSize, value[i]);
		canvas->drawGlyph(glyph, glyph.bearingX + pen + 1, y - glyph.bearingY + 1, glyph.texInfo.width, glyph.texInfo.height, fontShadowColor);
		canvas->drawGlyph(glyph, glyph.bearingX + pen, y - glyph.bearingY, glyph.texInfo.width, glyph.texInfo.height, fontColor);
		pen += glyph.advance;
	}
}

void Label::calculateTextWidth()
{
	assert(container);

	textWidth = 0;
	
	Atlas* atlas = container->getAtlas();

	for(unsigned int i = 0; i < value.length(); ++i)
	{
		if(value[i] == ' ')
			textWidth += atlas->getGlyphInfo(font, fontSize, 'i').advance;
		else
			textWidth += atlas->getGlyphInfo(font, fontSize, value[i]).advance;
	}
}
