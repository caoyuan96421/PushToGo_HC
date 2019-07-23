/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/CoordinatePopupBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

CoordinatePopupBase::CoordinatePopupBase()
{
    setWidth(480);
    setHeight(750);

    box1.setPosition(45, 111, 391, 579);
    box1.setColor(touchgfx::Color::getColorFrom24BitRGB(74, 16, 54));

    okButton.setXY(60, 604);
    okButton.setBitmaps(Bitmap(BITMAP_BUTTON2_ID), Bitmap(BITMAP_BUTTON2_PRESSED_ID));
    okButton.setLabelText(TypedText(T_SINGLEUSEID81));
    okButton.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(135, 135, 135));
    okButton.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(219, 170, 170));

    cancelButton.setXY(251, 604);
    cancelButton.setBitmaps(Bitmap(BITMAP_BUTTON2_ID), Bitmap(BITMAP_BUTTON2_PRESSED_ID));
    cancelButton.setLabelText(TypedText(T_SINGLEUSEID82));
    cancelButton.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(135, 135, 135));
    cancelButton.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(219, 170, 170));

    unit11.setXY(171, 231);
    unit11.setColor(touchgfx::Color::getColorFrom24BitRGB(207, 114, 114));
    unit11.setLinespacing(0);
    Unicode::snprintf(unit11Buffer, UNIT11_SIZE, "%s", TypedText(T_SINGLEUSEID84).getText());
    unit11.setWildcard(unit11Buffer);
    unit11.resizeToCurrentText();
    unit11.setTypedText(TypedText(T_SINGLEUSEID83));

    unit12.setXY(281, 231);
    unit12.setColor(touchgfx::Color::getColorFrom24BitRGB(207, 114, 114));
    unit12.setLinespacing(0);
    Unicode::snprintf(unit12Buffer, UNIT12_SIZE, "%s", TypedText(T_SINGLEUSEID88).getText());
    unit12.setWildcard(unit12Buffer);
    unit12.resizeToCurrentText();
    unit12.setTypedText(TypedText(T_SINGLEUSEID85));

    unit13.setXY(391, 231);
    unit13.setColor(touchgfx::Color::getColorFrom24BitRGB(207, 114, 114));
    unit13.setLinespacing(0);
    Unicode::snprintf(unit13Buffer, UNIT13_SIZE, "%s", TypedText(T_SINGLEUSEID89).getText());
    unit13.setWildcard(unit13Buffer);
    unit13.resizeToCurrentText();
    unit13.setTypedText(TypedText(T_SINGLEUSEID87));

    unit21.setXY(171, 463);
    unit21.setColor(touchgfx::Color::getColorFrom24BitRGB(207, 114, 114));
    unit21.setLinespacing(0);
    Unicode::snprintf(unit21Buffer, UNIT21_SIZE, "%s", TypedText(T_SINGLEUSEID91).getText());
    unit21.setWildcard(unit21Buffer);
    unit21.resizeToCurrentText();
    unit21.setTypedText(TypedText(T_SINGLEUSEID90));

    unit22.setXY(281, 463);
    unit22.setColor(touchgfx::Color::getColorFrom24BitRGB(207, 114, 114));
    unit22.setLinespacing(0);
    Unicode::snprintf(unit22Buffer, UNIT22_SIZE, "%s", TypedText(T_SINGLEUSEID93).getText());
    unit22.setWildcard(unit22Buffer);
    unit22.resizeToCurrentText();
    unit22.setTypedText(TypedText(T_SINGLEUSEID92));

    unit23.setXY(391, 463);
    unit23.setColor(touchgfx::Color::getColorFrom24BitRGB(207, 114, 114));
    unit23.setLinespacing(0);
    Unicode::snprintf(unit23Buffer, UNIT23_SIZE, "%s", TypedText(T_SINGLEUSEID95).getText());
    unit23.setWildcard(unit23Buffer);
    unit23.resizeToCurrentText();
    unit23.setTypedText(TypedText(T_SINGLEUSEID94));

    name1.setPosition(60, 163, 360, 30);
    name1.setColor(touchgfx::Color::getColorFrom24BitRGB(191, 177, 186));
    name1.setLinespacing(0);
    Unicode::snprintf(name1Buffer, NAME1_SIZE, "%s", TypedText(T_SINGLEUSEID97).getText());
    name1.setWildcard(name1Buffer);
    name1.setTypedText(TypedText(T_SINGLEUSEID96));

    name2.setPosition(60, 385, 360, 30);
    name2.setColor(touchgfx::Color::getColorFrom24BitRGB(191, 177, 186));
    name2.setLinespacing(0);
    Unicode::snprintf(name2Buffer, NAME2_SIZE, "%s", TypedText(T_SINGLEUSEID99).getText());
    name2.setWildcard(name2Buffer);
    name2.setTypedText(TypedText(T_SINGLEUSEID98));

    sign1.setXY(47, 222);
    sign1.setColor(touchgfx::Color::getColorFrom24BitRGB(207, 114, 114));
    sign1.setLinespacing(0);
    Unicode::snprintf(sign1Buffer, SIGN1_SIZE, "%s", TypedText(T_SINGLEUSEID101).getText());
    sign1.setWildcard(sign1Buffer);
    sign1.resizeToCurrentText();
    sign1.setTypedText(TypedText(T_SINGLEUSEID100));

    sign2.setXY(47, 454);
    sign2.setColor(touchgfx::Color::getColorFrom24BitRGB(207, 114, 114));
    sign2.setLinespacing(0);
    Unicode::snprintf(sign2Buffer, SIGN2_SIZE, "%s", TypedText(T_SINGLEUSEID103).getText());
    sign2.setWildcard(sign2Buffer);
    sign2.resizeToCurrentText();
    sign2.setTypedText(TypedText(T_SINGLEUSEID102));

    add(box1);
    add(okButton);
    add(cancelButton);
    add(unit11);
    add(unit12);
    add(unit13);
    add(unit21);
    add(unit22);
    add(unit23);
    add(name1);
    add(name2);
    add(sign1);
    add(sign2);
}

void CoordinatePopupBase::initialize()
{
	
}