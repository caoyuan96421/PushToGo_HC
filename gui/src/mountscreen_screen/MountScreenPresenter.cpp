#include <gui/mountscreen_screen/MountScreenView.hpp>
#include <gui/mountscreen_screen/MountScreenPresenter.hpp>

MountScreenPresenter::MountScreenPresenter(MountScreenView& v) :
		BaseScreenPresenter(v.baseview), view(v)
{
}

void MountScreenPresenter::activate()
{
	BaseScreenPresenter::activate();
	view.updateDisplay(TelescopeBackend::getEqCoords(), TelescopeBackend::getMountCoords());
}

void MountScreenPresenter::deactivate()
{

}

void MountScreenPresenter::setCoords(const EquatorialCoordinates& eq, const MountCoordinates& meq)
{
	BaseScreenPresenter::setCoords(eq, meq);
	view.updateDisplay(eq, meq);
}

