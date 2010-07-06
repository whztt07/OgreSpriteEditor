#ifndef __GUICANVAS_H__
#define __GUICANVAS_H__

#include "Canvas.h"
#include "OgreTimer.h"
#include <OIS/OIS.h>
#include <map>
#include "FastDelegate.h"

class GUICanvas;

/**
* The 'GUIElement' class is an abstract interface that every element of
* a GUICanvas must implement.
*/
class GUIElement
{
public:

	typedef fastdelegate::FastDelegate0<void> GUIDelegate;

	virtual ~GUIElement() {}

	virtual void setContainer(GUICanvas* container) = 0;
	virtual void draw(Canvas* canvas, Atlas* atlas) = 0;
	virtual void getBounds(int& left, int& top, int& right, int& bottom) {	left = top = right = bottom = 0; }
	virtual bool onMouseDown(int x, int y) { return false; }
	virtual bool onMouseUp(int x, int y) { return false; }
	virtual bool onMouseOver() { return false; }
	virtual bool onMouseOut() { return false; }
	virtual bool onKeyboardEvent(const OIS::KeyEvent& keyEvent) { return false; }
	virtual bool isFocusable() { return false; }
	virtual void onGainFocus() {}
	virtual void onLoseFocus() {}
	virtual void update() {}

	void bindEvent(const std::string& eventName, const GUIDelegate& delegate);
	void fireEvent(const std::string& eventName);
protected:
	std::map<std::string, GUIDelegate> boundEvents;
	bool isMouseOver;
	friend class GUICanvas;
};

/**
* A GUICanvas manages a collection of GUIElements and renders them to a specific
* viewport. It uses the Canvas system for all rendering.
*/
class GUICanvas
{
public:
	GUICanvas(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr, Ogre::uint zOrder,
		const std::vector<Ogre::String>& textureFilenames, 
		const std::vector<FontFaceDefinition>& fonts);

	~GUICanvas();

	void addElement(GUIElement* element);

	void removeElement(GUIElement* element);

	void clear();

	void update();

	bool hasFocus();
	void clearFocus();

	bool injectMouseMove(int x, int y);
	bool injectMouseDown(int x, int y);
	bool injectMouseUp(int x, int y);

	bool injectKeyDown(const OIS::KeyEvent& keyEvent);
	bool injectKeyUp(const OIS::KeyEvent& keyEvent);

	void setDirty();

	Atlas* getAtlas();
	Canvas* getCanvas();
	Ogre::Viewport* getViewport();

protected:
	bool isDirty;
	Atlas* atlas;
	Canvas* canvas;
	Ogre::Viewport* viewport;
	Ogre::SceneNode* canvasNode;
	std::vector<GUIElement*> elements;
	GUIElement* focusedElement;
	OIS::KeyEvent lastKeyEvent;
	Ogre::Timer keyRepeatTimer;
	bool isKeyDown, isRepeatingKey;
};

// Enumeration of various relative positions, used by Position struct
enum RelativePosition
{
	Left,
	TopLeft,
	TopCenter,
	TopRight,
	Right,
	BottomRight,
	BottomCenter,
	BottomLeft,
	Center
};

/**
* Describes a relative and/or absolute position, used by various GUIElement types.
*/
struct Position
{
	bool usingRelative;
	union {
		struct { RelativePosition position; short x; short y; } rel;
		struct { short left; short top; } abs;
	} data;

	/**
	* Creates a default Position object (absolute, top-left corner).
	*/
	Position();

	/**
	* Creates a relatively-positioned Position object.
	*
	* @param	relPosition		The position of the Object in relation to the Viewport
	*
	* @param	offsetLeft	How many pixels from the left to offset the Object from the relative position.
	*
	* @param	offsetTop	How many pixels from the top to offset the Object from the relative position.
	*/
	Position(const RelativePosition &relPosition, short offsetLeft = 0, short offsetTop = 0);

	/**
	* Creates an absolutely-positioned Position object.
	*
	* @param	absoluteLeft	The number of pixels from the left of the Viewport.
	*
	* @param	absoluteTop		The number of pixels from the top of the Viewport.
	*/
	Position(short absoluteLeft, short absoluteTop);

	void derivePosition(int width, int height, int containerWidth, int containerHeight, int& x, int& y);
};

// Converts a hex color string (eg "#ABC123") into an Ogre ColourValue
inline Ogre::ColourValue parseHexColor(const std::string& str)
{
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned char A = 0;

	if(str.length() == 7 || str.length() == 9)
	{
		if(str.find("#") == 0 && str.substr(1).find_first_not_of("0123456789ABCDEFabcdef") == std::string::npos)
		{
			R = (unsigned char)strtoul(str.substr(1, 2).c_str(), 0, 16);
			G = (unsigned char)strtoul(str.substr(3, 2).c_str(), 0, 16);
			B = (unsigned char)strtoul(str.substr(5, 2).c_str(), 0, 16);
			A = str.length() == 7 ? 255 : (unsigned char)strtoul(str.substr(7, 2).c_str(), 0, 16);
		}
	}

	return Ogre::ColourValue(R / 255.0, G / 255.0, B / 255.0, A / 255.0);
}

// Trims the whitespace off both ends of a string
inline std::string trim(std::string source)
{
	std::string result = source.erase(source.find_last_not_of(" \t") + 1);
	return result.erase(0, result.find_first_not_of(" \t"));
}

#endif
