#ifndef STARMAPSCREEN_VIEW_HPP
#define STARMAPSCREEN_VIEW_HPP

#include <gui_generated/starmapscreen_screen/StarMapScreenViewBase.hpp>
#include <gui/starmapscreen_screen/StarMapScreenPresenter.hpp>

class StarMapScreenView : public StarMapScreenViewBase
{
public:
    StarMapScreenView();
    virtual ~StarMapScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // STARMAPSCREEN_VIEW_HPP
