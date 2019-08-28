#include <gui/containers/JoyStick.hpp>
#include <cmath>
#include "Debug.h"

#define JS_DEBUG false


JoyStick::JoyStick() {
	this->setTouchable(true);
	posChangedCallback = NULL;
	xcurr = 0;
	ycurr = 0;
	zone = 0;
}

void JoyStick::handleClickEvent(const ClickEvent &evt) {
	if (evt.getType() == ClickEvent::PRESSED) {
		debug_if(JS_DEBUG, "press\r\n");
		updateStickPosition(evt.getX(), evt.getY());
	} else if (evt.getType() == ClickEvent::RELEASED) {
		debug_if(JS_DEBUG, "release\r\n");
		evt.getType();
		resetStick();
	}
}

void JoyStick::handleDragEvent(const DragEvent &evt) {
	if (evt.getType() == DragEvent::DRAGGED) {
		debug_if(JS_DEBUG, "drag\r\n");
		int x = evt.getNewX();
		int y = evt.getNewY();
		if (x < 0)
			x = 0;
		if (x > getWidth())
			x = getWidth();
		if (y < 0)
			y = 0;
		if (y > getHeight())
			y = getHeight();
		updateStickPosition(x, y);
	}
}

void JoyStick::resetStick() {
	updateStickPosition(getWidth() / 2, getHeight() / 2);
}


void JoyStick::updateStickPosition(int x, int y) {
	debug_if(JS_DEBUG, "x=%d y=%d\r\n", x, y);
	handle.setX(x - handle.getWidth() / 2);
	handle.setY(y - handle.getHeight() / 2);
	invalidate();

	if (x == getWidth() / 2 && y == getHeight() / 2) {
		// Reset position
		if (posChangedCallback) {
			posChangedCallback(0, 0);
		}
		xcurr = 0;
		ycurr = 0;
		zone = 0;
	} else {
		// Calculate zone
		float dX = ((float) x / getWidth() - 0.5f) * 2;
		float dY = -((float) y / getHeight() - 0.5f) * 2;

		if (posChangedCallback){
			posChangedCallback(dX, dY);
		}
	}
}
