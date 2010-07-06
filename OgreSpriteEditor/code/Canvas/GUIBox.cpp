#include "GUIBox.h"

using namespace GUIElements;

Box::Box(int width, int height) : width(width), height(height), container(0)
{
}

void Box::setSize(int width, int height)
{
	flagDirty();
	this->width = width;
	this->height = height;
}

void Box::setPosition(const Position& position)
{
	flagDirty();
	this->position = position;
}

void Box::setBackground(const Fill& fill)
{
	flagDirty();
	background = fill;
}

void Box::setBorder(const Border& border)
{
	flagDirty();
	this->border = border;
}

void Box::setContainer(GUICanvas* container)
{
	this->container = container;
}

void Box::draw(Canvas* canvas, Atlas* atlas)
{
	int x, y;
	position.derivePosition(width, height, container->getViewport()->getActualWidth(),
		container->getViewport()->getActualHeight(), x, y);

	canvas->drawRectangle(x, y, width, height, background, border);
}

void Box::getBounds(int& left, int& top, int& right, int& bottom)
{
	assert(container);

	position.derivePosition(width, height, container->getViewport()->getActualWidth(),
		container->getViewport()->getActualHeight(), left, top);

	right = left + width;
	bottom = top + height;
}

void Box::flagDirty()
{
	if(container)
		container->setDirty();
}