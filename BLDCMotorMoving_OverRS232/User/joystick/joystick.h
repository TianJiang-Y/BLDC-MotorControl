
#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "stm32f10x.h"
#include "global_math.h"
#include "usart1.h"
#include "motor.h"

static uint8_t PicthMotor_Down = 1;
static uint8_t PicthMotor_Up = 0;

static uint8_t YawMotor_Right = 0;
static uint8_t YawMotor_Left = 1;

static uint8_t RollMotor_Clockwise = 0;
static uint8_t RollMotor_AntiClockwise = 1;

void JoystickCommandRun(void);

#endif

