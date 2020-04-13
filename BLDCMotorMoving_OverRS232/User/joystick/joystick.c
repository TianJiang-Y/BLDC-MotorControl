
#include "joystick.h"
#include "string.h"

#define POS_INIT_DIFF		    ((float)(1.0))		//unit: degree
#define POS_INTI_SPEED		  ((uint16_t)(60))	//unit: degree per second
#define JOYSTICK_COM_SPEED  ((uint16_t)(900))
#define POS_INIT_JUDGE		  ((uint16_t)(200))

uint8_t MaxStep = 9;

uint8_t* Command_Check(char *str)
{
	char *strx = 0;
	
	if(RecvCommandFlag)
	{
		usart1RxBuff[USART1_RX_LEN - 1] = 0;
		
		strx = strstr((const char*)usart1RxBuff,(const char*)str);	
	}
	
	return (uint8_t*)strx;
}

void JoystickCommandRun(void)
{
	uint16_t ip;
	uint16_t turns = 0;
	int16_t MotorStep = 0;
	uint8_t MotorDirection;
	uint16_t sineArraySize = 1200;
	
	uint16_t i = 0;
	
	if(Command_Check("$STATUS"))
	{
		U1RS232_PRINT("%%STATUS,Y,N,0,0\r\n");
	}
	
	else if(Command_Check("$CAM,On"))
	{
		U1RS232_PRINT("%%CAM,Ok\r\n");
	}
	
	else if(Command_Check("$CAM,Off"))
	{
		U1RS232_PRINT("%%CAM,Err\r\n");
	}
	
	else if(Command_Check("$IR,On"))
	{
		U1RS232_PRINT("%%IR,Ok\r\n");
	}
	
	else if(Command_Check("$IR,Off"))
	{
		U1RS232_PRINT("%%IR,Err\r\n");
	}
	
	else if(Command_Check("$HOME,Go"))
	{
		U1RS232_PRINT("%%HOME,Ok\r\n");
		U1RS232_PRINT("%%HOME,Done\r\n");
	}
	
	else if(Command_Check("$MOVE,X,"))
	{
		if(usart1RxBuff[8] == '-')
		{
			MotorDirection = PicthMotor_Down;
			MotorStep = usart1RxBuff[9] - 48;
			U1RS232_PRINT("%%Run-PitchMotor0 Moving to Down as Step %d\r\n", MotorStep);
		}
		else
		{
			MotorDirection = PicthMotor_Up;			
			MotorStep = usart1RxBuff[8] - 48;
			U1RS232_PRINT("%%Run-PitchMotor0 Moving to Up as Step %d\r\n", MotorStep);
		}
		
		turns = MotorStep * (sineArraySize / MaxStep);
		
		for(ip = 0; ip < turns; ip++)
		{
			RunX_PitchMotor0((mdir_t)MotorDirection, JOYSTICK_COM_SPEED);
			Delay_ms(2);
		}
		U1RS232_PRINT("Moved\r\n");
	}
	
	else if(Command_Check("$MOVE,Y,"))
	{
		if(usart1RxBuff[8] == '-')
		{
			MotorDirection = YawMotor_Left;
			MotorStep = usart1RxBuff[9] - 48;
			U1RS232_PRINT("%%Run-YawMotor2 Moving to Left as Step %d\r\n", MotorStep);
		}
		else
		{
			MotorDirection = YawMotor_Right;			
			MotorStep = usart1RxBuff[8] - 48;
			U1RS232_PRINT("%%Run-YawMotor2 Moving to Right as Step %d\r\n", MotorStep);
		}
		
		turns = MotorStep * (sineArraySize / MaxStep);
		
		for(ip = 0; ip < turns; ip++)
		{
			RunY_YawMotor2((mdir_t)MotorDirection, JOYSTICK_COM_SPEED);
			Delay_ms(2);
		}
		U1RS232_PRINT("Moved\r\n");
	}
	
	else if(Command_Check("$MOVE,Z,"))
	{
		if(usart1RxBuff[8] == '-')
		{
			MotorDirection = RollMotor_AntiClockwise;
			MotorStep = usart1RxBuff[9] - 48;
			U1RS232_PRINT("%%Run-RollMotor1 Moving to AntiClockwise as Step %d\r\n", MotorStep);
		}
		else
		{
			MotorDirection = RollMotor_Clockwise;
			MotorStep = usart1RxBuff[8] - 48;
			U1RS232_PRINT("%%Run-RollMotor1 Moving to Clockwise as Step %d\r\n", MotorStep);
		}
		
		turns = MotorStep * (sineArraySize / MaxStep);
		
		for(ip = 0; ip < turns; ip++)
		{
			RunZ_RollMotor1((mdir_t)MotorDirection, JOYSTICK_COM_SPEED);
			Delay_ms(2);
		}
		
		U1RS232_PRINT("Moved\r\n");
	}
	
	else
		U1RS232_PRINT("Not Command\r\n");
	
	// Initial Rx buffer
	for(i = 0; i < USART1_RX_LEN; i++)
	{
		usart1RxBuff[i] = 0;
	}
	
	cntUsart1RxBuff = 0;
}

