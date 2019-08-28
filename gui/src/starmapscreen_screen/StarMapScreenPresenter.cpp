#include <gui/starmapscreen_screen/StarMapScreenView.hpp>
#include <gui/starmapscreen_screen/StarMapScreenPresenter.hpp>

StarMapScreenPresenter::StarMapScreenPresenter(StarMapScreenView& v) :
		BaseScreenPresenter(v.baseview), view(v)
{
}

void StarMapScreenPresenter::activate()
{
	BaseScreenPresenter::activate();
}

void StarMapScreenPresenter::deactivate()
{

}

void StarMapScreenPresenter::setCoords(const EquatorialCoordinates &eq,
		const MountCoordinates &meq) {
	BaseScreenPresenter::setCoords(eq, meq);
	view.updateCenter(eq);
}
