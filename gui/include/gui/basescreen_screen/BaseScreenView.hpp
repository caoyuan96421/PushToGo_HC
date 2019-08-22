#ifndef BASESCREEN_VIEW_HPP
#define BASESCREEN_VIEW_HPP

#include <gui_generated/basescreen_screen/BaseScreenViewBase.hpp>
#include <gui/basescreen_screen/BaseScreenPresenter.hpp>
#include <ctime>
#include "CelestialMath.h"

class BaseScreenView : public BaseScreenViewBase
{
public:
    BaseScreenView();
    virtual ~BaseScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void addTo(Container *scr);
    void setTime(time_t timestamp, int tz);
    void setEqCoords(const EquatorialCoordinates &eq);
    void showMessage(const char *msg);
protected:
};

#endif // BASESCREEN_VIEW_HPP
