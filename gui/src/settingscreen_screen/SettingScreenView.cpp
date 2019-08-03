#include <gui/settingscreen_screen/SettingScreenView.hpp>
#include "BitmapDatabase.hpp"
#include <cstdlib>
#include <cstring>
#include "Debug.h"
#define SS_DEBUG	1

using namespace touchgfx;

SettingScreenView::SettingScreenView() :
		configCallback(this, &SettingScreenView::configButtonPressed), configOKCallback(this, &SettingScreenView::configSet),
		configSaveCallback(this, &SettingScreenView::configSavePressed)
{
	baseview.addTo(&container);

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
}

SettingScreenView::~SettingScreenView()
{

}

void SettingScreenView::setupScreen_delayed(void *param)
{
	SettingScreenView *view = (SettingScreenView *) param;

	// Setup menu structure
	view->num_config = TelescopeBackend::getConfigAll(view->configs, MAX_CONFIG);

	debug_if(SS_DEBUG, "# of config: %d\r\n", view->num_config);

	for (int i = 0; i < view->num_config; i++)
	{
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
}

void SettingScreenView::tearDownScreen()
{
	// Delete ALL allocated resources
	for (int i = 0; i < num_config; i++)
	{
		delete[] abuttons[i]->getName();
		delete[] abuttons[i]->getValue();
		delete abuttons[i];
	}
}

void SettingScreenView::configButtonPressed(const ButtonItem& button)
{
	ConfigItem *config = (ConfigItem *) button.getUserData();
	configPopup1.editConfig(config);
	configPopup1.setCallback(&configOKCallback);
}

void SettingScreenView::configSavePressed(const AbstractButton&)
{
	TelescopeBackend::saveConfig();
}

void SettingScreenView::configSet(ConfigItem* config, bool ok)
{
	if (ok)
	{
		TelescopeBackend::writeConfig(config);
	}
}
