/*
 * StarMapWidget.h
 *
 *  Created on: 2018Äê5ÔÂ4ÈÕ
 *      Author: caoyuan9642
 */

#ifndef TOUCHGFX_GUI_WIDGETS_STARMAPWIDGET_H_
#define TOUCHGFX_GUI_WIDGETS_STARMAPWIDGET_H_

#include <touchgfx/widgets/canvas/CanvasWidget.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/AbstractButton.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/Color.hpp>
#include "StarCatalog.h"
#include "PlanetMoon.h"
#include "TelescopeBackend.h"

#define STARMAPWIDGET_ANIMATE_MAX_DURATION 20

class StarMapWidget: public touchgfx::CanvasWidget {
public:
	StarMapWidget();
	virtual ~StarMapWidget();

	virtual bool drawCanvasWidget(const touchgfx::Rect &invalidatedArea) const;
	virtual void draw(const touchgfx::Rect &invalidatedArea) const;

	void aimAt(double ra, double dec, bool smooth = false, touchgfx::GenericCallback<> *cb = NULL) {
		ra = remainder(ra, 360.0);
		if (dec > 90.0)
			dec = 90.0;
		else if (dec < -90.0)
			dec = -90.0;
		if (!smooth) {
			ra_ctr = ra;
			dec_ctr = dec;
		} else {
			moveAnimationRunning = true;
			moveAnimationCounter = 0;
			moveAnimationStartX = ra_ctr;
			moveAnimationStartY = dec_ctr;
			moveAnimationEndX = ra;
			moveAnimationEndY = dec;
			moveAnimationCallback = cb;
			moveAnimationDuration = STARMAPWIDGET_ANIMATE_MAX_DURATION;
//					(uint16_t) (STARMAPWIDGET_ANIMATE_MAX_DURATION
//							* dist_angle(ra_ctr, dec_ctr, ra, dec) / 180.0F);
		}
		updateView();
		invalidate();
	}

	void aimAt(const EquatorialCoordinates &eq) {
		aimAt(eq.ra, eq.dec);
	}

	EquatorialCoordinates getAim() {
		return EquatorialCoordinates(dec_ctr, ra_ctr);
	}

	void setFOV(double fov) {
		if (fov > 45.0)
			fov = 45.0;
		if (fov < 0.5) {
			fov = 0.5;
		}
		fovw = fov;
		fovh = fov / getWidth() * getHeight();
		updateView();
		invalidate();
	}

	void setRotation(double r) {
		rot = r;
		updateView();
		invalidate();
	}

	double getFOV() {
		return fovw;
	}

	double getRotation() {
		return rot;
	}

	double getRA() {
		return ra_ctr;
	}

	double getDEC() {
		return dec_ctr;
	}

	void setColor(touchgfx::colortype c) {
		painter.setColor(c);
		invalidate();
	}

	touchgfx::colortype getColor() {
		return painter.getColor();
	}

	virtual void handleDragEvent(const touchgfx::DragEvent &evt);
	virtual void handleClickEvent(const touchgfx::ClickEvent &evt);
	virtual void handleTickEvent();
	virtual void handleGestureEvent(const touchgfx::GestureEvent &evt) {
	}

	const LocationCoordinates& getLocation() const {
		return location;
	}

	void setLocation(const LocationCoordinates &location) {
		this->location = location;
		updateView();
		invalidate();
	}

	void setDrawConstell(bool drawConstell) {
		this->drawConstell = drawConstell;
		invalidate();
	}

	void setSelectionCallback(
			touchgfx::GenericCallback<const SkyObjInfo*> &cb) {
		selectionCallback = &cb;
	}

	bool isDraggable() const {
		return draggable;
	}

	void setDraggable(bool draggable) {
		this->draggable = draggable;
	}

protected:
	double ra_ctr, dec_ctr; // Center of FOV
	float fovw; // Field of view along the width of the widget
	float fovh; // Field of view along the height of the widget
	float rot; // Rotation of FOV
	LocationCoordinates location;
	mutable touchgfx::PainterRGB888 painter;
	touchgfx::colortype labelColor;
	uint8_t labelAlpha;
	touchgfx::Bitmap moon_bitmap;
	touchgfx::GenericCallback<const SkyObjInfo*> *selectionCallback;
	mutable class TextureMapperEx: public touchgfx::TextureMapper {
	public:
		void setParent(Drawable *p) {
			this->parent = p;
		}
	} moon_texture;

	mutable class BoxEx: public touchgfx::Box {
	public:
		void setParent(Drawable *p) {
			this->parent = p;
		}
	} bgBox;

	bool drawConstell;

	void updateView();

private:
	// These are for internal use
//	mutable touchgfx::Canvas *canvas;
	mutable float xc, yc, zc;
	mutable float xp, yp, zp;
	mutable float xq, yq;
	mutable float fovr; // Field radius
	mutable bool renderSuccessful;
	mutable bool displayMoon, displaySun;
	mutable struct {
		touchgfx::CWRUtil::Q5 x;
		touchgfx::CWRUtil::Q5 y;
		touchgfx::CWRUtil::Q5 size;
		int apex;
		int illumangle;
	} moonPos, sunPos;

	mutable int tick_rotation;
	mutable SkyObjInfo *selected;

	bool ispressed;
	bool isdoubleclick;
	bool isdrag;
	bool first;
	int clickStartTime;
	int lastClickDuration;
	int clickX, clickY;
	unsigned long tim;
	time_t timestamp;
	bool draggable;
	bool moveAnimationRunning; ///< Boolean that is true if the animation is running
	uint16_t moveAnimationCounter; ///< Counter that is equal to the current step in the animation
//	uint16_t       moveAnimationDelay;     ///< A delay that is applied before animation start. Expressed in ticks.
	uint16_t moveAnimationDuration; ///< The complete duration of the animation. Expressed in ticks.
	double moveAnimationStartX; ///< The X value at the beginning of the animation.
	double moveAnimationStartY; ///< The Y value at the beginning of the animation.
	double moveAnimationEndX;      ///< The X value at the end of the animation.
	double moveAnimationEndY;      ///< The Y value at the end of the animation.
	touchgfx::GenericCallback<> *moveAnimationCallback; ///< The Y value at the end of the animation.
	touchgfx::EasingEquation moveAnimationEquation; ///< EasingEquation during the animation.

	static const int STARMAP_WIDGET_MAX_LABEL = 20;
	static const int STARMAP_WIDGET_MAX_LABEL_LENGTH = 20;
	static const int STARMAP_WIDGET_MAX_STARS = 500;
	mutable struct {
		char label[STARMAP_WIDGET_MAX_LABEL_LENGTH];
		int x;
		int y;
	} starlabels[STARMAP_WIDGET_MAX_LABEL];
	mutable int num_label;
	mutable struct StarPos {
		touchgfx::CWRUtil::Q5 x;
		touchgfx::CWRUtil::Q5 y;
		touchgfx::CWRUtil::Q5 size;
		touchgfx::colortype color;
		const SkyObjInfo *info;
	} visibleStars[STARMAP_WIDGET_MAX_STARS];
	mutable int num_stars;

	mutable struct SolarSystemInfo: SkyObjInfo {
		MoonPhase phase;
		EquatorialCoordinatesWithDist accurate_pos;
		PlanetMoon::Object obj;

		virtual bool isPlanet() const {
			return (obj != PlanetMoon::SUN) && (obj != PlanetMoon::MOON);
		}
		virtual ~SolarSystemInfo() {
		}
	} planetSunMoon[10];

	static void callback(SkyObjInfo*, void*);
	void _handleStarsAndDSO(const SkyObjInfo*, bool hires) const;
	void _handleSun(const SolarSystemInfo*) const;
	void _handleMoon(const SolarSystemInfo*) const;

	bool _calcScreenPosition(const SkyObjInfo*, bool isStar,
			touchgfx::CWRUtil::Q5 &xscr, touchgfx::CWRUtil::Q5 &yscr,
			float) const;

	void _drawstar(touchgfx::Canvas &canvas, StarPos&) const;
	void _drawsun(touchgfx::Canvas &canvas) const;
	void _drawmoon(touchgfx::Canvas &canvas) const;
	void _drawticks(touchgfx::Canvas &canvas, touchgfx::CWRUtil::Q5 x,
			touchgfx::CWRUtil::Q5 y, touchgfx::CWRUtil::Q5 r) const;
	void _drawcross(touchgfx::Canvas &canvas) const;
	void _drawconstell(const touchgfx::Rect &invalidatedArea) const;
	void _drawline(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
			const touchgfx::Rect &invalid, touchgfx::colortype color,
			uint8_t *fb) const;

};

#endif /* TOUCHGFX_GUI_WIDGETS_STARMAPWIDGET_H_ */
