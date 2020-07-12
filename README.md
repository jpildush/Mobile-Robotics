# Mobile Robotics - With Robot C, C++, VEX Cortex and Beaglebone
* Robot C is used for direct communication between a VEX Cortex and a Beaglebone (Bealgebone has been connected to the VEX Cortex through a UART cable)
* C++ is used to communicate from terminal (Mac) to the Beaglebone through the Beaglebone's LAN network

  This repository consists of four different projects that have been designed to give a VEX agent (robot), using a VEX Cortex, autonomous control of its surroundings by utilizing different sensors and multiple motors. This allows the VEX agent to interact with its environment accordingly.

## Sensors and Motors Used
**Sensor#1 and Motor Type 393** - VEX Motor 393 Integrated Encoder Modules - used to control the wheel motors and ensure exact power adjustment

**Sensor #2** - VEX Line Trackers - used to find a line and travel along the line
  * Calibration for tracking can be found in the source code
  
**Sensor #3** - VEX Ultrasonic Range Finder - used to determine the distance between and object and the agent

**Sensor #4** - VEX Potentiometer - used to control the arm and ensure exact arm positioning


## Project 1 - [Project Directory](https://github.com/jpildush/Mobile-Robotics/tree/master/Project%201)
Project one is an obstacle course that utilizes 2 x Sensor #1. The course consists of a few turns which lead to the finsh line. This requires calculating different power outputs for the agent's motors and knowing the track length. 
### Designed Path

## Project 2 - [Project Directory](https://github.com/jpildush/Mobile-Robotics/tree/master/Project%202)
Project two is an obstacle course that utilizes 2 x Sensor #1, 5 x Sensor #2, 1 x Sensor #3 and 1 x Sensor #4.  The goal of this course is for the agent to find a water bottle and bring it to its proper destination. 
### Designed Path
 * Using Sensor #1 and associated motors - Agent will move forward
 * Using Sensor #2 - Agent will find a start line (electric tape for me)
 * Using Sensor #1 and associated motors - Agent will make a 90 degree turn to the left
 * Using Sensor #1 & #2 and associated motors - Agent will adjust so that the agent is moving along the line and continues to move along the line
 * Using Sensor #1 to #3 and associated motors - Agent will continue to move along the line until the agent tracks that the line has ended. Agent will commence scanning ahead for the bottle and move toward the bottle. 
 * Using Sensor #1 to #4 and associated motors - Agent will open its claw and slowly move towards the bottle and stop when the bottle is inside of the claw.
 * Using Sensor #1 to #3 and associated motors - Agent will turn 180 degrees and move forward for approximately 2 feet. Agent will turn 90 degrees to the left and move forward. Agent will stop in front of a set obstacle - indicating the drop off point - and fully stop. 
 * Using Sensor #4 and associated motors - Agent will drop the bottle  - completing the obstacle course!

## Project 3 - [Project Directory](https://github.com/jpildush/Mobile-Robotics/tree/master/Project%203)

### Designed Path

## Project 4 - [Project Directory](https://github.com/jpildush/Mobile-Robotics/tree/master/Project%204)

### Designed Path
