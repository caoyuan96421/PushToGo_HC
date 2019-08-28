#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <gui_generated/containers/JoyStickBase.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/widgets/AbstractButton.hpp>

class JoyStick: public JoyStickBase {
public:
	JoyStick();
	virtual ~JoyStick() {
	}

	void handleClickEvent(const ClickEvent&);
	void handleDragEvent(const DragEvent&);

	void resetStick();

	void setPositionChangedCallback(void (*cb)(float, float)) {
		posChangedCallback = cb;
	}

	void forcePosition(float x, float y) {
		if (fabsf(x) < 0.1 && fabsf(y) < 0.1) {
			setTouchable(true);
		} else {
			// Disable touch. Update is through Model
			setTouchable(false);
			handle.setXY(x / 2 * getWidth(), -y / 2 * getHeight());
		}
	}

	static const float thd; // Threshold

protected:

	// Current speed
	int xcurr;
	int ycurr;
	int zone;

	void (*posChangedCallback)(float, float);

	void updateStickPosition(int x, int y);
};

#endif // JOYSTICK_HPP
