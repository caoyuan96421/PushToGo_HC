/*
 * Joystick.h
 *
 *  Created on: Aug 28, 2019
 *      Author: caoyu
 */

#ifndef ASTRONOMY_SRC_JOYSTICK_H_
#define ASTRONOMY_SRC_JOYSTICK_H_

void JoyStick_Init();

void JoyStick_Read(float &x, float &y);

void JoyStick_Calibrate();

#endif /* ASTRONOMY_SRC_JOYSTICK_H_ */
