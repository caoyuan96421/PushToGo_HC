#ifndef ALIGNSCREEN_VIEW_HPP
#define ALIGNSCREEN_VIEW_HPP

#include <gui_generated/alignscreen_screen/AlignScreenViewBase.hpp>
#include <gui/alignscreen_screen/AlignScreenPresenter.hpp>

class AlignScreenView : public AlignScreenViewBase
{
public:
    AlignScreenView();
    virtual ~AlignScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // ALIGNSCREEN_VIEW_HPP
