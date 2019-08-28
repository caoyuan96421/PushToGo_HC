#include <gui/starmapscreen_screen/StarMapScreenView.hpp>
#include <cstdio>

StarMapScreenView::StarMapScreenView() :
		buttonZoomCallback(this, &StarMapScreenView::buttonZoomPressed), buttonGotoCallback(
				this, &StarMapScreenView::goTo), toggleConstellCallback(this,
				&StarMapScreenView::toggleConstellSwitched), toggleFollowCallback(
				this, &StarMapScreenView::toggleFollowSwitched), toggleEquatorialCallback(
				this, &StarMapScreenView::toggleEquatorialSwitched), starSelectedCallback(
				this, &StarMapScreenView::starSelected), starMapAnimatedCallback(
				this, &StarMapScreenView::animationFinished), follow(true), selected_star(
				NULL) {
	starmap.setPosition(20, 20, 440, 440);
	starmap.setSelectionCallback(starSelectedCallback);

	add(starmap);

	tim = get_millisec_time();

	baseview.addTo(&container);
}

void StarMapScreenView::setupScreen() {
	starmap.setCenter(TelescopeBackend::getEqCoords());
	starmap.setFoV(presenter->getFoV());

	button_zoomin.setAction(buttonZoomCallback);
	button_zoomout.setAction(buttonZoomCallback);
	button_goto.setAction(buttonGotoCallback);

	toggleConstell.setAction(toggleConstellCallback);
	toggleConstell.forceState(presenter->isShowConstellation());
	starmap.setDrawConstell(presenter->isShowConstellation());

	toggleFollow.setAction(toggleFollowCallback);
	toggleFollow.forceState(presenter->isFollow());
	follow = presenter->isFollow();
	if (presenter->isFollow()) {
		starmap.setDraggable(false);
	}

	toggleEquatorial.setAction(toggleEquatorialCallback);
	toggleEquatorial.forceState(presenter->isEquatorial());
	starmap.setEquatorial(presenter->isEquatorial());
}

void StarMapScreenView::tearDownScreen() {
	// Save FoV before leaving
	presenter->setFoV(starmap.getFoV());
}

void StarMapScreenView::draw(Rect &rect) {
//	int timestart = tim.read_us();

	Screen::draw(rect);

//	printf("Time consumed to draw screen: %d us\r\n", tim.read_us() - timestart);
}

void StarMapScreenView::buttonZoomPressed(const AbstractButton &src) {
	float fov = starmap.getFoV();

	if (&src == &button_zoomin || &src == &button_zoomout) {
		if (&src == &button_zoomout) {
			fov *= 1.25;
		} else {
			fov /= 1.25;
		}
		if (fov > 45) {
			// Max FOV: 45 deg
			fov = 45;
		}
		if (fov < 0.5) {
			// Min FOV: 1 deg
			fov = 0.5;
		}
		starmap.setFoV(fov);
	}
}

void StarMapScreenView::toggleConstellSwitched(const AbstractButton &src) {
	if (((const ToggleButton&) src).getState()) {
		starmap.setDrawConstell(true);
		presenter->setShowConstellation(true);
	} else {
		starmap.setDrawConstell(false);
		presenter->setShowConstellation(false);
	}
}

void StarMapScreenView::toggleFollowSwitched(const AbstractButton &src) {
	if (((const ToggleButton&) src).getState()) {
		follow = true;
		starmap.setDraggable(false);
		presenter->setFollow(true);
	} else {
		follow = false;
		starmap.setDraggable(true);
		presenter->setFollow(false);
	}
}

void StarMapScreenView::toggleEquatorialSwitched(const AbstractButton &src) {
	if (((const ToggleButton&) src).getState()) {
		presenter->setEquatorial(true);
		starmap.setEquatorial(true);
	} else {
		presenter->setEquatorial(false);
		starmap.setEquatorial(false);
	}
}

void StarMapScreenView::starSelected(const SkyObjInfo *star) {
	if (star) {
		EquatorialCoordinates eq(star->DEC, star->RA);
		LocationCoordinates loc = LocationCoordinates(
				TelescopeBackend::getConfigDouble("latitude"),
				TelescopeBackend::getConfigDouble("longitude"));
		time_t time_now = TelescopeBackend::getTime();
		LocalEquatorialCoordinates leq = eq.toLocalEquatorialCoordinates(
				time_now, loc);
		AzimuthalCoordinates az = leq.toAzimuthalCoordinates(loc);

		double r = (eq.ra < 0) ? eq.ra + 360.0 : eq.ra;
		double d = fabs(eq.dec);
		double h = (leq.ha < 0) ? leq.ha + 360.0 : leq.ha;

		char buf[TEXTINFO_SIZE];
		snprintf(buf, sizeof(buf), "RA: %2dh%02d'%02d\"\n"
				"Dec: %c%2d\x00b0%02d'%02d\"\n"
				"HA: %2dh%02d'%02d\"\n"
				"Altitude: %.2f\x00b0\n"
				"Magnitude: %.2f\n", int(r / 15), int(fmod(r, 15.0) * 4),
				(int) round(fmod(r, 0.25) * 240), eq.dec > 0 ? '+' : '-',
				int(d), int(fmod(d, 1.0) * 60),
				(int) round(fmod(d, 1.0 / 60) * 3600), int(h / 15),
				int(fmod(h, 15.0) * 4), (int) round(fmod(h, 0.25) * 240),
				az.alt, star->magnitude);

		Unicode::strncpy(textInfoBuffer, buf, TEXTINFO_SIZE);
		textInfo.invalidate();
		selected_star = star;
	} else {
		textInfoBuffer[0] = 0;
		textInfo.invalidate();
		selected_star = NULL;
	}
}

void StarMapScreenView::goTo(const AbstractButton &src) {
//	showMessage("Function not implemented");
	if (!follow || selected_star != NULL) {
		EquatorialCoordinates eq_now = TelescopeBackend::getEqCoords();
		if (selected_star) {
			goto_target = EquatorialCoordinates(selected_star->DEC,
					selected_star->RA);
		} else {
			goto_target = starmap.getCenter();
		}
		starmap.setCenter(eq_now.ra, eq_now.dec, true,
				&starMapAnimatedCallback);
	}
}

void StarMapScreenView::updateCenter(const EquatorialCoordinates &eq) {
	if (follow) {
		starmap.setCenter(eq);
	}
}

void StarMapScreenView::animationFinished() {
	follow = true;
	toggleFollow.forceState(true);
	toggleFollow.invalidate();
	starmap.setDraggable(false);

	TelescopeBackend::goTo(goto_target);
}
