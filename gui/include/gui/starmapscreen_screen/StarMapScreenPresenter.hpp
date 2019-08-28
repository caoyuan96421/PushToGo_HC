#ifndef STARMAPSCREEN_PRESENTER_HPP
#define STARMAPSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <gui/basescreen_screen/BaseScreenPresenter.hpp>

using namespace touchgfx;

class StarMapScreenView;

class StarMapScreenPresenter: public BaseScreenPresenter {
public:
	StarMapScreenPresenter(StarMapScreenView &v);

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

	virtual ~StarMapScreenPresenter() {
	}
	;

	virtual void setCoords(const EquatorialCoordinates &eq,
			const MountCoordinates &meq);

	bool isFollow() {
		return model->isFollow();
	}

	void setFollow(bool follow) {
		model->setFollow(follow);
	}

	bool isShowConstellation() {
		return model->isShowConstellation();
	}

	void setShowConstellation(bool showConst) {
		model->setShowConstellation(showConst);
	}

	bool isEquatorial() {
		return model->isEquatorial();
	}

	void setEquatorial(bool eq){
		model->setEquatorial(eq);
	}

	float getFoV(){
		return model->getFoV();
	}

	void setFoV(float fov){
		model->setFoV(fov);
	}

private:
	StarMapScreenPresenter();

	StarMapScreenView &view;
};

#endif // STARMAPSCREEN_PRESENTER_HPP
