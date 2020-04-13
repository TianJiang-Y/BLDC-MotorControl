
This is simple firmware for testing three motors on Henway Gimbal Board.
There are three LEDs on Henway Gimbal Board.
Manually move motors in any direction by sending commands over RS232
Regarding RS232 commands(Protocols), please consider a pdf "HOOGimbal-JoystickSpecs"
Motor Step value range that can set up is from 1 to 9.
If receive Command over RS232, Board turn on LED2(red LED) and output "Receive Joystick Command" to RS232.
Next, after carry out Joystick command, Board turn off LED2 and transmit needed reply depending on received command.

Ex: Test of Move motors in any direction.
Command : < $MOVE,X,-2\r\n >
If board receive this command over RS232, board will turn Motor0(Pitch motor) as 2 steps distance to down.
Next, after turn the motor, it response as < %Run-PitchMotor0 Moving to down as Step 2 >.
After finish to move, response as < Moved >.

User can decide the connector direction of three motors by using this firmware.
For example, user transmit a command as "move to right as step 3", but if motor will be turn to left, The user must reconnect the motor connector in reverse.
After it, it can confirm motor rotation direction.

According to above method, user can decide connecting direction of motors' connector.

-----------------------------------------------------------------------------------------------------------------------------------------------------------
In this firmware source code, It is a program that improved the speed of the motor, the driving force of the motor, and the reaction speed according to the gimbal motion change.
2020-02-12
Jiang.
-----------------------------------------------------------------------------------------------------------------------------------------------------------

Thank you.