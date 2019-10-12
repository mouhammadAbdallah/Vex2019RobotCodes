/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    leftCIPot,      sensorPotentiometer)
#pragma config(Sensor, in2,    rightCIPot,     sensorPotentiometer)
#pragma config(Sensor, in3,    MobPot,         sensorPotentiometer)
#pragma config(Sensor, in6,    backIR,         sensorLineFollower)
#pragma config(Sensor, in7,    frontIR,        sensorLineFollower)
#pragma config(Sensor, in8,    myGyro,         sensorGyro)
#pragma config(Sensor, dgtl1,  CraneEnc,       sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  CraneEncR,      sensorQuadEncoder)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           mobIntake1,    tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           leftDrive,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           rightDrive,    tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port4,           crane1,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           crane2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           crane3,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           crane4,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           coneInLeft,    tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           coneInRight,   tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          intakeMotor,   tmotorVex393HighSpeed_HBridge, openLoop, reversed)
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

#define MOTOR_NUM               kNumbOfTotalMotors
#define MOTOR_MAX_VALUE         127
#define MOTOR_MIN_VALUE         (-127)
#define MOTOR_DEFAULT_SLEW_RATE 30      // Default will cause 375mS from full fwd to rev
#define MOTOR_FAST_SLEW_RATE    256     // essentially off
#define MOTOR_TASK_DELAY        15      // task 1/frequency in mS (about 66Hz)
#define MOTOR_DEADBAND          10
void pre_auton()
{
  int motorReq[MOTOR_NUM];
int motorSlew[MOTOR_NUM];

task MotorSlewRateTask()
{
	int motorIndex;
	int motorTmp;


	// Initialize stuff
	for (motorIndex = 0; motorIndex < (signed)MOTOR_NUM; motorIndex++)
	{
		motorReq[motorIndex] = 0;
		motorSlew[motorIndex] = MOTOR_DEFAULT_SLEW_RATE;
	}

	// run task until stopped
	while (true)
	{
		// run loop for every motor
		for (motorIndex = 0; motorIndex < (signed)MOTOR_NUM; motorIndex++)
		{
			// So we don't keep accessing the internal storage
			motorTmp = motor[motorIndex];

			// Do we need to change the motor value ?
			if (motorTmp != motorReq[motorIndex])
			{
				// increasing motor value
				if (motorReq[motorIndex] > motorTmp)
				{
					motorTmp += motorSlew[motorIndex];
					// limit
					if (motorTmp > motorReq[motorIndex])
						motorTmp = motorReq[motorIndex];
				}

				// decreasing motor value
				if (motorReq[motorIndex] < motorTmp)
				{
					motorTmp -= motorSlew[motorIndex];
					// limit
					if (motorTmp < motorReq[motorIndex])
						motorTmp = motorReq[motorIndex];
				}

				// finally set motor
				motor[motorIndex] = motorTmp;
			}
		}

		// Wait approx the speed of motor update over the spi bus
		wait1Msec(MOTOR_TASK_DELAY);
	}
}

  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
  // ..........................................................................
  // Insert user code here.
  // ..........................................................................

  // Remove this function call once you have "real" code.
  AutonomousCodePlaceholderForTesting();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
  // User control code here, inside the loop

  while (true)
  {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // Remove this function call once you have "real" code.
    UserControlCodePlaceholderForTesting();
  }
}
