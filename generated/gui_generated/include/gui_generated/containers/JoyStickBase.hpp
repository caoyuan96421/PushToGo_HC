/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef JOYSTICK_BASE_HPP
#define JOYSTICK_BASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>

class JoyStickBase : public touchgfx::Container
{
public:
    JoyStickBase();
    virtual ~JoyStickBase() {}

    virtual void initialize();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Image image1;
    touchgfx::Image image2;
    touchgfx::Image image3;
    touchgfx::Image image4;
    touchgfx::Image handle;

private:

};

#endif // JOYSTICK_BASE_HPP