#ifndef __GUIBOX_H__
#define __GUIBOX_H__

#include "GUICanvas.h"
#include <vector>

namespace GUIElements
{

class Box : public GUIElement
{
public:
	Box(int width, int height);

	virtual void setSize(int width, int height);
	virtual void setPosition(const Position& position);
	virtual void setBackground(const Fill& fill);
	virtual void setBorder(const Border& border);

	virtual void setContainer(GUICanvas* container);
	virtual void draw(Canvas* canvas, Atlas* atlas);
	virtual void getBounds(int& left, int& top, int& right, int& bottom);
	virtual bool onMouseDown(int x, int y) { return false; }
	virtual bool onMouseUp(int x, int y) { return false; }
	virtual bool onMouseOver() { return false; }
	virtual bool onMouseOut() { return false; }
	virtual bool onKeyboardEvent(const OIS::KeyEvent& keyEvent) { return false; }
	virtual bool isFocusable() { return false; }
	virtual void onGainFocus() {}
	virtual void onLoseFocus() {}
	virtual void update() {}
protected:

	void flagDirty();

	GUICanvas* container;
	int width, height;
	Position position;
	Fill background;
	Border border;
};

}

#endif