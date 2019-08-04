#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() :
		modelListener(0) {
	StarCatalog::getInstance().constructTree(); // Init star catalog
	TelescopeBackend::initialize();
	SkyCulture::getInstance().init();
	king = false;
	timezone = 0;
}

static int timeupdate = 0;

void Model::tick() {
	if (modelListener) {
		if (timeupdate-- == 0) {
			// Update time every 10 ticks
			modelListener->setTime(TelescopeBackend::getTime(), timezone);
			EquatorialCoordinates new_coords;
			MountCoordinates new_coords_mount;
			eq_coord = TelescopeBackend::getEqCoords();
			mount_coord = TelescopeBackend::getMountCoords();

			modelListener->setCoords(eq_coord, mount_coord);
			timeupdate = 10;
		}

	}
}

//time_t Model::getTime()
//{
//	return time(NULL);
//}

//EquatorialCoordinates Model::getEqCoords()
//{
//	return eq_coord;
//}
//
//MountCoordinates Model::getMountCoords()
//{
//	return mount_coord;
//}
//
//int Model::getTimeZone()
//{
//	return timezone;
//}
//
//LocationCoordinates Model::getLocation()
//{
//	return location;
//}

//TelescopeBackend::mountstatus_t Model::getStatus()
//{
//	return TelescopeBackend::getStatus();
//}
//
//void Model::track(bool on)
//{
//	TelescopeBackend::track(on);
//}
//
//int Model::getConfigString(const char* config, char* buf, int size)
//{
//	return TelescopeBackend::getConfigString(config, buf, size);
//}
//
//int Model::getConfigInt(const char* config)
//{
//	return TelescopeBackend::getConfigInt(config);
//}
//
//double Model::getConfigDouble(const char* config)
//{
//	return TelescopeBackend::getConfigDouble(config);
//}
//
//bool Model::getConfigBool(const char* config)
//{
//	return TelescopeBackend::getConfigBool(config);
//}
//
//double Model::getSpeed(const char *type)
//{
//	return TelescopeBackend::getSpeed(type);
//}
//
//void Model::setSpeed(const char *type, double speed)
//{
//	TelescopeBackend::setSpeed(type, speed);
//}
//
//int Model::getConfigAll(ConfigItem* configs, int maxConfig)
//{
//	return TelescopeBackend::getConfigAll(configs, maxConfig);
//}
//
//void Model::writeConfig(ConfigItem* config){
//	return TelescopeBackend::writeConfig(config);
//}
