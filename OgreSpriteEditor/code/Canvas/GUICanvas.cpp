#include "GUICanvas.h"

void GUIElement::bindEvent(const std::string& eventName, const GUIDelegate& delegate)
{
	boundEvents[eventName] = delegate;
}

void GUIElement::fireEvent(const std::string& eventName)
{
	std::map<std::string, GUIDelegate>::iterator i = boundEvents.find(eventName);

	if(i != boundEvents.end())
		i->second();
}

GUICanvas::GUICanvas(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr, Ogre::uint zOrder,
	const std::vector<Ogre::String>& textureFilenames, 
	const std::vector<FontFaceDefinition>& fonts) : focusedElement(0), isRepeatingKey(false), 
	isKeyDown(false), lastKeyEvent(0, OIS::KC_0, 0)
{
	atlas = new Atlas(textureFilenames, fonts, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	canvas = new Canvas(atlas, camera->getViewport(), zOrder);
	canvasNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	canvasNode->attachObject(canvas);
	viewport = camera->getViewport();
}

GUICanvas::~GUICanvas()
{
	for(std::vector<GUIElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
		delete *i;

	canvasNode->detachObject(canvas);
	delete canvas;
	delete atlas;
}

void GUICanvas::addElement(GUIElement* element)
{
	element->setContainer(this);
	element->isMouseOver = false;
	elements.push_back(element);
	setDirty();
}

void GUICanvas::removeElement(GUIElement* element)
{
	for(std::vector<GUIElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
	{
		if(*i == element)
		{
			setDirty();
			elements.erase(i);
			delete element;

			if(element == focusedElement)
				focusedElement = 0;
		}
	}
}

void GUICanvas::clear()
{
	for(std::vector<GUIElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
		delete *i;

	elements.clear();
	focusedElement = 0;
	canvas->clear();
}

void GUICanvas::update()
{
	if(isKeyDown && focusedElement)
	{
		if(isRepeatingKey && keyRepeatTimer.getMilliseconds() > 60)
		{
			focusedElement->onKeyboardEvent(lastKeyEvent);
			keyRepeatTimer.reset();
		}
		else if(keyRepeatTimer.getMilliseconds() > 600)
		{
			isRepeatingKey = true;
			focusedElement->onKeyboardEvent(lastKeyEvent);
			keyRepeatTimer.reset();
		}
	}

	if(isDirty)
	{
		canvas->clear();

		for(std::vector<GUIElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
		{
			(*i)->update();
			(*i)->draw(canvas, atlas);
		}

		isDirty = false;
	}
	else
	{
		for(std::vector<GUIElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
			(*i)->update();
	}
}

bool GUICanvas::hasFocus()
{
	return !!focusedElement;
}

void GUICanvas::clearFocus()
{
	if(focusedElement)
	{
		focusedElement->onLoseFocus();
		focusedElement = 0;
	}
}

bool GUICanvas::injectMouseMove(int x, int y)
{
	bool isOverElement = false;

	int bLeft, bTop, bRight, bBottom;
	for(std::vector<GUIElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
	{
		(*i)->getBounds(bLeft, bTop, bRight, bBottom);
		if(x >= bLeft && y >= bTop && x <= bRight && y <= bBottom)
		{
			isOverElement = true;
			if(!(*i)->isMouseOver)
			{
				(*i)->isMouseOver = true;
				(*i)->onMouseOver();
			}
		}
		else if((*i)->isMouseOver)
		{
			(*i)->isMouseOver = false;
			(*i)->onMouseOut();
		}	
	}

	return isOverElement;
}

bool GUICanvas::injectMouseDown(int x, int y)
{
	int bLeft, bTop, bRight, bBottom;
	for(std::vector<GUIElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
	{
		(*i)->getBounds(bLeft, bTop, bRight, bBottom);
		if(x >= bLeft && y >= bTop && x <= bRight && y <= bBottom)
		{
			if((*i)->onMouseDown(x, y))
			{
				if(*i != focusedElement)
				{
					if(focusedElement)
						focusedElement->onLoseFocus();

					focusedElement = *i;
					focusedElement->onGainFocus();
				}

				return true;
			}
		}
	}

	clearFocus();

	return false;
}

bool GUICanvas::injectMouseUp(int x, int y)
{
	int bLeft, bTop, bRight, bBottom;
	for(std::vector<GUIElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
	{
		(*i)->getBounds(bLeft, bTop, bRight, bBottom);
		if(x >= bLeft && y >= bTop && x <= bRight && y <= bBottom)
			if((*i)->onMouseUp(x, y))
				return true;
	}

	return false;
}

bool GUICanvas::injectKeyDown(const OIS::KeyEvent& keyEvent)
{
	isKeyDown = true;
	isRepeatingKey = false;
	keyRepeatTimer.reset();
	const_cast<OIS::KeyCode&>(lastKeyEvent.key) = keyEvent.key;
	lastKeyEvent.text = keyEvent.text;

	if(focusedElement)
		return focusedElement->onKeyboardEvent(keyEvent);

	return false;
}

bool GUICanvas::injectKeyUp(const OIS::KeyEvent& keyEvent)
{
	isRepeatingKey = false;
	isKeyDown = false;
	keyRepeatTimer.reset();
	
	return false;
}

void GUICanvas::setDirty()
{
	isDirty = true;
}

Atlas* GUICanvas::getAtlas()
{
	return atlas;
}

Canvas* GUICanvas::getCanvas()
{
	return canvas;
}

Ogre::Viewport* GUICanvas::getViewport()
{
	return viewport;
}

Position::Position()
{
	usingRelative = false;
	data.abs.left = 0;
	data.abs.top = 0;
}

Position::Position(const RelativePosition &relPosition, short offsetLeft, short offsetTop)
{
	usingRelative = true;
	data.rel.position = relPosition;
	data.rel.x = offsetLeft;
	data.rel.y = offsetTop;
}

Position::Position(short absoluteLeft, short absoluteTop)
{
	usingRelative = false;
	data.abs.left = absoluteLeft;
	data.abs.top = absoluteTop;
}

void Position::derivePosition(int width, int height, int containerWidth, int containerHeight, int& x, int& y)
{
	if(!usingRelative)
	{
		x = data.abs.left;
		y = data.abs.top;
		return;
	}

#define X_LEFT		data.rel.x
#define X_CENTER	(containerWidth/2)-(width/2) + data.rel.x
#define X_RIGHT		containerWidth - width + data.rel.x

#define Y_TOP		data.rel.y
#define Y_MIDDLE	(containerHeight/2)-(height/2) + data.rel.y
#define Y_BOTTOM	containerHeight-height + data.rel.y

	switch(data.rel.position)
	{
	case Left:
		x = X_LEFT;
		y = Y_MIDDLE;
		break;
	case TopLeft:
		x = X_LEFT;
		y = Y_TOP;
		break;
	case TopCenter:
		x = X_CENTER;
		y = Y_TOP;
		break;
	case TopRight:
		x = X_RIGHT;
		y = Y_TOP;
		break;
	case Right:
		x = X_RIGHT;
		y = Y_MIDDLE;
		break;
	case BottomRight:
		x = X_RIGHT;
		y = Y_BOTTOM;
		break;
	case BottomCenter:
		x = X_CENTER;
		y = Y_BOTTOM;
		break;
	case BottomLeft:
		x = X_LEFT;
		y = Y_BOTTOM;
		break;
	case Center:
		x = X_CENTER;
		y = Y_MIDDLE;
		break;
	default:
		x = X_LEFT;
		y = Y_TOP;
		break;
	}
}
