#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <gui_generated/containers/JoyStickBase.hpp>

class JoyStick : public JoyStickBase
{
public:
    JoyStick();
    virtual ~JoyStick() {}

    virtual void initialize();
protected:
};

#endif // JOYSTICK_HPP
