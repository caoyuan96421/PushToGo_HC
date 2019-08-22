/*
 * TelescopeBackend.cpp
 *
 *  Created on: 2018Äê4ÔÂ28ÈÕ
 *      Author: caoyuan9642
 */

#include <TelescopeBackend.h>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>

static EquatorialCoordinates eq_coord;
static MountCoordinates mc_coord;
static TelescopeBackend::mountstatus_t status;
static LocationCoordinates loc_coord;

#ifndef SIMULATOR
#include "Comm.h"
#include "Debug.h"
#include "stm32f4xx_hal_rtc.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_pwr.h"
#include "mbed_mktime.h"

#define TB_DEBUG 0

static int timezone;

// Timeout values for different commands
static const int TIMEOUT_IMMEDIATE = 500; // Commands that should immediately return
static xSemaphoreHandle mutex = xSemaphoreCreateMutex();
static xQueueHandle cmd_queue = xQueueCreate(64, sizeof(char*));
static RTC_HandleTypeDef rtc;
//static Thread backend_thread(osPriorityBelowNormal, OS_STACK_SIZE, NULL, "backend_thread");
//static Timer timer;

// Semaphore for signaling update of coords and time
// Taken -> good to read another
// Given -> good to update display
xQueueHandle data_updated = xQueueCreate(1, 1);

struct ListNode {
	const char *cmd;
	uint32_t time_deadline;
	ListNode *next;
	ListNode *prev;
	xSemaphoreHandle commandReturned;
	xQueueHandle msgqueue;
	int retval;
	ListNode();
	~ListNode();
};

ListNode::ListNode() :
		cmd(NULL), time_deadline(0), next(NULL), prev(NULL), commandReturned(
				xSemaphoreCreateBinary()), msgqueue(
				xQueueCreate(64, sizeof(char*))), retval(0x7FFFFFFF) {
}

ListNode::~ListNode() {
	vSemaphoreDelete(commandReturned);
	vQueueDelete(msgqueue);
}

static ListNode listhead;

static ListNode* commandStarted(const char *cmd, int timeout) {
	debug_if(TB_DEBUG, "commandStarted: %s\r\n", cmd);
	xSemaphoreTake(mutex, portMAX_DELAY);
	ListNode *p, *q;
	for (p = &listhead; p->next; p = p->next)
		;
	q = new ListNode;
	if (!q) {
		xSemaphoreGive(mutex);
		return NULL;
	}
	q->cmd = cmd;
	q->time_deadline = xTaskGetTickCount() + timeout;
	q->next = NULL;
	p->next = q;
	q->prev = p;
	xSemaphoreGive(mutex);
	return q;
}

static ListNode* findCommand(const char *cmd) {
	ListNode *p;
	for (p = listhead.next; p && strcmp(p->cmd, cmd) != 0; p = p->next)
		;
	return p;
}

static int addMessage(const char *cmd, char *str, int size) {
	xSemaphoreTake(mutex, portMAX_DELAY);
	ListNode *p = findCommand(cmd);
	if (!p) {
		xSemaphoreGive(mutex);
		return -1;
	}
	debug_if(TB_DEBUG, "addMessage: %s (%s)\r\n", str, p->cmd);
	taskENTER_CRITICAL();
	if (xQueueIsQueueFullFromISR(p->msgqueue)) {
		taskEXIT_CRITICAL();
		xSemaphoreGive(mutex);
		return -1;
	}
	taskEXIT_CRITICAL();
	char *msg = new char[size + 1];
	if (!msg) {
		xSemaphoreGive(mutex);
		return -1;
	}
	strncpy(msg, str, size);
	msg[size] = '\0'; // Ensure proper termination
	xQueueSend(p->msgqueue, &msg, portMAX_DELAY);
	xSemaphoreGive(mutex);
	return 0;
}

static void nodeDelete(ListNode *node) {
	if (node) {
		// Delete all messages
		xSemaphoreTake(mutex, portMAX_DELAY);
		debug_if(TB_DEBUG, "nodeDelete: %s\r\n", node->cmd);
		taskENTER_CRITICAL();
		while (!xQueueIsQueueEmptyFromISR(node->msgqueue)) {
			char *msg;
			xQueueReceiveFromISR(node->msgqueue, &msg, NULL);
			delete msg;
		}
		taskEXIT_CRITICAL();
		if (node->prev) {
			node->prev->next = node->next;
		}
		if (node->next) {
			node->next->prev = node->prev;
		}
		delete node;
		xSemaphoreGive(mutex);
	}
}

static bool rtc_enabled(void) {
	return ((RTC->ISR & RTC_ISR_INITS) == RTC_ISR_INITS);
}

static void rtc_init() {
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE; // Mandatory, otherwise the PLL is reconfigured!
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		error("Cannot initialize RTC with LSE\n");
	}

	__HAL_RCC_RTC_CLKPRESCALER(RCC_RTCCLKSOURCE_LSE);
	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
		error("PeriphClkInitStruct RTC failed with LSE\n");
	}

	__HAL_RCC_RTC_ENABLE();

	rtc.Instance = RTC;
	rtc.State = HAL_RTC_STATE_RESET;
	rtc.Init.HourFormat = RTC_HOURFORMAT_24;
	rtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	rtc.Init.AsynchPrediv = 127;
	rtc.Init.SynchPrediv = 32768U / (rtc.Init.AsynchPrediv + 1) - 1;
	// 1Hz clock get
	if (HAL_RTC_Init(&rtc) != HAL_OK) {
		error("RTC initialization failed");
	}
	if (HAL_RTCEx_EnableBypassShadow(&rtc) != HAL_OK) {
		error("EnableBypassShadow error");
	}
}

// Reader thread
static void read_thread(void*) {
	char buf;
	char linebuf[1024];
	char *p = linebuf;
	char *g = linebuf + sizeof(linebuf);
	char cmd[32];
	ListNode *node;
	while (true) {
		Comm::read(&buf, 1); // Read will block
		switch (buf) {
		case '\r':
			continue;
		case '\n':
			if (p < g) {
				*(p++) = '\0';
			} else {
				*(--p) = '\0';
			}

			debug_if(TB_DEBUG, "|%s|\r\n", linebuf);
			// Check if it is status return from the command
			if (isdigit(linebuf[0])) {
				int retval;
				sscanf(linebuf, "%d %s", &retval, cmd);

				// Send a signal to whoever waiting for the command to finish
				xSemaphoreTake(mutex, portMAX_DELAY);
				if ((node = findCommand(cmd)) != NULL) {
					node->retval = retval;
					xSemaphoreGive(node->commandReturned);
				}
				xSemaphoreGive(mutex);
			} else {
				char *saveptr;
				const char *delim = " ";
				strncpy(cmd, strtok_r(linebuf, delim, &saveptr), sizeof(cmd));
				cmd[sizeof(cmd) - 1] = '\0';

				addMessage(cmd, saveptr, strlen(saveptr));
			}

			p = linebuf; // Rewind
			break;
		default:
			if (p < g)
				*(p++) = buf;
			break;
		}
	}
}

// Writer thread
static void write_thread(void *params) {
	while(true){
		char *cmd;
		xQueueReceive(cmd_queue, &cmd, portMAX_DELAY);
		Comm::write(cmd, strlen(cmd));
		delete cmd;
	}
}

// Write command, ignore output
static void queryNoResponse(const char *command, const char *arg) {
	char buf[256];
	int len;

// Make command
	if (!arg)
		len = snprintf(buf, sizeof(buf), "%s\r\n", command);
	else
		len = snprintf(buf, sizeof(buf), "%s %s\r\n", command, arg);

	buf[len-1] = '\0'; // Ensure termination

	debug_if(TB_DEBUG, "commandNoReturn: %s\r\n", buf);

	char *cmd = new char[len+1];
	if (!cmd){
		return;
	}
	strcpy(cmd, buf);
	xQueueSend(cmd_queue, &cmd, portMAX_DELAY);
}

static ListNode* queryStart(const char *command, const char *arg, int timeout) {
	char buf[256];
	int len;

	if (!arg)
		len = snprintf(buf, sizeof(buf), "%s\r\n", command);
	else {
		len = snprintf(buf, sizeof(buf), "%s %s\r\n", command, arg);
	}
	buf[len-1] = '\0'; // Ensure termination

	debug_if(TB_DEBUG, "commandSend: %s\r\n", buf);

	ListNode *cmd_node = commandStarted(command, timeout);

// Write command
	char *cmd = new char[len+1];
	if (!cmd){
		nodeDelete(cmd_node);
		return NULL;
	}
	strcpy(cmd, buf);
	xQueueSend(cmd_queue, &cmd, portMAX_DELAY);

	return cmd_node;
}

static bool queryHasReturned(ListNode *p) {
	return p->retval != 0x7FFFFFFF;
}

static int queryMessage(ListNode *p, char *buf, int size, int timeout) {
	if (!p || queryHasReturned(p)) {
		return -1;
	}
	char *msg;
	if (!xQueueReceive(p->msgqueue, &msg, timeout)) {
		return -2;
	}
	strncpy(buf, msg, size);
	delete msg;
	return 0;
}

static int queryWaitForReturn(ListNode *p, int timeout) {
	if (!p) {
		return -1;
	}
	if (!xSemaphoreTake(p->commandReturned, timeout)) {
		return -2;
	}
	int ret = p->retval;
	if (ret && TB_DEBUG) {
		debug_if(TB_DEBUG, "ret=%d", ret);
	}
	return ret;
}

static void queryFinish(ListNode *p) {
	nodeDelete(p);
}

static int _getEqCoords() {
	char buf[64];
	double ra, dec;
	ListNode *node = queryStart("read", NULL, TIMEOUT_IMMEDIATE);
	if (!node) {
		return -1;
	}
	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);

	if (sscanf(buf, "%lf%lf", &ra, &dec) != 2) {
		goto failed;
	}
	debug_if(TB_DEBUG, "RA=%lf, DEC=%lf\r\n", ra, dec);
	eq_coord = EquatorialCoordinates(dec, ra);
	return 0;

	failed: debug_if(TB_DEBUG, "Failed to read coordinates.\r\n");
	queryFinish(node);
	return -1;
}

static int _getMountCoords() {
	char buf[64];
	double ra, dec;
	ListNode *node = queryStart("read", "mount", TIMEOUT_IMMEDIATE);
	if (!node) {
		return -1;
	}
	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);

	if (sscanf(buf, "%lf%lf", &ra, &dec) != 2) {
		goto failed;
	}
	debug_if(TB_DEBUG, "RA=%f, DEC=%f\r\n", ra, dec);
	mc_coord = MountCoordinates(dec, ra);
	return 0;

	failed: debug_if(TB_DEBUG, "Failed to read coordinates.\r\n");
	queryFinish(node);
	return -1;
}

static TelescopeBackend::mountstatus_t _getStatus() {

	char buf[32];
	ListNode *node = queryStart("status", NULL, TIMEOUT_IMMEDIATE);
	if (!node) {
		return TelescopeBackend::UNDEFINED;
	}
	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);

	debug_if(TB_DEBUG, "state: %s\r\n", buf);

	if (strcmp(buf, "stopped") == 0)
		return TelescopeBackend::MOUNT_STOPPED;
	else if (strcmp(buf, "slewing") == 0)
		return TelescopeBackend::MOUNT_SLEWING;
	else if (strcmp(buf, "tracking") == 0)
		return TelescopeBackend::MOUNT_TRACKING;
	else if (strcmp(buf, "nudging") == 0)
		return TelescopeBackend::MOUNT_NUDGING;
	else if (strcmp(buf, "nudging_tracking") == 0)
		return TelescopeBackend::MOUNT_NUDGING_TRACKING;
	else if (strcmp(buf, "tracking_guiding") == 0)
		return (TelescopeBackend::mountstatus_t) (TelescopeBackend::MOUNT_TRACKING
				| TelescopeBackend::MOUNT_GUIDING);
	else
		return TelescopeBackend::UNDEFINED;

	failed: debug_if(TB_DEBUG, "Failed to read status.\r\n");
	queryFinish(node);
	return TelescopeBackend::UNDEFINED;
}

// Periodically update mount position through polling
static void poll_thread(void *params) {
	int timesync = 0;
	while (true) {
		_getEqCoords();
		_getMountCoords();
		status = _getStatus();
		if (timesync-- == 0) {
			TelescopeBackend::syncTime();
			timezone = TelescopeBackend::getConfigInt("timezone");
			loc_coord = LocationCoordinates(
					TelescopeBackend::getConfigDouble("latitude"),
					TelescopeBackend::getConfigDouble("longitude"));
			timesync = 100;
		}
		char dummy = 'a';
		xQueueSend(data_updated, &dummy, portMAX_DELAY); // Wait until consumer to take away the data before starting next one
	}
}

void TelescopeBackend::initialize() {
	Comm::init();
	xTaskCreate(read_thread, (TASKCREATE_NAME_TYPE)"rx_thread", 1024,
			NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(write_thread, (TASKCREATE_NAME_TYPE)"tx_thread", 1024,
			NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(poll_thread, (TASKCREATE_NAME_TYPE)"poll_thread", 1024, NULL,
			tskIDLE_PRIORITY + 2, NULL);
	rtc_init();
	if (!rtc_enabled()) {
		setTime(1564788744);
	}
}

static int millisec_offset = 0;

double TelescopeBackend::getTimeHR() {
	struct tm timeinfo;

	/* Since the shadow registers are bypassed we have to read the time twice and compare them until both times are the same */
	uint32_t Read_time = RTC->TR & RTC_TR_RESERVED_MASK;
	uint32_t Read_date = RTC->DR & RTC_DR_RESERVED_MASK;
	int32_t Read_subsec = RTC->SSR;

	while ((Read_time != (RTC->TR & RTC_TR_RESERVED_MASK))
			|| (Read_date != (RTC->DR & RTC_DR_RESERVED_MASK))
			|| (Read_subsec != RTC->SSR)
			) {
		Read_time = RTC->TR & RTC_TR_RESERVED_MASK;
		Read_date = RTC->DR & RTC_DR_RESERVED_MASK;
		Read_subsec = RTC->SSR;
	}

	/* Setup a tm structure based on the RTC
	 struct tm :
	 tm_sec      seconds after the minute 0-61
	 tm_min      minutes after the hour 0-59
	 tm_hour     hours since midnight 0-23
	 tm_mday     day of the month 1-31
	 tm_mon      months since January 0-11
	 tm_year     years since 1900
	 tm_yday     information is ignored by _rtc_maketime
	 tm_wday     information is ignored by _rtc_maketime
	 tm_isdst    information is ignored by _rtc_maketime
	 */
	timeinfo.tm_mday = RTC_Bcd2ToByte(
			(uint8_t) (Read_date & (RTC_DR_DT | RTC_DR_DU)));
	timeinfo.tm_mon = RTC_Bcd2ToByte(
			(uint8_t) ((Read_date & (RTC_DR_MT | RTC_DR_MU)) >> 8)) - 1;
	timeinfo.tm_year = RTC_Bcd2ToByte(
			(uint8_t) ((Read_date & (RTC_DR_YT | RTC_DR_YU)) >> 16)) + 68;
	timeinfo.tm_hour = RTC_Bcd2ToByte(
			(uint8_t) ((Read_time & (RTC_TR_HT | RTC_TR_HU)) >> 16));
	timeinfo.tm_min = RTC_Bcd2ToByte(
			(uint8_t) ((Read_time & (RTC_TR_MNT | RTC_TR_MNU)) >> 8));
	timeinfo.tm_sec = RTC_Bcd2ToByte(
			(uint8_t) ((Read_time & (RTC_TR_ST | RTC_TR_SU)) >> 0));

	// Convert to timestamp
	time_t t;
	if (_rtc_maketime(&timeinfo, &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
		return 0;
	}

	int32_t PREDIV_S = (RTC->PRER & RTC_PRER_PREDIV_S);
	int millisec = (PREDIV_S - Read_subsec) * 1000 / (PREDIV_S + 1);

	// Correct t if in middle of shifting
//	return (Read_subsec > PREDIV_S) ? t : t - 1;
//	return (millisec >= millisec_offset) ? t : t - 1;
	return t + (millisec - millisec_offset) / 1000.0;
}

time_t TelescopeBackend::getTime(){
	return (time_t) getTimeHR();
}

void TelescopeBackend::setTime(time_t t, int ms) {
	RTC_DateTypeDef dateStruct = { 0 };
	RTC_TimeTypeDef timeStruct = { 0 };

	taskENTER_CRITICAL();
	rtc.Instance = RTC;

	int32_t PREDIV_S = (RTC->PRER & RTC_PRER_PREDIV_S);
	int millisec = (PREDIV_S - RTC->SSR) * 1000 / (PREDIV_S + 1);

	millisec_offset = millisec - ms; // How much we're faster than the actual time
	if (millisec_offset < 0){
		millisec_offset += 1000;
		t++;
	}

	// Convert the time into a tm
	struct tm timeinfo;
	if (_rtc_localtime(t, &timeinfo, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
		return;
	}

	// Fill RTC structures
	if (timeinfo.tm_wday == 0) { /* Sunday specific case */
		dateStruct.WeekDay = RTC_WEEKDAY_SUNDAY;
	} else {
		dateStruct.WeekDay = timeinfo.tm_wday;
	}
	dateStruct.Month = timeinfo.tm_mon + 1;
	dateStruct.Date = timeinfo.tm_mday;
	dateStruct.Year = timeinfo.tm_year - 68;
	timeStruct.Hours = timeinfo.tm_hour;
	timeStruct.Minutes = timeinfo.tm_min;
	timeStruct.Seconds = timeinfo.tm_sec;

	timeStruct.TimeFormat = RTC_HOURFORMAT_24;
	timeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	timeStruct.StoreOperation = RTC_STOREOPERATION_RESET;

	// Change the RTC current date/time
	if (HAL_RTC_SetDate(&rtc, &dateStruct, RTC_FORMAT_BIN) != HAL_OK) {
		error("HAL_RTC_SetDate error\n");
	}
	if (HAL_RTC_SetTime(&rtc, &timeStruct, RTC_FORMAT_BIN) != HAL_OK) {
		error("HAL_RTC_SetTime error\n");
	}

	taskEXIT_CRITICAL();
}

int TelescopeBackend::syncTime() {
	char buf[32];
	double timestamp;
	int integral;
	ListNode *node = queryStart("time", "hr", TIMEOUT_IMMEDIATE);
	if (!node) {
		return -1;
	}
	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);
	timestamp = strtod(buf, NULL);
	debug_if(TB_DEBUG, "Setting time to %d\r\n", timestamp);

	integral = floor(timestamp);
	setTime(integral, floor((timestamp - integral) * 1000));
	return 0;

	failed: debug_if(TB_DEBUG, "Failed to sync time.\r\n");
	queryFinish(node);
	return -1;
}

EquatorialCoordinates TelescopeBackend::getEqCoords() {
	return eq_coord;
}

MountCoordinates TelescopeBackend::getMountCoords() {
	return mc_coord;
}

int TelescopeBackend::getTimeZone() {
	return timezone;
}

LocationCoordinates TelescopeBackend::getLocation() {
	return loc_coord;
}

int TelescopeBackend::startNudge(Direction dir) {
	const char *dir_str;
	switch (dir) {
	case EAST:
		dir_str = "east";
		break;
	case WEST:
		dir_str = "west";
		break;
	case SOUTH:
		dir_str = "south";
		break;
	case NORTH:
		dir_str = "north";
		break;
	case NORTHEAST:
		dir_str = "north east";
		break;
	case NORTHWEST:
		dir_str = "north west";
		break;
	case SOUTHEAST:
		dir_str = "south east";
		break;
	case SOUTHWEST:
		dir_str = "south west";
		break;
	default:
		return 0;
	}
	debug_if(TB_DEBUG, "NUDGE %s\r\n", dir_str);
	queryNoResponse("nudge", dir_str);
	return 0;
}

int TelescopeBackend::stopNudge() {
	queryNoResponse("nudge", "stop");
	return 0;
}

void TelescopeBackend::emergencyStop() {
	queryNoResponse("estop", NULL);
}

int TelescopeBackend::getConfigString(const char *config, char *buf, int size) {
	ListNode *node = queryStart("config", config, TIMEOUT_IMMEDIATE);
	if (!node) {
		return -1;
	}
	if (queryMessage(node, buf, size, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);

	return 0;

	failed: debug_if(TB_DEBUG, "Failed to read config.%s.\r\n", config);
	queryFinish(node);
	return -1;
}

int TelescopeBackend::getConfigInt(const char *config) {
	char buf[64];
	getConfigString(config, buf, sizeof(buf));
	return strtol(buf, NULL, 10);
}

double TelescopeBackend::getConfigDouble(const char *config) {
	char buf[64];
	getConfigString(config, buf, sizeof(buf));
	return strtod(buf, NULL);
}

bool TelescopeBackend::getConfigBool(const char *config) {
	char buf[64];
	getConfigString(config, buf, sizeof(buf));
	return (strcmp(config, "true") == 0);
}

int TelescopeBackend::track(bool on) {
	if (on)
		queryNoResponse("track", NULL);
	else
		queryNoResponse("stop", "track");
	return 0;
}

TelescopeBackend::mountstatus_t TelescopeBackend::getStatus() {
	return status;
}

static void setDataValue(DataType type, char *str, DataValue &value) {
	switch (type) {
	case DATATYPE_STRING:
		strncpy(value.strdata, str, sizeof(value.strdata));
		return;
	case DATATYPE_INT:
		value.idata = strtol(str, NULL, 10);
		return;
	case DATATYPE_DOUBLE:
		value.ddata = strtod(str, NULL);
		return;
	case DATATYPE_BOOL:
		value.bdata = (strcmp(str, "true") == 0);
		return;
	}
}

int TelescopeBackend::getConfigAll(ConfigItem *configs, int maxConfig) {
	char buf[1024];
	int nConfig = 0;
	char *saveptr;
	const char *delim = " ";
	char *str = buf;

// Read all configs
	ListNode *node = queryStart("config", NULL, TIMEOUT_IMMEDIATE);
	if (!node) {
		return 0;
	}
	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);

	for (nConfig = 0; nConfig < maxConfig; nConfig++) {
		char *config = strtok_r(str, delim, &saveptr);
		if (config == NULL) {
			// End of string
			break;
		}
		configs[nConfig].setConfig(config);
		str = NULL;
	}

// Read name, type and help
	char query[64];
	delim = ",";
	for (int i = 0; i < nConfig; i++) {
		snprintf(query, sizeof(query), "%s info", configs[i].config);
		node = queryStart("config", query, TIMEOUT_IMMEDIATE);
		if (!node) {
			return 0;
		}
		if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
			goto failed;
		}
		if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
			goto failed;
		}
		queryFinish(node);
		// Get type
		char *type = strtok_r(buf, delim, &saveptr);
		if (strcmp(type, "INT") == 0) {
			configs[i].type = DATATYPE_INT;
		} else if (strcmp(type, "DOUBLE") == 0) {
			configs[i].type = DATATYPE_DOUBLE;
		} else if (strcmp(type, "BOOL") == 0) {
			configs[i].type = DATATYPE_BOOL;
		} else if (strcmp(type, "STRING") == 0) {
			configs[i].type = DATATYPE_STRING;
		} else {
			goto failed;
		}
		// Get value
		char *value = strtok_r(NULL, delim, &saveptr);
		setDataValue(configs[i].type, value, configs[i].value);
		// Get name
		char *name = strtok_r(NULL, delim, &saveptr);
		configs[i].setName(name);
		// Get help from the rest
		configs[i].setHelp(saveptr);

		// TODO get limits
	}

	return nConfig;

	failed: debug_if(TB_DEBUG, "Failed to read config.\r\n");
	queryFinish(node);
	return nConfig;
}

int TelescopeBackend::goTo(EquatorialCoordinates eq) {
	char buf[64];
	snprintf(buf, sizeof(buf), "%.8f %.8f", eq.ra, eq.dec);
	queryNoResponse("goto", buf);
	return 0;
}

int TelescopeBackend::goToMount(MountCoordinates mc) {
	char buf[64];
	snprintf(buf, sizeof(buf), "mount %.8f %.8f", mc.ra_delta, mc.dec_delta);
	queryNoResponse("goto", buf);
	return 0;
}

int TelescopeBackend::getCalibration(EqCalibration &calib) {
	char buf[128];
	ListNode *node = queryStart("align", "show", TIMEOUT_IMMEDIATE);
	if (!node) {
		return -1;
	}

	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
// Read offset
	sscanf(buf, "%*s%lf%lf", &calib.offset.ra_off, &calib.offset.dec_off);

	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
// Read PA
	sscanf(buf, "%*s%lf%lf", &calib.pa.alt, &calib.pa.azi);

	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
// Read PA
	sscanf(buf, "%*s%lf", &calib.cone);

	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
// Read error
	sscanf(buf, "%*s%lf", &calib.error);

	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);
	return 0;

	failed: debug_if(TB_DEBUG, "Failed to get alignment.\r\n");
	queryFinish(node);
	return -1;
}

int TelescopeBackend::setCalibration(const EqCalibration &calib) {
	char buf[128];
	snprintf(buf, sizeof(buf), "set %.8f %.8f %.8f %.8f %.8f",
			calib.offset.ra_off, calib.offset.dec_off, calib.pa.alt,
			calib.pa.azi, calib.cone);
	ListNode *node = queryStart("align", buf, TIMEOUT_IMMEDIATE);
	if (!node) {
		return UNDEFINED;
	}
	int ret = queryWaitForReturn(node, TIMEOUT_IMMEDIATE);
	queryFinish(node);
	return ret;
}

int TelescopeBackend::addAlignmentStar(const AlignmentStar &star) {
	char buf[128];
	snprintf(buf, sizeof(buf), "add %.8f %.8f %.8f %.8f", star.star_ref.ra,
			star.star_ref.dec, star.star_meas.ra_delta,
			star.star_meas.dec_delta);
	ListNode *node = queryStart("align", buf, TIMEOUT_IMMEDIATE);
	if (!node) {
		return UNDEFINED;
	}
	int ret = queryWaitForReturn(node, TIMEOUT_IMMEDIATE);
	queryFinish(node);
	return ret;
}

void TelescopeBackend::removeAlignmentStar(int index) {
	char buf[128];
	snprintf(buf, sizeof(buf), "delete %d", index);
	ListNode *node = queryStart("align", buf, TIMEOUT_IMMEDIATE);
	if (!node) {
		return;
	}
	queryWaitForReturn(node, TIMEOUT_IMMEDIATE);
	queryFinish(node);
}

int TelescopeBackend::replaceAlignmentStar(int index,
		const AlignmentStar &star) {
	char buf[128];
	snprintf(buf, sizeof(buf), "replace %d %.8f %.8f %.8f %.8f", index,
			star.star_ref.ra, star.star_ref.dec, star.star_meas.ra_delta,
			star.star_meas.dec_delta);
	ListNode *node = queryStart("align", buf, TIMEOUT_IMMEDIATE);
	if (!node) {
		return UNDEFINED;
	}
	int ret = queryWaitForReturn(node, TIMEOUT_IMMEDIATE);
	queryFinish(node);
	return ret;
}

void TelescopeBackend::clearAlignment() {
	queryNoResponse("align", "clear");
}

void TelescopeBackend::forceAlignment() {
	queryNoResponse("align", "force");
}

void TelescopeBackend::updateAlignment() {
	queryNoResponse("align", "update");
}

int TelescopeBackend::addAlignmentStar(const EquatorialCoordinates &star_ref) {
	char buf[128];
	snprintf(buf, sizeof(buf), "add %.8f %.8f", star_ref.ra, star_ref.dec);
	ListNode *node = queryStart("align", buf, TIMEOUT_IMMEDIATE);
	if (!node) {
		return -1;
	}
	int ret = queryWaitForReturn(node, TIMEOUT_IMMEDIATE);
	queryFinish(node);
	return ret;
}

int TelescopeBackend::replaceAlignmentStar(int index,
		const EquatorialCoordinates &star_ref) {
	char buf[128];
	snprintf(buf, sizeof(buf), "replace %d  %.8f %.8f", index, star_ref.ra,
			star_ref.dec);
	ListNode *node = queryStart("align", buf, TIMEOUT_IMMEDIATE);
	if (!node) {
		return -1;
	}
	int ret = queryWaitForReturn(node, TIMEOUT_IMMEDIATE);
	queryFinish(node);
	return ret;
}

EquatorialCoordinates TelescopeBackend::convertMountToEquatorial(
		const MountCoordinates &mc) {
	char buf[128];
	snprintf(buf, sizeof(buf), "convert mount %.8f %.8f", mc.ra_delta,
			mc.dec_delta);
	ListNode *node = queryStart("align", buf, TIMEOUT_IMMEDIATE);
	if (!node) {
		return EquatorialCoordinates(0, 0);
	}
	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);

	double ra, dec;
	sscanf(buf, "%lf%lf", &ra, &dec);

	return EquatorialCoordinates(dec, ra);

	failed: debug_if(TB_DEBUG, "Failed to convert coordinates.\r\n");
	queryFinish(node);
	return EquatorialCoordinates(0, 0);
}

MountCoordinates TelescopeBackend::convertEquatorialToMount(
		const EquatorialCoordinates &eq) {
	char buf[128];
	snprintf(buf, sizeof(buf), "convert eq %.8f %.8f", eq.ra, eq.dec);
	ListNode *node = queryStart("align", buf, TIMEOUT_IMMEDIATE);
	if (!node) {
		return MountCoordinates(0, 0);
	}
	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);

	double ra, dec;
	sscanf(buf, "%lf%lf", &ra, &dec);

	return MountCoordinates(dec, ra);

	failed: debug_if(TB_DEBUG, "Failed to convert coordinates.\r\n");
	queryFinish(node);
	return MountCoordinates(0, 0);
}

int TelescopeBackend::getNumAlignmentStars() {
	char buf[16];
	ListNode *node = queryStart("align", "show num", TIMEOUT_IMMEDIATE);
	if (!node) {
		return 0;
	}
	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);

	return strtol(buf, NULL, 10);

	failed: debug_if(TB_DEBUG, "Failed to get number of alignment stars.\r\n");
	queryFinish(node);
	return 0;
}

int TelescopeBackend::getAlignmentStar(int index, AlignmentStar &star) {
	char buf[128];
	snprintf(buf, sizeof(buf), "show %d", index);
	ListNode *node = queryStart("align", buf, TIMEOUT_IMMEDIATE);
	if (!node) {
		return -1;
	}

	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}

	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);

// Alignment star
	double ref_ra, ref_dec, meas_ra, meas_dec;
	time_t timestamp;
	sscanf(buf, "%lf%lf%lf%lf%d", &ref_ra, &ref_dec, &meas_ra, &meas_dec,
			(int*) &timestamp);
	star.star_ref.ra = ref_ra;
	star.star_ref.dec = ref_dec;
	star.star_meas.ra_delta = meas_ra;
	star.star_meas.dec_delta = meas_dec;
	star.timestamp = timestamp;
	return 0;

	failed: debug_if(TB_DEBUG, "Failed to get alignment star %d.\r\n", index);
	queryFinish(node);
	return -1;
}

void TelescopeBackend::saveConfig() {
	ListNode *node = queryStart("save", NULL, TIMEOUT_IMMEDIATE);
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		debug_if(TB_DEBUG, "Failed to save config.\r\n");
	}
	queryFinish(node);
}

void TelescopeBackend::setSpeed(const char *type, double speed) {
	char buf[32];
	snprintf(buf, sizeof(buf), "%s %.8f", type, speed);
	ListNode *node = queryStart("speed", buf, TIMEOUT_IMMEDIATE);
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		debug_if(TB_DEBUG, "Failed to set speed to %f.\r\n", speed);
	}
	queryFinish(node);
}

double TelescopeBackend::getSpeed(const char *type) {
	char buf[32];
	ListNode *node = queryStart("speed", type, TIMEOUT_IMMEDIATE);
	if (!node) {
		return -1;
	}
	if (queryMessage(node, buf, sizeof(buf), TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		goto failed;
	}
	queryFinish(node);

	return strtod(buf, NULL);

	failed: debug_if(TB_DEBUG, "Failed to read speed %s.\r\n", type);
	queryFinish(node);
	return NAN;
}

void TelescopeBackend::writeConfig(ConfigItem *config) {
	if (!config)
		return;
	char buf[128];
	switch (config->type) {
	case DATATYPE_STRING:
		snprintf(buf, sizeof(buf), "%s %s", config->config,
				config->value.strdata);
		break;
	case DATATYPE_INT:
		snprintf(buf, sizeof(buf), "%s %d", config->config,
				config->value.idata);
		break;
	case DATATYPE_DOUBLE:
		snprintf(buf, sizeof(buf), "%s %.8f", config->config,
				config->value.ddata);
		break;
	case DATATYPE_BOOL:
		snprintf(buf, sizeof(buf), "%s %s", config->config,
				config->value.bdata ? "true" : "false");
		break;
	}
	ListNode *node = queryStart("config", buf, TIMEOUT_IMMEDIATE);
	if (queryWaitForReturn(node, TIMEOUT_IMMEDIATE) != 0) {
		debug_if(TB_DEBUG, "Failed to set: %s.\r\n", buf);
	}
	queryFinish(node);
}

void TelescopeBackend::handleNudge(float x, float y) {
	debug_if(TB_DEBUG, "nudge %f %f\r\n", x, y);
	static float savedspeed = -1; // For temporarily storing nudging speed
	if (x == 0 && y == 0) {
		TelescopeBackend::stopNudge();
		if (savedspeed != -1) {
			TelescopeBackend::setSpeed("slew", savedspeed); // Restore speed when nudging finishes
			savedspeed = -1;
		}
		return;
	}

	if ((x == 0 && fabsf(y) < 1) || (y == 0 && fabsf(x) < 1)) {
		// First stop nudging if it is already being nudged
//		if (TelescopeBackend::getStatus() & MOUNT_NUDGING)
//		{
//			TelescopeBackend::stopNudge();
//		}

		// Fractional slew speed
		if (savedspeed == -1) {
			savedspeed = TelescopeBackend::getSpeed("slew");
		}
		float newspeed = (fabsf(x) + fabsf(y)) * savedspeed; // New slewing speed
		TelescopeBackend::setSpeed("slew", newspeed);
	} else {
		// Simultaneous fast slew. Use max speed
		if (savedspeed != -1) {
			TelescopeBackend::setSpeed("slew", savedspeed); // Restore speed when nudging finishes
			savedspeed = -1;
		}
	}

// Determing ramping direction
	Direction d;
	if (x > 0)
		d = EAST;
	else if (x < 0)
		d = WEST;
	else
		d = NONE;

	if (y > 0)
		d = (Direction) (d | NORTH);
	else if (y < 0)
		d = (Direction) (d | SOUTH);

	TelescopeBackend::startNudge(d);
}

#else

void TelescopeBackend::initialize() {
}

void TelescopeBackend::setTime(time_t t, int ms) {

}
time_t TelescopeBackend::getTime() {
	return time(NULL);
}
int TelescopeBackend::syncTime() {
	return 0;
}
EquatorialCoordinates TelescopeBackend::getEqCoords() {
	return eq_coord;
}
MountCoordinates TelescopeBackend::getMountCoords() {
	return	eq_coord.toLocalEquatorialCoordinates(getTime(), loc_coord).toMountCoordinates(
					PIER_SIDE_AUTO);
}

int TelescopeBackend::getConfigAll(ConfigItem*, int) {
	return 0;
}
int TelescopeBackend::getConfigString(const char *config, char *buf, int size) {
	return 0;
}
int TelescopeBackend::getConfigInt(const char *config) {
	return 0;
}
double TelescopeBackend::getConfigDouble(const char *config) {
	return 0;
}
bool TelescopeBackend::getConfigBool(const char *config) {
	return false;
}
void TelescopeBackend::writeConfig(ConfigItem*) {

}
void TelescopeBackend::saveConfig() {

}

int TelescopeBackend::startNudge(Direction dir) {

}
int TelescopeBackend::stopNudge() {

}
void TelescopeBackend::emergencyStop() {

}

int TelescopeBackend::goTo(EquatorialCoordinates eq) {
	eq_coord = eq;
}
int TelescopeBackend::goToMount(MountCoordinates mc) {
	eq_coord = mc.toLocalEquatorialCoordinates().toEquatorial(getTime(),
			loc_coord);
}

int TelescopeBackend::getCalibration(EqCalibration &calib) {
	return 0;
}
int TelescopeBackend::setCalibration(const EqCalibration &calib) {
	return 0;
}
int TelescopeBackend::getNumAlignmentStars() {
	return 0;
}
int TelescopeBackend::getAlignmentStar(int index, AlignmentStar &star) {
	return 0;
}
int TelescopeBackend::addAlignmentStar(const AlignmentStar &star) {
	return 0;
}
int TelescopeBackend::addAlignmentStar(const EquatorialCoordinates &star_ref) {
	return 0;
}
void TelescopeBackend::removeAlignmentStar(int index) {
}
int TelescopeBackend::replaceAlignmentStar(int index,
		const AlignmentStar &star) {
	return 0;
}
int TelescopeBackend::replaceAlignmentStar(int index,
		const EquatorialCoordinates &star_ref) {
	return 0;
}
void TelescopeBackend::clearAlignment() {
}
void TelescopeBackend::forceAlignment() {
}
void TelescopeBackend::updateAlignment() {

}
EquatorialCoordinates TelescopeBackend::convertMountToEquatorial(
		const MountCoordinates &mc) {
	return mc.toLocalEquatorialCoordinates().toEquatorial(getTime(), loc_coord);
}
MountCoordinates TelescopeBackend::convertEquatorialToMount(
		const EquatorialCoordinates &eq) {
	return eq.toLocalEquatorialCoordinates(getTime(), loc_coord).toMountCoordinates(
			PIER_SIDE_AUTO);
}

int TelescopeBackend::track(bool on) {
	return 0;
}
TelescopeBackend::mountstatus_t TelescopeBackend::getStatus() {
	return MOUNT_STOPPED;
}

double TelescopeBackend::getSpeed(const char *type) {
	return 1;
}

void TelescopeBackend::setSpeed(const char *type, double speedSidereal) {

}

void TelescopeBackend::handleNudge(float x, float y) {

}

extern long int timezone;
int TelescopeBackend::getTimeZone(){
	return timezone;
}

LocationCoordinates TelescopeBackend::getLocation(){
	return loc_coord;
}


#endif
