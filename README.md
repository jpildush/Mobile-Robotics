# Mobile Robotics - With Robot C, C++, VEX Cortex and Beaglebone
* Robot C is used for direct communication between a VEX Cortex and the Agent (robot) - All C files must be uploaded directly to the cortex using Robot C IDE
* The Beaglebone uses a C++ application to communicate with the VEX Cortex (using a UART cable) - C++ application can be placed in a user defined directory to be called by the user
* A terminal emulator will be required to SSH into the Beaglebone (SSH guide not provided)

  This repository consists of three different projects that have been designed to give a VEX Agent (robot), using a VEX Cortex and Beaglebone, semi-autonomous control of its surroundings by utilizing different sensors and multiple motors. This allows the VEX agent to interact with its environment accordingly.

## Sensors and Motors Used
**Motor Type 393 Used**

**Sensor #1** - VEX Motor 393 Integrated Encoder Modules - used to control the wheel motors and ensure exact power adjustment

**Sensor #2** - VEX Line Trackers - used to find a line and travel along the line
  * Calibration for tracking can be found in the source code
  
**Sensor #3** - VEX Ultrasonic Range Finder - used to determine the distance between and object and the agent

**Sensor #4** - VEX Potentiometer - used to control the arm and ensure exact arm positioning


## Project 1  - Beaglebone not Used - [Project Directory](https://github.com/jpildush/Mobile-Robotics/tree/master/Project%201)
Project one is an obstacle course that utilizes 2 x Sensor #1. The course consists of a few turns which lead to the finsh line. This requires calculating different power outputs for the agent's motors and knowing the track length. 

### Designed Path Using Sensor #1 and Associated Motors
* Agent will move forward a few feet
* Agent will turn left 90 degrees
* Agent will move forward a couple feet
* Agent will turn left 90 degrees
* Agent will move forward a few feet
* Agent will turn right 90 degrees 
* Agent will move forward a couple feet and finish the obstacle

## Project 2  - Beaglebone not Used - [Project Directory](https://github.com/jpildush/Mobile-Robotics/tree/master/Project%202)
Project two is an obstacle course that utilizes 2 x Sensor #1, 5 x Sensor #2, 1 x Sensor #3 and 1 x Sensor #4.  The goal of this course is for the agent to find a water bottle and bring it to its proper destination. 
### Designed Path Using Sensor #1 to #4 and Associated Motors
 * Agent will move forward
 * Agent will find a start line (electric tape for me)
 * Agent will make a 90 degree turn to the left
 * Agent will adjust so that the agent is moving along the line and continues to move along the line
 * Agent will continue to move along the line until the agent tracks that the line has ended. 
 * Agent will commence scanning ahead for the bottle and move toward the bottle. 
 * Agent will open its claw and slowly move towards the bottle and stop when the bottle is inside of the claw.
 * Agent will turn 180 degrees and move forward for approximately 2 feet. 
 * Agent will turn 90 degrees to the left and move forward. 
 * Agent will stop in front of a set obstacle - indicating the drop off point - and fully stop. 
 * Agent will drop the bottle  - completing the obstacle course!

## Project 3  - Beaglebone Used - [Project Directory](https://github.com/jpildush/Mobile-Robotics/tree/master/Project%203)
Project three requires user input to control the Agent - custom built autonomous safety net method is designed to ensure safety of Agent regardless of what the user commands. The user must use a terminal emulator to connect to the Beaglebone, over the Beaglebone's LAN network. 
 
### Designed Path Using Sensor #1 to #4 and Associated Motors
 * There is no designed path for this project because the user has full control of the Agent. Safety net will override user command if Agent is in danger. The Agent will make use of all sensors and motors at the users request.


## Demonstration of Operation
**Demonstration 1**
 * In the video it can be noticed that the arm has surpassed the acceptable arch point. Make sure to thoroughly test that Sensor #4 is operating properly, otherwise the Agent may get damaged
 
 [![demo1](https://img.youtube.com/vi/j3XvzgxCWWs/0.jpg)](https://youtu.be/j3XvzgxCWWs)

 
**Demonstration 2**


**Demonstration 3**
