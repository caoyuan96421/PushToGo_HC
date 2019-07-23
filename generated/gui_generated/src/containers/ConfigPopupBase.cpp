/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/ConfigPopupBase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "BitmapDatabase.hpp"

ConfigPopupBase::ConfigPopupBase()
{
    setWidth(480);
    setHeight(750);

    box1.setPosition(45, 111, 391, 579);
    box1.setColor(touchgfx::Color::getColorFrom24BitRGB(74, 16, 54));

    box2.setPosition(68, 289, 344, 87);
    box2.setColor(touchgfx::Color::getColorFrom24BitRGB(35, 13, 48));

    nameArea.setPosition(60, 128, 360, 30);
    nameArea.setColor(touchgfx::Color::getColorFrom24BitRGB(191, 177, 186));
    nameArea.setLinespacing(0);
    Unicode::snprintf(nameAreaBuffer, NAMEAREA_SIZE, "%s", TypedText(T_SINGLEUSEID18).getText());
    nameArea.setWildcard(nameAreaBuffer);
    nameArea.setTypedText(TypedText(T_SINGLEUSEID17));

    valueArea.setPosition(74, 309, 333, 47);
    valueArea.setColor(touchgfx::Color::getColorFrom24BitRGB(212, 117, 117));
    valueArea.setLinespacing(0);
    Unicode::snprintf(valueAreaBuffer, VALUEAREA_SIZE, "%s", TypedText(T_SINGLEUSEID20).getText());
    valueArea.setWildcard(valueAreaBuffer);
    valueArea.setTypedText(TypedText(T_SINGLEUSEID19));

    okButton.setXY(60, 604);
    okButton.setBitmaps(Bitmap(BITMAP_BUTTON2_ID), Bitmap(BITMAP_BUTTON2_PRESSED_ID));
    okButton.setLabelText(TypedText(T_SINGLEUSEID21));
    okButton.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(135, 135, 135));
    okButton.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(219, 170, 170));

    cancelButton.setXY(251, 604);
    cancelButton.setBitmaps(Bitmap(BITMAP_BUTTON2_ID), Bitmap(BITMAP_BUTTON2_PRESSED_ID));
    cancelButton.setLabelText(TypedText(T_SINGLEUSEID22));
    cancelButton.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(135, 135, 135));
    cancelButton.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(219, 170, 170));

    downbutton.setXY(336, 457);
    downbutton.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID), Bitmap(BITMAP_DARK_ICONS_DOWN_ARROW_48_ID), Bitmap(BITMAP_DARK_ICONS_DOWN_ARROW_48_ID));
    downbutton.setIconXY(6, 16);

    upbutton.setXY(336, 391);
    upbutton.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID), Bitmap(BITMAP_DARK_ICONS_UP_ARROW_48_ID), Bitmap(BITMAP_DARK_ICONS_UP_ARROW_48_ID));
    upbutton.setIconXY(6, 16);

    button7.setXY(138, 391);
    button7.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    button7.setLabelText(TypedText(T_SINGLEUSEID23));
    button7.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    button7.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    button8.setXY(204, 391);
    button8.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    button8.setLabelText(TypedText(T_SINGLEUSEID24));
    button8.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    button8.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    button9.setXY(270, 391);
    button9.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    button9.setLabelText(TypedText(T_SINGLEUSEID25));
    button9.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    button9.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    button4.setXY(138, 457);
    button4.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    button4.setLabelText(TypedText(T_SINGLEUSEID26));
    button4.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    button4.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    button5.setXY(204, 457);
    button5.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    button5.setLabelText(TypedText(T_SINGLEUSEID27));
    button5.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    button5.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    button6.setXY(270, 457);
    button6.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    button6.setLabelText(TypedText(T_SINGLEUSEID28));
    button6.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    button6.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    button3.setXY(270, 523);
    button3.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    button3.setLabelText(TypedText(T_SINGLEUSEID29));
    button3.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    button3.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    button2.setXY(204, 523);
    button2.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    button2.setLabelText(TypedText(T_SINGLEUSEID30));
    button2.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    button2.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    button1.setXY(138, 523);
    button1.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    button1.setLabelText(TypedText(T_SINGLEUSEID31));
    button1.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    button1.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    button0.setXY(74, 523);
    button0.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    button0.setLabelText(TypedText(T_SINGLEUSEID32));
    button0.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    button0.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    buttondot.setXY(74, 457);
    buttondot.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    buttondot.setLabelText(TypedText(T_SINGLEUSEID33));
    buttondot.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    buttondot.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    buttoncr.setXY(74, 391);
    buttoncr.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    buttoncr.setLabelText(TypedText(T_SINGLEUSEID34));
    buttoncr.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    buttoncr.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    warning.setPosition(68, 202, 344, 28);
    warning.setVisible(false);
    warning.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 0, 0));
    warning.setLinespacing(0);
    warningBuffer[0] = 0;
    warning.setWildcard(warningBuffer);
    warning.setTypedText(TypedText(T_SINGLEUSEID35));

    buttonminus.setXY(336, 523);
    buttonminus.setBitmaps(Bitmap(BITMAP_BUTTON3_ID), Bitmap(BITMAP_BUTTON3_PRESSED_ID));
    buttonminus.setLabelText(TypedText(T_SINGLEUSEID36));
    buttonminus.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(100, 91, 130));
    buttonminus.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(160, 200, 214));

    helpArea.setPosition(60, 165, 360, 115);
    helpArea.setColor(touchgfx::Color::getColorFrom24BitRGB(142, 144, 194));
    helpArea.setLinespacing(0);
    Unicode::snprintf(helpAreaBuffer, HELPAREA_SIZE, "%s", TypedText(T_SINGLEUSEID38).getText());
    helpArea.setWildcard(helpAreaBuffer);
    helpArea.setTypedText(TypedText(T_SINGLEUSEID37));

    add(box1);
    add(box2);
    add(nameArea);
    add(valueArea);
    add(okButton);
    add(cancelButton);
    add(downbutton);
    add(upbutton);
    add(button7);
    add(button8);
    add(button9);
    add(button4);
    add(button5);
    add(button6);
    add(button3);
    add(button2);
    add(button1);
    add(button0);
    add(buttondot);
    add(buttoncr);
    add(warning);
    add(buttonminus);
    add(helpArea);
}

void ConfigPopupBase::initialize()
{
	
}
