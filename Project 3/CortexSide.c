#pragma config(UART_Usage, UART1, uartUserControl, baudRate115200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    ap,             sensorPotentiometer)
#pragma config(Sensor, in4,    rightLine,      sensorReflection)
#pragma config(Sensor, in5,    midRLine,       sensorReflection)
#pragma config(Sensor, in6,    midLine,        sensorLineFollower)
#pragma config(Sensor, in7,    midLLine,       sensorReflection)
#pragma config(Sensor, in8,    leftLine,       sensorReflection)
#pragma config(Sensor, dgtl7,  inSonic,        sensorSONAR_inch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, PIDControl, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port6,           clawMotor,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port7,           armMotor,      tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, PIDControl, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*+++++++++++++++++++++++++++++++++++++++++++++| Notes |++++++++++++++++++++++++++++++++++++++++++++++

----------------------------------------------------------------------------------------------------*/
//Directional movement and arm power enum
enum power {highBP = 100,midBP = 45,lowBP = 20,highAP = 50,midAP = 32,lowAP = 11};
//Inches per tire rotation
const float encCountPerIn = 29;
//Encouder counts per degree for point turn
const float encCountPerDeg = 5.4;
//Encouder counts per degree for swing turn
const float encSwingCountPerDeg = 10;
//currEncCount is accumulated by the encoder cycles that occur when the agent is tracking a line. Agent well then use that sum to go back.
int currEncCount = 0;
//Reset bot motors and encoders
void resetAgent(){
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
	nMotorEncoder[leftMotor] = 0;
	nMotorEncoder[rightMotor] = 0;
}
//Reset all connected sensors (except potentiometer)
void resetSensors(){
	resetSensor(rightLine);
  resetSensor(midLine);
  resetSensor(midLLine);
  resetSensor(midRLine);
  resetSensor(leftLine);
  resetSensor(inSonic);
}

//closeClaw - Closes the claw
void agentCloseClaw(int pwr = 20){
	//Set motor default
	motor[clawMotor] = 0;
	//Close claw
	motor[clawMotor] = -pwr;
	wait1Msec(1000);
	//Set motor default
	motor[clawMotor] = 0;
	wait1Msec(1000);
}
//openClaw - Opens the claw
void agentOpenClaw(){
	//Set motor default
	motor[clawMotor] = 0;
	//Open claw
	motor[clawMotor] = 38;
	wait1Msec(1000);
	//Set motor default
	motor[clawMotor] = 0;
	wait1Msec(1000);
}
//agentArmDown - setting: what height the arm should be lowered (1 or 2)
void agentArmDown(const int setting){
	//Set motor default
	motor[armMotor] = 0;
	//Setting 1 - first position
	if(setting == 1){
		while(SensorValue[ap] < 2100){
			motor[armMotor] = lowAP;
		}
		motor[armMotor] = 0;
	}
	//Setting 2 - second position while not resting on body
	else if(setting == 2){
		while(SensorValue[ap] < 2040){
			motor[armMotor] = lowAP;
		}
		motor[armMotor] = 0;
	}
}
//agentArmUp - setting: what height the arm should be raised (1 or 2)
void agentArmUp(const int setting){
	//Set motor default
	motor[armMotor] = 0;
	//Setting 1 - first position (allows use of sonar)
	if(setting == 1){
		while(SensorValue[ap] > 2570){
			motor[armMotor] = -1 * midBP;
		}
		motor[armMotor] = 0;
	}
	//Setting 2 - second position up
	else if(setting == 2){
		while(SensorValue[ap] > 2040){
			motor[armMotor] = -1 * highAP;
		}
		motor[armMotor] = 0;
	}
}
//agentReverse - distance: in feet - pwr: defaulted at 45
void agentReverse(float distance,int pwr = 45){
	resetAgent();
	//Feet to inches for distance
	distance = distance * 12;
	//Total cycle count for the motors
	int totalCount = (encCountPerIn + 20) * distance;
	//Reverse values
	pwr *= -1;
	totalCount *= -1;
	//Move forward for totalCount's with the power of pwr for each motor
	moveMotorTarget(leftMotor,totalCount,pwr,true);
  moveMotorTarget(rightMotor,totalCount,pwr,true);
	//While the motor cycles are reachinf totalCount, loop. Will exit once reaching totalCount
  while(getMotorTargetCompleted(leftMotor) == false || getMotorTargetCompleted(rightMotor) == false){}
	//Resetting motors
	resetAgent();
	wait1Msec(1000);
}
//agentForward - distance: in feet - pwr: defaulted at 45
void agentForward(float distance,int pwr = 45){
	resetAgent();
	//Feet to inches for distance
	distance = distance * 12;
	//Total cycle count for the motors
	int totalCount = (encCountPerIn + 20) * distance;
	//Move forward for totalCount's with the power of pwr for each motor
	moveMotorTarget(leftMotor,totalCount,pwr,true);
  moveMotorTarget(rightMotor,totalCount,pwr,true);
	//While the motor cycles are reachinf totalCount, loop. Will exit once reaching totalCount
  while(getMotorTargetCompleted(leftMotor) == false || getMotorTargetCompleted(rightMotor) == false){}
  resetAgent();
	wait1Msec(1000);
}
//pointTurnLeft - degree: degree to turn - pwr: defaulted at midAp
void agentPointTurnLeft(const int degree,int pwr = 26){
	//Amount of cycle counts
	float totalCount = encCountPerDeg * degree;
	//Turn left while the cycle count is below totalCount
	motor[leftMotor] = pwr;
	motor[rightMotor] = -1 * pwr;
	while(nMotorEncoder[leftMotor]<totalCount || nMotorEncoder[rightMotor] > -1*totalCount){
		if(nMotorEncoder[leftMotor]>totalCount)	motor[leftMotor] = 0;
		if(nMotorEncoder[rightMotor]< -1*totalCount)	motor[rightMotor] = 0;
	}
	resetAgent();
	wait1Msec(2000);
}
//pointTurnRight - degree: degree to turn - pwr: defaulted at midAp
void agentPointTurnRight(const int degree,int pwr = 26){
	//Amount of cycle counts
	float totalCount = encCountPerDeg * degree;
	//Turn right while the cycle count is below totalCount
	motor[rightMotor] = pwr;
	motor[leftMotor] = -1 * pwr;
	while(nMotorEncoder[rightMotor]<totalCount || nMotorEncoder[leftMotor ] > -1*totalCount){
		if(nMotorEncoder[rightMotor]>totalCount)	motor[rightMotor] = 0;
		if(nMotorEncoder[leftMotor]< -1*totalCount)	motor[leftMotor] = 0;
	}
	resetAgent();
	wait1Msec(2000);
}
//swingTurnLeft - degree: degree to turn - pwr: defaulted at midBP
void agentSwingTurnLeft(const int degree, int pwr = midBP){
	resetAgent();
	//Amount of cycle counts
	float totalCount = encSwingCountPerDeg * degree;
	currEncCount += totalCount;
	motor[leftMotor] = pwr;
	while(nMotorEncoder[leftMotor] < totalCount){
		if(nMotorEncoder[leftMotor] > totalCount)	break;
	}
	resetAgent();
	wait1Msec(1000);
}
//swingTurnRight - degree: degree to turn - pwr: defaulted at midBP
void agentSwingTurnRight(const int degree, int pwr = midBP){
	resetAgent();
	//Amount of cycle counts
	float totalCount = encSwingCountPerDeg * degree;
	currEncCount += totalCount;
	motor[rightMotor] = pwr;
	while(nMotorEncoder[rightMotor] < totalCount){
		if(nMotorEncoder[rightMotor] > totalCount)	break;
	}
	resetAgent();
	wait1Msec(1000);
}

//safetyNet - thread used to ensure the safety of the robot (from collisions)
task safetyNet{
	//Lift the arm up so the sonar can be used without interference
	agentArmUp(1);
	while(true){
		if(SensorValue[inSonic] <= 8)	{
			//Reset the agent from its current task and reverse away from the impact spot
			resetAgent();
			wait1Msec(500);
			agentReverse(0.2);
		}
	}
}
//Global variable used to receive commands
char rcv = NULL;
//connect - thread used to continuously receive data from the uart, which is placed into rcv
task connect(){
	while(true)
	{
		rcv = getChar(UART1);
		wait1Msec(5);
	}
}

task main()
{
	//start safetyNet thread
	startTask(safetyNet);
	//start connect thread
	startTask(connect);
	//Begin infinite loop used to command the agent based on the received data in rcv
	while(true)
	{
		//Sleep
		if(rcv == 'z'){
			wait1Msec(5);
		}
		//Forward
		else if(rcv == 'w'){
				agentForward(0.6);
				resetAgent();
		}
		//Reverse
		else if(rcv == 's'){
			agentReverse(0.3);
			resetAgent();
		}
		//Point turn left
		else if(rcv == 'a'){
			agentPointTurnLeft(90);
			resetAgent();
		}
		//Swing turn left
		else if(rcv == 'q'){
			agentSwingTurnLeft(30);
			resetAgent();
		}
		//Point turn right
		else if(rcv == 'd'){
			agentPointTurnRight(90);
			resetAgent();
		}
		//Swing turn right
		else if(rcv == 'e'){
			agentSwingTurnRight(30);
			resetAgent();
		}
		//Arm up setting 1
		else if(rcv == 'i'){
			agentArmUp(1);
			resetAgent();
		}
		//Arm up setting 2
		else if(rcv == 'j'){
			agentArmUp(2);
			resetAgent();
		}
		//Arm down setting 1
		else if(rcv == 'k'){
			agentArmDown(1);
			resetAgent();
		}
		//Arm down setting 2
		else if(rcv == 'l'){
			agentArmDown(2);
			resetAgent();
		}
		//Open claw
		else if(rcv == 'o'){
			agentOpenClaw();
			resetAgent();
		}
		//Close claw
		else if(rcv == 'p'){
			agentCloseClaw();
			resetAgent();
		}
	}
}
