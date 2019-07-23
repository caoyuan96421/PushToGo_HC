/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/basescreen_screen/BaseScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "BitmapDatabase.hpp"

BaseScreenViewBase::BaseScreenViewBase() :
    buttonCallback(this, &BaseScreenViewBase::buttonCallbackHandler)
{
    box2.setPosition(0, 750, 480, 50);
    box2.setColor(touchgfx::Color::getColorFrom24BitRGB(36, 5, 5));

    ra_coord.setPosition(0, 750, 192, 25);
    ra_coord.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 0, 0));
    ra_coord.setLinespacing(0);
    Unicode::snprintf(ra_coordBuffer, RA_COORD_SIZE, "%s", TypedText(T_SINGLEUSEID2).getText());
    ra_coord.setWildcard(ra_coordBuffer);
    ra_coord.setTypedText(TypedText(T_SINGLEUSEID1));

    dec_coord.setPosition(0, 774, 192, 25);
    dec_coord.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 0, 0));
    dec_coord.setLinespacing(0);
    Unicode::snprintf(dec_coordBuffer, DEC_COORD_SIZE, "%s", TypedText(T_SINGLEUSEID5).getText());
    dec_coord.setWildcard(dec_coordBuffer);
    dec_coord.setTypedText(TypedText(T_SINGLEUSEID4));

    time.setPosition(275, 750, 200, 25);
    time.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 78, 132));
    time.setLinespacing(0);
    Unicode::snprintf(timeBuffer, TIME_SIZE, "%s", TypedText(T_SINGLEUSEID8).getText());
    time.setWildcard(timeBuffer);
    time.setTypedText(TypedText(T_SINGLEUSEID7));

    date.setPosition(275, 772, 200, 28);
    date.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 0, 0));
    date.setLinespacing(0);
    Unicode::snprintf(dateBuffer, DATE_SIZE, "%s", TypedText(T_SINGLEUSEID10).getText());
    date.setWildcard(dateBuffer);
    date.setTypedText(TypedText(T_SINGLEUSEID9));

    home_button.setXY(225, 758);
    home_button.setBitmaps(Bitmap(BITMAP_HOME_BUTTON_ID), Bitmap(BITMAP_HOME_BUTTON2_ID));
    home_button.setAction(buttonCallback);

    add(box2);
    add(ra_coord);
    add(dec_coord);
    add(time);
    add(date);
    add(home_button);
}

void BaseScreenViewBase::setupScreen()
{

}

void BaseScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &home_button)
    {
        //Interaction1
        //When home_button clicked change screen to HomeScreen
        //Go to HomeScreen with screen transition towards West
        application().gotoHomeScreenScreenSlideTransitionWest();
    }
}