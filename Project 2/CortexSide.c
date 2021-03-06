#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    armPotent,      sensorPotentiometer)
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
//encFindUnk is the accumulated encoder cycles when the agent travels on the second line. Will be used to go back.
int encFindUnk = 0;
//lineLLimit is the limit of the left most light sensor for a black line
int	lineLLimit = 2940;
//lineRLimit is the limit of the right most light sensor for a black line
int lineRLimit = 2940;
//lineMidLimit is the limit of the three sensors below the agent for a black line
int lineMidLimit = 2760;
//Reset bot motors and encoders
void resetAgent(){
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
	nMotorEncoder[leftMotor] = 0;
	nMotorEncoder[rightMotor] = 0;
}
//Reset all connected sensors
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
//agentArmDown - degree: degree to move - pwr: highAP,midAP,lowAP
void agentArmDown(const int degree, int pwr = midAP){
	float waitTime;
	//Set motor default
	motor[armMotor] = 0;
	//Higher power will result in a quicker succession
	if(pwr == (int)highAP)	waitTime = 3 * degree;
	else if(pwr == (int)midAP)	waitTime = 7 * degree;
	else if(pwr == (int)lowAP)	waitTime = 44 * degree;
	//Start motor
	motor[armMotor] = pwr;
	wait1Msec(waitTime);
	//Set motor default
	motor[armMotor] = 0;
	wait1Msec(1000);
}
//agentArmUp - degree: degree to move - pwr: highAP,midAP,lowAP
void agentArmUp(const int degree,int pwr = midAP){
	float waitTime;
	//Set motor default
	motor[armMotor] = 0;
	//Higher power will result in a quicker succession
	if(pwr == (int)highAP)	waitTime = 22 * degree;
	else if(pwr == (int)midAP)	waitTime = 44 * degree;
	else if(pwr == (int)lowAP)  waitTime = 44 * degree;
	//Start motor
	motor[armMotor] = -1 * pwr;
	wait1Msec(waitTime);
	//Set motor default
	motor[armMotor] = 0;
	wait1Msec(1000);
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
//agentFindLine - agent will commence to find the line that is directly infront of it.
void agentFindLine(){
	motor[leftMotor] = midAP;
	motor[rightMotor] = midAP;
	//While the outermost sensors do not pick up any black tape, continue moving forward
	while(SensorValue[rightLine] < lineRLimit && SensorValue[leftLine] < lineLLimit){}
	resetAgent();
	wait1Msec(500);
}
//agentForwardToObject - agent will move forward along the black line to get to the object at the end of the tape.
void agentForwardToObject(){
	//Infinite loop that would continue to check if the middle sensors are tracking the black line and otherwise would adjust.
	while(true){
		//While the middle sensor of the 3 middle ones, is directly on the black line
		if(SensorValue[midLine] > lineMidLimit){
			motor[leftMotor] = 20;
			motor[rightMotor] = 20;
		}
		//While the right sensor of the middle sensors is directly on the black line, readjust to the right
		if(SensorValue[midRLine] > lineMidLimit){
			agentSwingTurnRight(8);
			//Check if object is infront of the bot. If so, break the while loop to stop any further adjustments
			if(SensorValue[inSonic] < 10 && SensorValue[inSonic] != -1)	break;
			agentSwingTurnLeft(5);
			//Check if object is infront of the bot. If so, break the while loop to stop any further adjustments
			if(SensorValue[inSonic] < 10 && SensorValue[inSonic] != -1)	break;
		}
		//While the left sensor of the middle sensor is directly on the black line, readjust to the left
		if(SensorValue[midLLine] > lineMidLimit){
			agentSwingTurnLeft(8);
			//Check if object is infront of the bot. If so, break the while loop to stop any further adjustments
			if(SensorValue[inSonic] < 10 && SensorValue[inSonic] != -1)	break;
			agentSwingTurnRight(5);
			//Check if object is infront of the bot. If so, break the while loop to stop any further adjustments
			if(SensorValue[inSonic] < 10 && SensorValue[inSonic] != -1)	break;
		}
		//Check if object is infront of the bot. If so, break the while loop to stop any further adjustments
		if(SensorValue[inSonic] < 10 && SensorValue[inSonic] != -1) break;
		//Otherwise check to see if the three line trackers in the middle do not see a black line at all. If so, reverse until black line has been found
		else if(SensorValue[midLLine] < lineMidLimit && SensorValue[midRLine] < lineMidLimit && SensorValue[midLine] < lineMidLimit){
			while(SensorValue[midLine] < lineMidLimit){
				int tempCount = nMotorEncoder[leftMotor];
				motor[rightMotor] = -20;
				motor[leftMotor] = -20;
				tempCount -= nMotorEncoder[leftMotor];
				currEncCount -= tempCount;
			}
		}
	}
	currEncCount += nMotorEncoder[leftMotor];
	resetAgent();
	wait1Msec(500);
}
//agentAlignToLine - Agent will align itself to the unknown line that it has found, for 5 seconds
void agentAlignToLine(){
	currEncCount = 0;
	agentForward(1);
	clearTimer(T1);
	while(time1[T1] < 5000){
		//If middle sensor is on line, move forward
		if(SensorValue[midLine] > lineMidLimit){
			motor[leftMotor] = 20;
			motor[rightMotor] = 20;
		}
		//If the middle right sensor is on the line, make adjustments to the right
		else if(SensorValue[midRLine] > lineMidLimit){
			agentSwingTurnRight(8);
			agentSwingTurnLeft(5);
		}
		//If middle left sensor is on the line, make adjustments to the left
		if(SensorValue[midLLine] > lineMidLimit){
			agentSwingTurnLeft(8);
			agentSwingTurnRight(5);
		}
		//If none of the three middle sensors are tracking a black line, reverse until the Agent rides onto the black line again
		if(SensorValue[midLLine] < lineMidLimit && SensorValue[midRLine] < lineMidLimit && SensorValue[midLine] < lineMidLimit){
			while(SensorValue[midLine] < lineMidLimit){
				int tempCount = nMotorEncoder[leftMotor];
				motor[rightMotor] = -20;
				motor[leftMotor] = -20;
				tempCount -= nMotorEncoder[leftMotor];
				currEncCount -= tempCount;
			}
		}
	}
	resetAgent();
	wait1Msec(200);
}
//calibAgent - The agent will calibrate its sensors to the floor being used.
void calibAgent(){
	int midSensor = (SensorValue[midLLine] + SensorValue[midLine] + SensorValue[midRLine])/3;
	lineLLimit = (lineLLimit + SensorValue[leftLine])/2;
	lineRLimit = (lineRLimit + SensorValue[rightLine])/2;
 	lineMidLimit = (lineMidLimit + midSensor)/2;
 	wait1Msec(2000);
}
//agentGrabObject - The agent will open its claw and move slowly forward until the object is right inside the claw, at which point the Agent will close its claw
void agentGrabObject(){
	agentOpenClaw();
	//Move forward to object until inSonic reads less than 6
	motor[leftMotor] = 20;
	motor[rightMotor] = 20;
	while(SensorValue[inSonic] > 6){}
	resetAgent();
	//Lower the arm so the claw is around the object
	agentArmDown(4);
	wait1Msec(1000);
	agentCloseClaw(midAP);
	resetAgent();
}
//agentFindUnkLine - The agent will search for an unknown line
void agentFindUnkLine(){
	agentForward(2);
	//While the left and right sensors do not track a black line, search for black line
	while(SensorValue[leftLine] < lineLLimit || SensorValue[rightLine] < lineRLimit){
		agentPointTurnRight(90);
		resetAgent();
		motor[rightMotor] = midAP;
		motor[leftMotor] = midAP;
		//Travel 25% of the encouder cycles used in the first black line to search for the second black line.
		while(nMotorEncoder[leftMotor] < encFindUnk) {
			if(SensorValue[leftLine] > lineLLimit || SensorValue[rightLine] > lineRLimit)	break;
		}
		//While travelling to the right, make a swing turn left if the black line is found
		if(SensorValue[leftLine] > lineLLimit || SensorValue[rightLine] > lineRLimit){
			agentReverse(0.27,lowBP);
			agentSwingTurnLeft(90);
			break;
		}
		resetAgent();
		wait1Msec(500);
		//Turn around if the black line has not been found
		agentPointTurnLeft(180);
		resetAgent();
		wait1Msec(500);
		//Move double the distance just moved, creating a zig zagging T formation.
		motor[rightMotor] = midAP;
		motor[leftMotor] = midAP;
		while(nMotorEncoder[leftMotor] < encFindUnk*2) {
			if(SensorValue[leftLine] > lineLLimit || SensorValue[rightLine] > lineRLimit)	break;
		}
		//If black line is found, commence a swing turn to the right
		if(SensorValue[leftLine] > lineLLimit || SensorValue[rightLine] > lineRLimit){
			agentReverse(0.27,lowBP);
			agentSwingTurnRight(90);
			break;
		}
		resetAgent();
		wait1Msec(500);
		//If black line is not found, turn around again
		agentPointTurnRight(180);
		resetAgent();
		wait1Msec(500);
		//Move to the top of the T formation and start moving up creating a new T formation in the cycling loop
		motor[rightMotor] = midAP;
		motor[leftMotor] = midAP;
		while(nMotorEncoder[leftMotor] < encFindUnk){}
		resetAgent();
		wait1Msec(500);
		agentPointTurnLeft(90);
		wait1Msec(500);
		agentForward(1.5);
		wait1Msec(500);
	}

}
//agentCalcBack - Agent calculates the encoder cycles back to feet, used when agent is moving forward or backwards
void agentCalcBack(){
	float tempCount = ceil(currEncCount * 0.4);
	encFindUnk = tempCount;
	int distance = ceil((currEncCount/(encCountPerIn + 20)));
	tempCount = ceil((distance * 0.5)/12);
	currEncCount = tempCount;
}
//agentEndOfLine - Agent will travel right to the end of the black line and then stop
void agentEndOfLine(){
	while(true){
		//If the middle sensor of the three middle sensors is on the black line, then move forward
		if(SensorValue[midLine] > lineMidLimit){
			motor[leftMotor] = 20;
			motor[rightMotor] = 20;
		}
		//If the right sensor of the three middle sensors is on the black line, then adjust to the right
		if(SensorValue[midRLine] > lineMidLimit){
			agentSwingTurnRight(8);
			agentSwingTurnLeft(5);
		}
		//If the left sensor of the three middle sensors is on the black line, then adjust to the left
		if(SensorValue[midLLine] > lineMidLimit){
			agentSwingTurnLeft(8);
			agentSwingTurnRight(5);

		}
		//Otherwise if the middle sensors do not see anymore black line, the agent has come to its destination to dropoff the object
		if(SensorValue[midLLine] < lineMidLimit && SensorValue[midRLine] < lineMidLimit && SensorValue[midLine] < lineMidLimit){
			currEncCount += nMotorEncoder[leftMotor];
			resetAgent();
			break;
		}
	}
}
//agentFindBarrier - Agent will drive forward and use its range finder sensor to get very close to the barrier wall
void agentFindBarrier(){
	motor[leftMotor] = midAP;
	motor[rightMotor] = midAP;
	agentArmUp(7);
	while(SensorValue[inSonic] > 7){
		if(SensorValue[inSonic] <= 7)
			break;
	}
	agentArmDown(6);
	resetAgent();
}

//Main Task Start
task main()
{
	/*
	//Reset the Agents movement motors and associated encoders
	resetAgent();
	//Reset the Agents sensors
	resetSensors();
	//Calibrate agent to the track
	calibAgent();
	//Agent start to look for a line directly infront of itself
	agentFindLine();
	//Reverse and initiate a swing turn to the left to get onto the black line
	agentReverse(0.27,lowBP);
	agentSwingTurnLeft(90);
	//The agent will take 5 seconds to align itself to the black line
	agentAlignToLine();
	//The agent will lift up the arm to allow the range finder to track distance properly
	agentArmUp(7);
	//Agent will move along the black line until the range finder senses the object
	agentForwardToObject();
	//The agent will move close to the object and grab it
	agentGrabObject();
	//The agent will adjust its arm to not drag the object on the track
	agentArmUp(3);
	//The agent will turn 180 degress
	agentPointTurnRight(180);
	//Agent will calculate the neeed encouder cycles to get to about 60%-70% of the earlier traversed black line
	agentCalcBack();
	//Agent move forward the designated encoder counts
	agentForward(currEncCount);
	//Agent will turn left to face the unknown line
	agentPointTurnLeft(90);
	//Agent will start looking for the unknown line by zig zagging in a T formation
	agentFindUnkLine();
	//Once unknown line has been found, align to that line
	agentAlignToLine();
	//Agent will follow the black line until the finish.
	agentEndOfLine();
	//Put the object down in the designated position
	agentArmDown(3);
	//Open claw to release object
	agentOpenClaw();
	//Agent will reverse away from object
	agentReverse(1);
	//Agent will will turn 180 degrees
	agentPointTurnLeft(180);
	//Agent will calculate the needed encoder cycles to get to the other end of the black line
	agentCalcBack();
	//Agent moves forward
	agentForward(currEncCount);
	//Agent will turn to the right
	agentPointTurnRight(90);
	//Agent moves forward away from the black line
	agentForward(1);
	//Agent will start trying to find the barrier to get to the finish
	agentFindBarrier();
	//Once the barrier has been found, reverse slightly and turn to the right to face the finish line
	agentReverse(0.3);
	agentPointTurnRight(90);
	//Agent will move forward towards the finish line and stop once it gets to the finish line and right up against the wall
	agentFindBarrier();*/
}
