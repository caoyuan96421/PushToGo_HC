#include <gui/settingscreen_screen/SettingScreenView.hpp>
#include "BitmapDatabase.hpp"
#include <cstdlib>
#include <cstring>
#include "Debug.h"

#ifndef SIMULATOR
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_dsi.h"
#include "stm32469i_discovery_lcd.h"
extern DSI_HandleTypeDef hdsi_eval;
#endif

#define SS_DEBUG	1

using namespace touchgfx;

SettingScreenView::SettingScreenView() :
		configCallback(this, &SettingScreenView::configButtonPressed), configOKCallback(
				this, &SettingScreenView::configSet), configSaveCallback(this,
				&SettingScreenView::configSavePressed), brightnessCallback(this,
				&SettingScreenView::brightnessChanged), colorCallback(this,
				&SettingScreenView::colorToggled) {
	scrollableContainer1.setScrollThreshold(0);

	accordion.setXY(0, 0);
	accordion.setWidth(scrollableContainer1.getWidth());

	static const int menuHeight = 100;
	hc_menu = accordion.createMenu("Hand Control Configuration", menuHeight);
	mount_menu = accordion.createMenu("Mount Configuration", menuHeight);

	scrollableContainer1.add(accordion);

	num_config = 0;

	buttonSave.setAction(configSaveCallback);
	configPopup1.setVisible(false);

	baseview.addTo(&container);
}

SettingScreenView::~SettingScreenView() {

}

void SettingScreenView::setupScreen_delayed(void *param) {
	debug_if(1, "delayed init started\r\n");
	SettingScreenView *view = (SettingScreenView*) param;

	// Setup menu structure
	view->num_config = TelescopeBackend::getConfigAll(view->configs,
			MAX_CONFIG);

	debug_if(SS_DEBUG, "# of config: %d\r\n", view->num_config);

	for (int i = 0; i < view->num_config; i++) {
		view->abuttons[i] = new ButtonItem;
		if (!view->abuttons[i])
			break;

		int nameLen = strlen(view->configs[i].name) + 1;
		Unicode::UnicodeChar *nameBuf = new Unicode::UnicodeChar[nameLen];
		if (!nameBuf)
			break;
		Unicode::strncpy(nameBuf, view->configs[i].name, nameLen);
		view->abuttons[i]->setName(nameBuf);

		Unicode::UnicodeChar *valueBuf = new Unicode::UnicodeChar[32];
		if (!valueBuf)
			break;
		*valueBuf = 0;
		view->abuttons[i]->setValue(valueBuf);

		//Associate the config with the button for easier callback
		view->abuttons[i]->setUserData(&view->configs[i]);

		view->mount_menu->addItem(*view->abuttons[i], view->configCallback);
	}
	view->mount_menu->invalidate();

	debug_if(1, "delayed init finished\r\n");
#ifndef SIMULATOR
	vTaskDelete(NULL);
#endif
}

void SettingScreenView::setupScreen() {
#ifndef SIMULATOR
	xTaskCreate(setupScreen_delayed, (TASKCREATE_NAME_TYPE)"_delayed", 1024,
			this, tskIDLE_PRIORITY + 1, NULL);
#else
			setupScreen_delayed(this); // call directly
#endif

	toggleColor.forceState(((SettingScreenPresenter*) presenter)->isRed());
	toggleColor.setAction(colorCallback);

	sliderBrightness.setValue(((SettingScreenPresenter*) presenter)->getBrightness());
	sliderBrightness.setNewValueCallback(brightnessCallback);
}

void SettingScreenView::tearDownScreen() {
// Delete ALL allocated resources
	for (int i = 0; i < num_config; i++) {
		delete[] abuttons[i]->getName();
		delete[] abuttons[i]->getValue();
		delete abuttons[i];
	}
}

void SettingScreenView::configButtonPressed(const ButtonItem &button) {
	ConfigItem *config = (ConfigItem*) button.getUserData();
	configPopup1.editConfig(config);
	configPopup1.setCallback(&configOKCallback);
}

void SettingScreenView::configSavePressed(const AbstractButton&) {
	TelescopeBackend::saveConfig();
}

void SettingScreenView::configSet(ConfigItem *config, bool ok) {
	if (ok) {
		TelescopeBackend::writeConfig(config);
	}
}

void SettingScreenView::brightnessChanged(const Slider&, int newval) {
#ifndef SIMULATOR
	if (newval < 5)
		newval = 5;
	HAL_DSI_ShortWrite(&hdsi_eval, 0, DSI_DCS_SHORT_PKT_WRITE_P1,
	OTM8009A_CMD_WRDISBV, newval);
#endif
	((SettingScreenPresenter*) presenter)->setBrightness(newval);
}

void SettingScreenView::colorToggled(const AbstractButton &btn) {
	bool state = ((const ToggleButton&) btn).getState();
#ifndef SIMULATOR

	HAL_DSI_ShortWrite(&hdsi_eval, 0, DSI_DCS_SHORT_PKT_WRITE_P0, OTM8009A_CMD_DISPOFF, 0);

	vTaskDelay(50);

// Enter CMD2 mode
	const uint8_t data[] = { 0x80, 0x09, 0x01 };
	HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 3, 0xFF,
			(uint8_t*) data);
	HAL_DSI_ShortWrite(&hdsi_eval, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x00, 0x80);
	HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, 0xFF,
			(uint8_t*) data);
// Shift address to 0x00
	HAL_DSI_ShortWrite(&hdsi_eval, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x00, 0x00);

	uint8_t params[33] = { 0 };
	if (!state) {
		// Set to color mode
		params[0] = 0x40;
		params[32] = 0x04;
		for (int i = 1; i <= 31; i++)
			params[i] = 0x44;
	}
	HAL_DSI_ShortWrite(&hdsi_eval, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x00, 0x00); // Address 0xED00~ED20, DGAMG (green)
	HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 33, 0xED, params);

	HAL_DSI_ShortWrite(&hdsi_eval, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x00, 0x00); // Address 0xEE00~EE20, DGAMB (blue)
	HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 33, 0xEE, params);

// Exit command mode after write
	const uint8_t data1[] = { 0xFF, 0xFF, 0xFF };
	HAL_DSI_ShortWrite(&hdsi_eval, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x00, 0x00);
	HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 3, 0xFF,
			(uint8_t*) data1);

	// Force refresh
    HAL_DSI_Refresh(&hdsi_eval);
    vTaskDelay(50);
	HAL_DSI_ShortWrite(&hdsi_eval, 0, DSI_DCS_SHORT_PKT_WRITE_P0, OTM8009A_CMD_DISPON, 0);
#endif

	((SettingScreenPresenter*) presenter)->setRed(state);
}
