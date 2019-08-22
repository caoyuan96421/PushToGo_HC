#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>


#ifndef SIMULATOR
#include "FreeRTOS.h"
#include "queue.h"

extern xQueueHandle data_updated;
static int timeout;

#endif

Model::Model() :
		modelListener(0) {
	StarCatalog::getInstance().constructTree(); // Init star catalog
	TelescopeBackend::initialize();
	SkyCulture::getInstance().init();
	king = false;
	red = false;
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

	}
}
