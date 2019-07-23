/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/alignscreen_screen/AlignScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

AlignScreenViewBase::AlignScreenViewBase() :
    buttonCallback(this, &AlignScreenViewBase::buttonCallbackHandler)
{
    box1.setPosition(0, 0, 480, 800);
    box1.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    mount_button.setXY(10, 20);
    mount_button.setBitmaps(Bitmap(BITMAP_BUTTON2_ID), Bitmap(BITMAP_BUTTON2_PRESSED_ID));
    mount_button.setLabelText(TypedText(T_SINGLEUSEID111));
    mount_button.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(118, 141, 242));
    mount_button.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    mount_button.setAction(buttonCallback);

    Back_arrow_482.setXY(14, 26);
    Back_arrow_482.setBitmap(Bitmap(BITMAP_BACK_ARROW_48_ID));

    scrollableContainer1.setPosition(10, 96, 225, 191);
    scrollableContainer1.setScrollbarsColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));


    scrollableContainer1.setScrollbarsPermanentlyVisible();
    scrollableContainer1.setScrollbarsVisible(false);

    buttonRefresh.setXY(272, 20);
    buttonRefresh.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID), Bitmap(BITMAP_DARK_ICONS_REFRESH_48_ID), Bitmap(BITMAP_DARK_ICONS_REFRESH_48_ID));
    buttonRefresh.setIconXY(6, 8);

    text_description.setPosition(12, 313, 220, 155);
    text_description.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 32, 32));
    text_description.setLinespacing(0);
    Unicode::snprintf(text_descriptionBuffer, TEXT_DESCRIPTION_SIZE, "%s", TypedText(T_SINGLEUSEID117).getText());
    text_description.setWildcard(text_descriptionBuffer);
    text_description.setTypedText(TypedText(T_SINGLEUSEID112));

    scrollableContainer2.setPosition(245, 96, 225, 191);
    scrollableContainer2.setScrollbarsColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));


    scrollableContainer2.setScrollbarsPermanentlyVisible();
    scrollableContainer2.setScrollbarsVisible(false);

    buttonAdd.setXY(340, 20);
    buttonAdd.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID), Bitmap(BITMAP_DARK_ICONS_GO_NEXT_48_ID), Bitmap(BITMAP_DARK_ICONS_GO_NEXT_48_ID));
    buttonAdd.setIconXY(6, 7);

    buttonDelete.setXY(410, 20);
    buttonDelete.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID), Bitmap(BITMAP_DARK_ICONS_TRASH_48_ID), Bitmap(BITMAP_DARK_ICONS_TRASH_48_ID));
    buttonDelete.setIconXY(11, 6);

    buttonGoto.setXY(272, 301);
    buttonGoto.setBitmaps(Bitmap(BITMAP_BUTTON2_ID), Bitmap(BITMAP_BUTTON2_PRESSED_ID));
    buttonGoto.setLabelText(TypedText(T_SINGLEUSEID113));
    buttonGoto.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(118, 141, 242));
    buttonGoto.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));

    buttonAlign.setXY(272, 370);
    buttonAlign.setBitmaps(Bitmap(BITMAP_BUTTON2_ID), Bitmap(BITMAP_BUTTON2_PRESSED_ID));
    buttonAlign.setLabelText(TypedText(T_SINGLEUSEID114));
    buttonAlign.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(118, 141, 242));
    buttonAlign.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));

    text_alignment.setPosition(12, 468, 205, 203);
    text_alignment.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 32, 32));
    text_alignment.setLinespacing(0);
    Unicode::snprintf(text_alignmentBuffer, TEXT_ALIGNMENT_SIZE, "%s", TypedText(T_SINGLEUSEID116).getText());
    text_alignment.setWildcard(text_alignmentBuffer);
    text_alignment.setTypedText(TypedText(T_SINGLEUSEID115));

    joyStick2.setXY(226, 461);

    buttonStop.setXY(200, 20);
    buttonStop.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID), Bitmap(BITMAP_DARK_ICONS_REMOVE_48_ID), Bitmap(BITMAP_DARK_ICONS_REMOVE_48_ID));
    buttonStop.setIconXY(6, 6);

    sliderSpeed.setXY(-3, 654);
    sliderSpeed.setBitmaps(Bitmap(BITMAP_SLIDERSMALL_ROUND_BACK_ID), Bitmap(BITMAP_SLIDERSMALL_ROUND_FILL_ID), Bitmap(BITMAP_DARK_SLIDER_HORIZONTAL_LARGE_INDICATORS_SLIDER_ROUND_NOB_ID));
    sliderSpeed.setupHorizontalSlider(7, 11, 0, 0, 161);
    sliderSpeed.setValueRange(0, 5);
    sliderSpeed.setValue(0);

    add(box1);
    add(mount_button);
    add(Back_arrow_482);
    add(scrollableContainer1);
    add(buttonRefresh);
    add(text_description);
    add(scrollableContainer2);
    add(buttonAdd);
    add(buttonDelete);
    add(buttonGoto);
    add(buttonAlign);
    add(text_alignment);
    add(joyStick2);
    add(buttonStop);
    add(sliderSpeed);
}

void AlignScreenViewBase::setupScreen()
{
    joyStick2.initialize();
}

void AlignScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &mount_button)
    {
        //Interaction1
        //When mount_button clicked change screen to MountScreen
        //Go to MountScreen with screen transition towards West
        application().gotoMountScreenScreenSlideTransitionWest();
    }
    else if (&src == &buttonRefresh)
    {

    }
    else if (&src == &buttonAdd)
    {

    }
    else if (&src == &buttonDelete)
    {

    }
    else if (&src == &buttonGoto)
    {

    }
    else if (&src == &buttonAlign)
    {

    }
    else if (&src == &buttonStop)
    {

    }
}
