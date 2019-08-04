#include <gui/basescreen_screen/BaseScreenView.hpp>
#include <gui/basescreen_screen/BaseScreenPresenter.hpp>

BaseScreenPresenter::BaseScreenPresenter(BaseScreenView& v) :
		view(v)
{
}

void BaseScreenPresenter::activate()
{
	view.setTime(TelescopeBackend::getTime(), TelescopeBackend::getTimeZone());
	view.setEqCoords(TelescopeBackend::getEqCoords());
}

void BaseScreenPresenter::deactivate()
{

}

void BaseScreenPresenter::setTime(time_t timestamp, int tz)
{
	view.setTime(timestamp, tz);
}

void BaseScreenPresenter::setCoords(const EquatorialCoordinates& eq, const MountCoordinates& meq)
{
	view.setEqCoords(eq);
}
