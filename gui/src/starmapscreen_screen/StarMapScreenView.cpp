#include <gui/starmapscreen_screen/StarMapScreenView.hpp>
#include <cstdio>

StarMapScreenView::StarMapScreenView() :
		buttonZoomCallback(this, &StarMapScreenView::buttonZoomPressed), buttonGotoCallback(
				this, &StarMapScreenView::goTo), toggleConstellCallback(this,
				&StarMapScreenView::toggleConstellSwitched), toggleFollowCallback(
				this, &StarMapScreenView::toggleFollowSwitched), starSelectedCallback(
				this, &StarMapScreenView::starSelected), starMapAnimatedCallback(
				this, &StarMapScreenView::animationFinished), follow(true) {
	starmap.setPosition(20, 20, 440, 440);
	starmap.setSelectionCallback(starSelectedCallback);
	starmap.setDraggable(false);

	add(starmap);

	button_zoomin.setAction(buttonZoomCallback);
	button_zoomout.setAction(buttonZoomCallback);
	button_goto.setAction(buttonGotoCallback);

	toggleConstell.setAction(toggleConstellCallback);
	toggleConstell.forceState(false);

	toggleFollow.setAction(toggleFollowCallback);
	toggleFollow.forceState(true);

	tim = get_millisec_time();

	baseview.addTo(&container);
}

void StarMapScreenView::setupScreen() {
	starmap.aimAt(TelescopeBackend::getEqCoords());
	starmap.setFOV(5);
}

void StarMapScreenView::tearDownScreen() {

}

void StarMapScreenView::draw(Rect &rect) {
//	int timestart = tim.read_us();

	Screen::draw(rect);

//	printf("Time consumed to draw screen: %d us\r\n", tim.read_us() - timestart);
}

void StarMapScreenView::buttonZoomPressed(const AbstractButton &src) {
	float fov = starmap.getFOV();

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
		starmap.setFOV(fov);
	}
}

void StarMapScreenView::toggleConstellSwitched(const AbstractButton &src) {
	if (((const ToggleButton&) src).getState()) {
		starmap.setDrawConstell(true);
	} else {
		starmap.setDrawConstell(false);
	}
}

void StarMapScreenView::toggleFollowSwitched(const AbstractButton &src) {
	if (((const ToggleButton&) src).getState()) {
		follow = true;
		starmap.setDraggable(false);
	} else {
		follow = false;
		starmap.setDraggable(true);
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
	} else {
		textInfoBuffer[0] = 0;
		textInfo.invalidate();
	}
}

void StarMapScreenView::goTo(const AbstractButton &src) {
//	showMessage("Function not implemented");
	if (!follow){
		EquatorialCoordinates eq_now = TelescopeBackend::getEqCoords();
		goto_target = starmap.getAim();
		starmap.aimAt(eq_now.ra, eq_now.dec, true, &starMapAnimatedCallback);
	}
}

void StarMapScreenView::animationFinished(){
	follow = true;
	toggleFollow.forceState(true);
	toggleFollow.invalidate();
	starmap.setDraggable(false);

	TelescopeBackend::goTo(goto_target);
}
