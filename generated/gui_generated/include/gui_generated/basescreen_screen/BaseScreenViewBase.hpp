/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef BASESCREEN_VIEW_BASE_HPP
#define BASESCREEN_VIEW_BASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/basescreen_screen/BaseScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <gui/containers/MessagePopup.hpp>

class BaseScreenViewBase : public touchgfx::View<BaseScreenPresenter>
{
public:
    BaseScreenViewBase();
    virtual ~BaseScreenViewBase() {}

    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box box2;
    touchgfx::TextAreaWithOneWildcard ra_coord;
    touchgfx::TextAreaWithOneWildcard dec_coord;
    touchgfx::TextAreaWithOneWildcard time;
    touchgfx::TextAreaWithOneWildcard date;
    touchgfx::Button home_button;
    MessagePopup messagePopup1;

    /*
     * Wildcard Buffers
     */
    static const uint16_t RA_COORD_SIZE = 20;
    touchgfx::Unicode::UnicodeChar ra_coordBuffer[RA_COORD_SIZE];
    static const uint16_t DEC_COORD_SIZE = 20;
    touchgfx::Unicode::UnicodeChar dec_coordBuffer[DEC_COORD_SIZE];
    static const uint16_t TIME_SIZE = 20;
    touchgfx::Unicode::UnicodeChar timeBuffer[TIME_SIZE];
    static const uint16_t DATE_SIZE = 20;
    touchgfx::Unicode::UnicodeChar dateBuffer[DATE_SIZE];

private:

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

    /*
     * Callback Declarations
     */
    touchgfx::Callback<BaseScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

};

#endif // BASESCREEN_VIEW_BASE_HPP
