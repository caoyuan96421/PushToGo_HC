#ifndef SETTINGSCREEN_PRESENTER_HPP
#define SETTINGSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/basescreen_screen/BaseScreenPresenter.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SettingScreenView;

class SettingScreenPresenter : public BaseScreenPresenter
{
public:
    SettingScreenPresenter(SettingScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~SettingScreenPresenter() {};

    bool isRed(){
    	return model->isMonochromeRed();
    }

    void setRed(bool red){
    	return model->setMonochromeRed(red);
    }

    int getBrightness(){
    	return model->getBrightness();
    }

    void setBrightness(int b){
    	model->setBrightness(b);
    }

private:
    SettingScreenPresenter();

    SettingScreenView& view;
};


#endif // SETTINGSCREEN_PRESENTER_HPP
