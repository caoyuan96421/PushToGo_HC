#ifndef SETTINGSCREEN_VIEW_HPP
#define SETTINGSCREEN_VIEW_HPP

#include <gui_generated/settingscreen_screen/SettingScreenViewBase.hpp>
#include <gui/settingscreen_screen/SettingScreenPresenter.hpp>
#include <gui/basescreen_screen/BaseScreenView.hpp>
#include <widgets/ConfigButton.h>
#include <widgets/AccordionMenu.h>
#include <widgets/ButtonItem.h>
#include <gui/BaseScreenAdaptor.h>
#include "TelescopeBackend.h"

class SettingScreenView: public SettingScreenViewBase, public BaseScreenAdaptor {
public:
	SettingScreenView();
	virtual ~SettingScreenView();
	virtual void setupScreen();
	virtual void tearDownScreen();
	void handleClickEvent(const ClickEvent &evt) {
		if (evt.getType() == ClickEvent::PRESSED) {
			lastPressed.x = evt.getX();
			lastPressed.y = evt.getY();
		}
		Screen::handleClickEvent(evt);
	}
//	void handleGestureEvent(const GestureEvent &evt) {
//		if (evt.getType() == GestureEvent::SWIPE_HORIZONTAL
//				&& evt.getVelocity() > MIN_SWIPE_VELOCITY
//				&& !scrollableContainer1.getRect().intersect(lastPressed.x,
//						lastPressed.y) && !configPopup1.isVisible()) {
//			application().gotoHomeScreenScreenSlideTransitionWest();
//		}
//		Screen::handleGestureEvent(evt);
//	}
protected:
	static const int MAX_CONFIG = 64;
	ConfigItem configs[MAX_CONFIG];
	int num_config;

	Accordion accordion;
	AccordionMenu *hc_menu;
	AccordionMenu *mount_menu;
	ButtonItem *abuttons[MAX_CONFIG];

	Unicode::UnicodeChar name1[20];
	Unicode::UnicodeChar name2[20];
	Unicode::UnicodeChar name3[20];
	Unicode::UnicodeChar name4[20];

	touchgfx::Callback<SettingScreenView, const ButtonItem&> configCallback;
	touchgfx::Callback<SettingScreenView, ConfigItem*, bool> configOKCallback;
	touchgfx::Callback<SettingScreenView, const AbstractButton&> configSaveCallback;
	touchgfx::Callback<SettingScreenView, const Slider&, int> brightnessCallback;
	touchgfx::Callback<SettingScreenView, const AbstractButton&> colorCallback;

	static void setupScreen_delayed(void *param);

	void configButtonPressed(const ButtonItem&);
	void configSavePressed(const AbstractButton&);
	void configSet(ConfigItem*, bool ok);
	void brightnessChanged(const Slider&, int);
	void colorToggled(const AbstractButton &);

	struct {
		int x;
		int y;
	} lastPressed;
};

#endif // SETTINGSCREEN_VIEW_HPP
