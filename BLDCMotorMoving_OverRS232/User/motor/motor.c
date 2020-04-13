
#include "motor.h"

void Motor_Init(void)
{
	SineArray_Init();
	MotorPWM_Init();
	MotorPos_Init();
}

void SineArray_Init(void)
{
	uint16_t cntL = 0;
	sineArraySize = 1000 / SYSTEM_PERIOD / (MOTOR_BASIC_SPEED/360.0);//360.0 avoid divisor become zero
	phaseShift = sineArraySize / 3;	

	mPitch.stepA = 0;
	mPitch.stepB = mPitch.stepA + phaseShift;
	mPitch.stepC = mPitch.stepB + phaseShift;
	mRoll.stepA = 0;
	mRoll.stepB = mRoll.stepA + phaseShift;
	mRoll.stepC = mRoll.stepB + phaseShift;
	mYaw.stepA = 0;
	mYaw.stepB = mYaw.stepA + phaseShift;
	mYaw.stepC = mYaw.stepB + phaseShift;

//	U1RS232_PRINT("%d %d %d %d %d\r\n", sineArraySize, phaseShift, mPitch.stepA, mPitch.stepB, mPitch.stepC);
	
	for(cntL = 0; cntL < sineArraySize; cntL++)
	{
//		pwmSin[cntL] = (uint16_t)( ( sin( (cntL+1)*1.0/sineArraySize*2*MATH_PI )+1 )*TIM_PERIOD/2 );
		pwmSin[cntL] = (uint16_t)( ( sin( (cntL+1)*1.0/sineArraySize*2*MATH_PI )+1 )*(TIM_PERIOD - 50)/2 ) + 25;
//		pwmSin[cntL] = (uint16_t)( ( sin((cntL+1)*1.0/sineArraySize*2*MATH_PI))*(TIM_PERIOD)/2 );// + 70;
		
//		U1RS232_PRINT("[%d]%d\r\n", cntL, pwmSin[cntL]);
//		U1RS232_PRINT("%d,", pwmSin[cntL]);
	}
}

void MotorPWM_Init(void)
{

	GPIO_InitTypeDef gpioInit;
	TIM_TimeBaseInitTypeDef timeBaseInit;
	TIM_OCInitTypeDef ocInit;
	
	/** Motors Pin Definition.
	*********************************************************
		Motor0 Pitch
						A0 - PB1 -> TIM3_CH4
						B0 - PB0 -> TIM3_CH3
						C0 - PA7 -> TIM3_CH2
	*/
	/********************************************************
		Motor1 Roll		
						A1 - PA6 -> TIM3_CH1
						B1 - PA3 -> TIM2_CH4
						C1 - PA2 -> TIM2_CH3
	*/
	/********************************************************
		Motor2 Yaw		
						A2 - PB9 -> TIM4_CH4
						B2 - PA1 -> TIM2_CH2
						C2 - PB8 -> TIM4_CH3
	*********************************************************/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4, ENABLE);

	gpioInit.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	//pitch-gpio
	gpioInit.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &gpioInit);
	gpioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &gpioInit);
	//roll-gpio
	gpioInit.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6;
	GPIO_Init(GPIOA, &gpioInit);
	//yaw-gpio
	gpioInit.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &gpioInit);
	gpioInit.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &gpioInit);

	//timer-timebase
	timeBaseInit.TIM_Prescaler = TIM_PRESCALER;
	timeBaseInit.TIM_Period = TIM_PERIOD;
	
	timeBaseInit.TIM_CounterMode = TIM_CounterMode_CenterAligned3;
//	timeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
	
	timeBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
	timeBaseInit.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM3, &timeBaseInit);
	TIM_TimeBaseInit(TIM2, &timeBaseInit);
	TIM_TimeBaseInit(TIM4, &timeBaseInit);

	ocInit.TIM_OCMode = TIM_OCMode_PWM1;
	ocInit.TIM_OutputState = TIM_OutputState_Enable;
	//ocInit.TIM_OutputNState = TIM_OutputNState_Disable;
	ocInit.TIM_OCPolarity = TIM_OCPolarity_High;
	//ocInit.TIM_OCNPolarity = TIM_OCNPolarity_High;
	ocInit.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//ocInit.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	ocInit.TIM_Pulse = 0;
	
	//pitch-timer-ouputchannel
	TIM_OC2Init(TIM3, &ocInit);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM3, &ocInit);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM3, &ocInit);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//roll-timer-ouputchannel
	TIM_OC1Init(TIM3, &ocInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2, &ocInit);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM2, &ocInit);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	//yaw-timer-ouputchannel
	TIM_OC2Init(TIM2, &ocInit);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM4, &ocInit);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM4, &ocInit);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void MotorPos_Init(void)
{
	//Motor0 Pitch
	Motor0_SetPWM();
	//Motor1 Roll
	Motor1_SetPWM();
	//Motor2 Yaw
	Motor2_SetPWM();
}

void RunX_PitchMotor0(mdir_t mdir, uint16_t speed)		//speed unit: °/s
{
	static uint16_t preSpeed = 0;
	static uint16_t timeout = 0;
	static uint16_t cntTime = 0;
	static uint16_t growthFactor = 0;
/**
	if(speed <= MOTOR_RUN_LIMIT)
	{
		speed = 0;
	}
*/
	if(speed == 0)
	{
		//keep
	}
	else
	{
		if(speed != preSpeed)
		{
			if(speed < MOTOR_BASIC_SPEED)
			{
				timeout = MOTOR_BASIC_SPEED / speed - 1;	//faster speed shorter timeout
				growthFactor = 1;
//				U1RS232_PRINT("timeout:[%d]\r\n", timeout);
			}
			else
			{
				timeout = 0;
				growthFactor = speed / MOTOR_BASIC_SPEED;
//				U1RS232_PRINT("growthFactor:[%d]\r\n", growthFactor);
			}
			preSpeed = speed;
		}
		
		if(cntTime < timeout)
		{
//			U1RS232_PRINT("cntTime:[%d],  timeout: %d,   speed: %d\r\n", cntTime, timeout, speed);
			cntTime += 1;
		}
		else
		{
			cntTime = 0;	
			//TIM_Cmd(TIM3, DISABLE);
			//Motor0_Disable();
			Motor0_SetPWM();
			//Motor0_Enable();
			//TIM_Cmd(TIM3, ENABLE);
		
			//uint16_t has not negative so add sineArraySize
			mPitch.stepA = (mPitch.stepA + sineArraySize + mdir_cal_factor[mdir]*growthFactor) % sineArraySize;
			mPitch.stepB = (mPitch.stepB + sineArraySize + mdir_cal_factor[mdir]*growthFactor) % sineArraySize;
			mPitch.stepC = (mPitch.stepC + sineArraySize + mdir_cal_factor[mdir]*growthFactor) % sineArraySize;
			//U1RS232_PRINT("%d %d %d\r\n", pwmSin[mPitch.stepA], pwmSin[mPitch.stepB], pwmSin[mPitch.stepC]);
		}
	}
}

void Motor0_SetPWM(void)
{
	TIM_SetCompare4(TIM3, pwmSin[mPitch.stepA]);
	TIM_SetCompare3(TIM3, pwmSin[mPitch.stepB]);
	TIM_SetCompare2(TIM3, pwmSin[mPitch.stepC]);
}

/**
void Motor0_Disable(void)
{
	TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Disable);
	TIM_CCxCmd(TIM3, TIM_Channel_3, TIM_CCx_Disable);
	TIM_CCxCmd(TIM3, TIM_Channel_4, TIM_CCx_Disable);
}

void Motor0_Enable(void)
{
	TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Enable);
	TIM_CCxCmd(TIM3, TIM_Channel_3, TIM_CCx_Enable);
	TIM_CCxCmd(TIM3, TIM_Channel_4, TIM_CCx_Enable);
}
*/

void Motor0_Shutdown(void)
{
	TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Disable);
	TIM_CCxCmd(TIM3, TIM_Channel_3, TIM_CCx_Disable);
	TIM_CCxCmd(TIM3, TIM_Channel_4, TIM_CCx_Disable);
}

void RunZ_RollMotor1(mdir_t mdir, uint16_t speed)		//speed unit: °/s
{
	static uint16_t preSpeed = 0;
	static uint16_t timeout = 0;
	static uint16_t cntTime = 0;
	static uint16_t growthFactor = 0;

/**
	if(speed <= MOTOR_RUN_LIMIT)
	{
		speed = 0;
	}
*/
	if(speed == 0)
	{
		//keep
	}
	else
	{
		if(speed != preSpeed)
		{
			if(speed < MOTOR_BASIC_SPEED)
			{
				timeout = MOTOR_BASIC_SPEED / speed - 1;	//faster speed shorter timeout
				growthFactor = 1;
			}
			else
			{
				timeout = 0;
				growthFactor = speed / MOTOR_BASIC_SPEED;
			}
			preSpeed = speed;
		}

		if(cntTime < timeout)
		{
			//U1RS232_PRINT("[%d]%d %d\r\n", cntTime, timeout, speed);
			cntTime += 1;
		}
		else
		{
			cntTime = 0;
			Motor1_SetPWM();
		
			//uint16_t has not negative so add sineArraySize
			mRoll.stepA = (mRoll.stepA + sineArraySize + mdir_cal_factor[mdir]*growthFactor) % sineArraySize;
			mRoll.stepB = (mRoll.stepB + sineArraySize + mdir_cal_factor[mdir]*growthFactor) % sineArraySize;
			mRoll.stepC = (mRoll.stepC + sineArraySize + mdir_cal_factor[mdir]*growthFactor) % sineArraySize;
			//U1RS232_PRINT("%d %d %d\r\n", pwmSin[mRoll.stepA], pwmSin[mRoll.stepB], pwmSin[mRoll.stepC]);
		}
	}
}

void Motor1_SetPWM(void)
{
	TIM_SetCompare1(TIM3, pwmSin[mRoll.stepA]);
	TIM_SetCompare4(TIM2, pwmSin[mRoll.stepB]);
	TIM_SetCompare3(TIM2, pwmSin[mRoll.stepC]);
}

void Motor1_Shutdown(void)
{
	TIM_CCxCmd(TIM2, TIM_Channel_3, TIM_CCx_Disable);
	TIM_CCxCmd(TIM2, TIM_Channel_4, TIM_CCx_Disable);
	TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Disable);
}

void RunY_YawMotor2(mdir_t mdir, uint16_t speed)		//speed unit: °/s
{
	static uint16_t preSpeed = 0;
	static uint16_t timeout = 0;
	static uint16_t cntTime = 0;
	static uint16_t growthFactor = 0;

/**
	if(speed <= MOTOR_RUN_LIMIT)
	{
		speed = 0;
	}
*/
	if(speed == 0)
	{
		//keep
	}
	else
	{
		if(speed != preSpeed)
		{
			if(speed < MOTOR_BASIC_SPEED)
			{
				timeout = MOTOR_BASIC_SPEED / speed - 1;	//faster speed shorter timeout
				growthFactor = 1;
			}
			else
			{
				timeout = 0;
				growthFactor = speed / MOTOR_BASIC_SPEED;
			}
			preSpeed = speed;
		}

		if(cntTime < timeout)
		{
			//U1RS232_PRINT("[%d]%d %d\r\n", cntTime, timeout, speed);
			cntTime += 1;
		}
		else
		{
			cntTime = 0;
			Motor2_SetPWM();
		
			//uint16_t has not negative so add sineArraySize
			mYaw.stepA = (mYaw.stepA + sineArraySize + mdir_cal_factor[mdir]*growthFactor) % sineArraySize;
			mYaw.stepB = (mYaw.stepB + sineArraySize + mdir_cal_factor[mdir]*growthFactor) % sineArraySize;
			mYaw.stepC = (mYaw.stepC + sineArraySize + mdir_cal_factor[mdir]*growthFactor) % sineArraySize;
			//U1RS232_PRINT("%d %d %d\r\n", pwmSin[mYaw.stepA], pwmSin[mYaw.stepB], pwmSin[mYaw.stepC]);
		}
	}
}

void Motor2_SetPWM(void)
{
	TIM_SetCompare4(TIM4, pwmSin[mYaw.stepA]);
	TIM_SetCompare2(TIM2, pwmSin[mYaw.stepB]);
	TIM_SetCompare3(TIM4, pwmSin[mYaw.stepC]);
}

void Motor2_Shutdown(void)
{
	TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Disable);
	TIM_CCxCmd(TIM2, TIM_Channel_2, TIM_CCx_Disable);
	TIM_CCxCmd(TIM4, TIM_Channel_4, TIM_CCx_Disable);
}

void Turn_PitchX_Motor0(uint8_t direction, uint8_t speed, uint8_t MStep)
{
	const uint8_t MaxSteps = 6;
	static uint8_t step = 0;
	uint16_t pulseWidth;
	
	pulseWidth = speed * TIM_PERIOD / 100;

//	Motor0_Shutdown();
	
	switch(step)
	{
		case 0:
						TIM_SetCompare4(TIM3, pulseWidth);
            TIM_SetCompare2(TIM3, TIM_PERIOD);
						TIM_CCxCmd(TIM3, TIM_Channel_4, TIM_CCx_Enable);   //A+
						TIM_CCxNCmd(TIM3, TIM_Channel_2, TIM_CCxN_Enable); //C-
						break;
		
		case 1:
						TIM_SetCompare3(TIM3, pulseWidth);
						TIM_SetCompare2(TIM3, TIM_PERIOD);
						TIM_CCxCmd(TIM3, TIM_Channel_3, TIM_CCx_Enable);   //B+
						TIM_CCxNCmd(TIM3, TIM_Channel_2, TIM_CCxN_Enable); //C-
						break;
		
		case 2:
						TIM_SetCompare3(TIM3, pulseWidth);
            TIM_SetCompare4(TIM3, TIM_PERIOD);
						TIM_CCxCmd(TIM3, TIM_Channel_3, TIM_CCx_Enable);   //B+
						TIM_CCxNCmd(TIM3, TIM_Channel_4, TIM_CCxN_Enable); //A-
						break;
		
		case 3:
						TIM_SetCompare2(TIM3, pulseWidth);
            TIM_SetCompare4(TIM3, TIM_PERIOD);
						TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Enable);   //C+
						TIM_CCxNCmd(TIM3, TIM_Channel_4, TIM_CCxN_Enable); //A-
						break;
		
		case 4:
						TIM_SetCompare2(TIM3, pulseWidth);
            TIM_SetCompare3(TIM3, TIM_PERIOD);
						TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Enable);   //C+
						TIM_CCxNCmd(TIM3, TIM_Channel_3, TIM_CCxN_Enable); //B-
						break;
		
		case 5:
						TIM_SetCompare4(TIM3, pulseWidth);
            TIM_SetCompare3(TIM3, TIM_PERIOD);
						TIM_CCxCmd(TIM3, TIM_Channel_4, TIM_CCx_Enable);   //A+
						TIM_CCxNCmd(TIM3, TIM_Channel_3, TIM_CCxN_Enable); //B-
						break;
		
		default:
						break;
	}
	
	if(direction)
	{
		step = (step+1) % MaxSteps;
	}
	else
	{
		if(0 == step)
		{
			step = MaxSteps;
		}
		step = (step-1) % MaxSteps;
	}
}

