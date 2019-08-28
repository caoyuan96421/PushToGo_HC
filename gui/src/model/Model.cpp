#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>


#ifndef SIMULATOR
#include "FreeRTOS.h"
#include "queue.h"
#include "JoyStick.h"
#include "Debug.h"

extern xQueueHandle data_updated;
static int timeout;

static int timer_js = 0;
static bool nudge_JS = false;

#endif

Model::Model() :
		modelListener(0) {
	StarCatalog::getInstance().constructTree(); // Init star catalog
	TelescopeBackend::initialize();
	SkyCulture::getInstance().init();
	king = false;
	red = false;
	brightness = 16; // TODO: read config
	follow = false;
	showConstellation = true;
	equatorial = true;
	fov = 5;
#ifndef SIMULATOR
	JoyStick_Init();
#endif
}


void Model::tick() {
	if (modelListener) {
		char x;
#ifndef SIMULATOR
		if (xQueuePeek(data_updated, &x, 0)) { // If new data is available
#endif
			modelListener->setTime(TelescopeBackend::getTime(), TelescopeBackend::getTimeZone());
			EquatorialCoordinates new_coords;
			MountCoordinates new_coords_mount;
			new_coords = TelescopeBackend::getEqCoords();
			new_coords_mount = TelescopeBackend::getMountCoords();

			modelListener->setCoords(new_coords, new_coords_mount);

#ifndef SIMULATOR
			timeout = 0;
			xQueueReceive(data_updated, &x, 0); // Remove it from the queue
		}
		else{ // No info
			timeout++;
			if (timeout > 20){ // No data in 20 ticks - comm error?
				modelListener->showMessage("Failed to communicate with controller. Check cable connections.");
			}
		}
#endif

#ifndef SIMULATOR
	// Read the physical joystick and let the listener know
	if (--timer_js < 0) {
		float x,y;
		JoyStick_Read(x,y);
		debug_if(1, "x=%f, y=%f\r\n", x, y);
		if (fabsf(x) >= 0.1 || fabsf(y) >= 0.1) {
			TelescopeBackend::handleNudge(x, y);
			nudge_JS = true;
		}
		else {
			// Release
			if (nudge_JS) {
				nudge_JS = false;
				TelescopeBackend::handleNudge(0, 0);
			}
		}
		modelListener->joystickMoved(x, y);
		timer_js = 5;
	}
#endif
	}
}
