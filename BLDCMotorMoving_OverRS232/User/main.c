
/*
* Test Firmware-03 for Henway Gimbal Project
* Created : 2020-01-19
* Modified : 2020-01-19
* Project Name : HGimbal_
* Version : Ver01
*/

////////////////////////////////////////////////////////////////////////////////////

#include "stm32f10x.h"
#include "global_math.h"
#include "led.h"
#include "systick.h"
#include "usart1.h"
#include "motor.h"
#include "pid.h"
#include "string.h"
#include "joystick.h"


#define ENABLE_PRINT

#define FLASH_SAVE_ADDR                  WRITE_START_ADDR

////#define FLASH_HUBPASSWORD_ADDR         FLASH_SAVE_ADDR + 100
////#define FLASH_SERVERURL_ADDR           FLASH_SAVE_ADDR + 200
////#define FLASH_SERVERPORT_ADDR          FLASH_SAVE_ADDR + 300
////#define FLASH_PINPASSWORD_ADDR         FLASH_SAVE_ADDR + 400
////#define FLASH_DEVICEVERSION_ADDR       FLASH_SAVE_ADDR + 500

#define MPU_SAMPLE_RATE		((uint16_t)(50))	//unit: Hz
#define MPU_DLPF_SWITCH		((uint8_t)(1))		//1:on 0:off
#define MPU_ACCEL_WEIGHT	((float)(0.05))
#define MPU_CAL_PERIOD		((float)(SYSTEM_PERIOD/1000.0))		//unit: s
	

/////////////////////////////////////////////////////////////////////////////

int main(void)
{
	u32 counter = 0;

	Led_Init();
	Systick_Init();	
		
#ifdef ENABLE_PRINT
	Usart1_Init(115200);	//conflict with RC0 and RC1
#endif
	
	Delay_ms(1);

	LED0_ON;
	LED1_ON;
	LED2_ON;
	
//	U1RS232_PRINT("Three Motor Initial:\r\n");
	
	LED0_OFF;
	LED1_OFF;
	LED2_OFF;

	Delay_ms(1);

	Motor_Init();
	
	Delay_ms(1);

	while(1)
	{
			counter++; LED2_TOGGLE;
		
			if(RecvCommandFlag)
			{
				U1RS232_PRINT("Receive Joystick Command\r\n");
				JoystickCommandRun();
				RecvCommandFlag = 0;
				LED1_OFF;
			}
//		  U1RS232_PRINT("[#1]Count Up : %d\r\n", counter);
			
//			RunX_PitchMotor0((mdir_t)0, 1800);
			
//			RunY_YawMotor2((mdir_t)0, 2100);
		
			Delay_ms(1);
	}
}

