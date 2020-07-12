#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, PIDControl, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port6,           clawMotor,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port7,           armMotor,      tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, PIDControl, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*+++++++++++++++++++++++++++++++++++++++++++++| Notes |++++++++++++++++++++++++++++++++++++++++++++++
Lab1 Maze

Robot Model(s): Modified Squarebot

[I/O Port]          [Name]              [Type]                [Description]
Motor Port 1        rightMotor          393 Motor             Right side motor, Reversed
Motor Port 10        leftMotor           393 Motor             Left side motor
I2C_1               leftIEM            Integrated Encoder    Encoder mounted on rightMotor
I2C_2               rightIEM             Integrated Encoder    Encoted mounted on leftMotor
----------------------------------------------------------------------------------------------------*/
//Directional/Operational enum
enum gearShift {left,right,forw,back,up,down,open,close};
//Directional movement and arm power enum
enum power {highBP = 100,midBP = 45,lowBP = 20,highAP = 50,midAP = 32,lowAP = 11};
//Inches per tire rotation
const float encCountPerIn = 29;
//Cycles per degree
const float encCountPerDeg = 5.7; //5.7
//Reset bot motors and encoders
void resetBot(){
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
	nMotorEncoder[leftMotor] = 0;
	nMotorEncoder[rightMotor] = 0;
}
//Move the claw - Movement: open,close - dowait: true if NOT doing victory()
void moveClaw(const int movement,bool dowait = true){
	motor[clawMotor] = 0;
	switch (movement){
		case open:
			motor[clawMotor] = 42;
			wait1Msec(1000);
			break;
		case close:
			motor[clawMotor] = -20;
			wait1Msec(1000);
			break;
	}
	motor[clawMotor] = 0;
	if(dowait)	wait1Msec(150);
}
//Move the arm - direction: up,down - degree: degree to move - pwr: highAP,midAP,lowAP - dowait: true if NOT doing victory()
void moveArm(const int direction,const int degree, int pwr = midAP, bool dowait = true){
	float waitTime;
	motor[armMotor] = 0;
	switch (direction){
		case up:
			if(dowait){
				//Higher power will result in a quicker succession
				if(pwr == (int)highAP)	waitTime = 22 * degree;
				else if(pwr == (int)midAP)	waitTime = 44 * degree;
				else if(pwr == (int)lowAP)  waitTime = 44 * degree;
				motor[armMotor] = -1 * pwr;
				wait1Msec(waitTime);
			}
			//Continue Victory
			else{
				motor[armMotor] = -30;
				moveClaw(open);
				moveClaw(close);
			}
			break;
		case down:
			if(dowait){
				//Higher power will result in a quicker succession
				if(pwr == (int)highAP)	waitTime = 3 * degree;
				else if(pwr == (int)midAP)	waitTime = 7 * degree;
				else if(pwr == (int)lowAP)	waitTime = 44 * degree;
				motor[armMotor] = pwr;
				wait1Msec(waitTime);
			}
			//Continue Victory
			else{
				motor[armMotor] = 20;
				moveClaw(open);
				moveClaw(close);
			}
			break;
		default:
			break;
	}
	motor[armMotor] = 0;
	//If doing Victory, wait differently - for the spin
	if(dowait)	wait1Msec(1000);
	else	wait1Msec(540);
}
//Calls upon the moveArm functions to finish victory
void victory(){
	moveArm(up,1,midAP,false);
	moveArm(down,1,midAP,false);
}
//moveBot - direction: forw,back - mvDist: distance in feet - pwr: defaulted at 45 - dowait: true if NOT doing victory()
void moveBot(const int direction, float mvDist,int pwr = 45, bool dowait = true){
	//Feet to inches for distance
	mvDist = mvDist * 12;
	//Total cycle count for the motors
	int totalCount = (encCountPerIn + 20) * mvDist;
	switch (direction){
		case forw:
			//Move forward for totalCount's with the power of pwr for each motor
			moveMotorTarget(leftMotor,totalCount,pwr,true);
		  moveMotorTarget(rightMotor,totalCount,pwr,true);
			//While the motor cycles are reachinf totalCount, loop. Will exit once reaching totalCount
		  while(getMotorTargetCompleted(leftMotor) == false || getMotorTargetCompleted(rightMotor) == false){}
			break;
		case back:
			//Reverse values
			pwr *= -1;
			totalCount *= -1;
			//Move forward for totalCount's with the power of pwr for each motor
			moveMotorTarget(leftMotor,totalCount,pwr,true);
		  moveMotorTarget(rightMotor,totalCount,pwr,true);
			//While the motor cycles are reachinf totalCount, loop. Will exit once reaching totalCount
		  while(getMotorTargetCompleted(leftMotor) == false || getMotorTargetCompleted(rightMotor) == false){}
			break;
		default:
			break;
	}
	//Resetting motors
	resetBot();
	//Disable the wait IF victory is enabled
	if(dowait)	wait1Msec(1000);
}
//turnDeg - direction: left,right - degree: degree to turn - pwr: defaulted at 20 - dowait: true if NOT doing victory()
void turnDeg(const int direction,const int degree,int pwr = midAP, bool dowait = true){
	//Amount of cycle counts
	float totalCount = encCountPerDeg * degree;
	switch(direction)
	{
		case left:
			//Turn left while the cycle count is below totalCount
			motor[leftMotor] = pwr;
			motor[rightMotor] = -1 * pwr;
			while(nMotorEncoder[leftMotor]<totalCount || nMotorEncoder[rightMotor] > -1*totalCount){
				if(nMotorEncoder[leftMotor]>totalCount)	motor[leftMotor] = 0;
				if(nMotorEncoder[rightMotor]< -1*totalCount)	motor[rightMotor] = 0;
				//IF pwr has been set to highBP, commence victory()
				if(pwr == (int)highBP)	victory();
			}
			break;
		case right:
			//Turn right while the cycle count is below totalCount
			motor[rightMotor] = pwr;
			motor[leftMotor] = -1 * pwr;
			while(nMotorEncoder[rightMotor]<totalCount || nMotorEncoder[leftMotor ] > -1*totalCount){
				if(nMotorEncoder[rightMotor]>totalCount)	motor[rightMotor] = 0;
				if(nMotorEncoder[leftMotor]< -1*totalCount)	motor[leftMotor] = 0;
				//IF pwr has been set to highBP, commence victory()
				if(pwr == (int)highBP)	victory();
			}
			break;
		default:
			break;
	}
	resetBot();
	//Disable the wait IF victory is enabled
	if(dowait)	wait1Msec(2000);
}
//Main Task Start
task main()
{
	resetBot();
	wait1Msec(2000);
	moveBot(forw,2.5);
	turnDeg(left,90);
	moveBot(forw,4);
	turnDeg(right,90);
	moveBot(forw,3);
	turnDeg(right,90);
	moveBot(forw,4.5);
	//Maze finished - Victory commence
	turnDeg(right,1,highBP,false); //Calls nested victory function - pwr must be set to highBP and dowait must be set to FALSE
}
