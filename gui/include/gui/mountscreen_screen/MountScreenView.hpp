#ifndef MOUNTSCREEN_VIEW_HPP
#define MOUNTSCREEN_VIEW_HPP

#include <gui_generated/mountscreen_screen/MountScreenViewBase.hpp>
#include <gui/mountscreen_screen/MountScreenPresenter.hpp>

class MountScreenView : public MountScreenViewBase
{
public:
    MountScreenView();
    virtual ~MountScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // MOUNTSCREEN_VIEW_HPP
