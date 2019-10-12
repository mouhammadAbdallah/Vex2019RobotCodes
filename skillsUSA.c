#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    myGyro,         sensorGyro)
#pragma config(Sensor, in2,    clawsPot,       sensorPotentiometer)
#pragma config(Sensor, in3,    backLim,        sensorLineFollower)
#pragma config(Sensor, in4,    frontLim,       sensorLineFollower)
#pragma config(Sensor, in6,    backLim2,       sensorLineFollower)
#pragma config(Sensor, dgtl1,  craneEnc,       sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  RightBump,      sensorTouch)
#pragma config(Sensor, dgtl4,  LeftBump,       sensorTouch)
#pragma config(Sensor, dgtl5,  AdjtSwitch,     sensorTouch)
#pragma config(Sensor, dgtl6,  RorB,           sensorTouch)
#pragma config(Sensor, dgtl7,  AdjtSwitch2,    sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           MobGoalntake,  tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           frontRight1,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           frontLeftExp,  tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           rightClaws,    tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           backRight,     tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           backLeft,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           leftClaws,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           craneMotorsExp, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           frontRight2,   tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port10,          rollerMotor,   tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"

#define MOTOR_NUM               kNumbOfTotalMotors
#define MOTOR_MAX_VALUE         127
#define MOTOR_MIN_VALUE         (-127)
#define MOTOR_DEFAULT_SLEW_RATE 30      // Default will cause 375mS from full fwd to rev
#define MOTOR_FAST_SLEW_RATE    256     // essentially off
#define MOTOR_TASK_DELAY        20      // task 1/frequency in mS (about 66Hz)
#define MOTOR_DEADBAND          10
#define MAX_CMD									127

void moveClawsTo(int clawsTar);
void moveClawsWith(int speed);
void moveCraneWith(int speed);
void moveCraneTo(int craneTar);
void moveRobot(int RSpeed, int LSpeed);
void turnRollerToEject();
void turnRollerToEat();

int motorReq[MOTOR_NUM];
int motorSlew[MOTOR_NUM];

int Err = 0;
int IErr = 0;
int test = 0;
bool MobInDown = false;
bool MobInUp = false;


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


void pre_auton()
{
	bStopTasksBetweenModes = true;
	SensorValue[craneEnc] = 0;
	SensorValue[myGyro] = 0;
	nMotorEncoder[frontRight2] = 0;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// 			P R O G R A M M I N G    S K I L L S					//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
	//////////////////////////////////////////////////////////////////////////////////////// Getting First Mobile Goal
	nMotorEncoder[frontRight2] = 0;
	startTask(MotorSlewRateTask);
	SensorValue[myGyro]= 0;
	Err = 0;
	clearTimer(T1);
	IErr = 0;
	/////////////////

	//////////////////

	// move straight until line detection then brake
	while(nMotorEncoder[frontRight2] < 1100 || SensorValue[backLim] > 1700)
	{
		Err = SensorValue[myGyro];
		IErr = IErr + Err;
		moveRobot(60 - 0.6*Err - 0.0001*IErr,60 + 0.6*Err + 0.0001*IErr);
		if(time1(T1) < 900)
		{
			motorReq[MobGoalntake] = -127;
		}
		else
		{
			motorReq[MobGoalntake] = 0;
		}
		if(time1(T1) > 5000)
		{
			while(true)
			{
				moveRobot(0,0);
			}
		}
	}
	moveRobot(0,0);
	clearTimer(T2);
	while(time1(T2)<100)
	{
		moveRobot(-50,-50);
	}
	moveRobot(0,0);

	//
	nMotorEncoder[frontRight2] = 0;
	IErr = 0;
	while(nMotorEncoder[frontRight2] < 150)
	{
		IErr = IErr + Err;
		Err = SensorValue[myGyro];
		moveRobot(60 - 0.8*Err - 0.0003*IErr,60 + 0.8*Err + 0.0003*IErr);
	}
	moveRobot(0,0);
	//put goal on robot
	IErr = 0;
	clearTimer(T1);
	while(time1(T1)<1500)
	{
		IErr = IErr + Err;
		motorReq[MobGoalntake] = 127;
		Err = SensorValue[myGyro];
		//moveRobot(-0.6*Err,0.6*Err);
	}
	moveRobot(0,0);
	motorReq[MobGoalntake] = 0;

	nMotorEncoder[frontRight2] = 0;
	IErr = 0;
	//moving back g1
	while(abs(nMotorEncoder[frontRight2]) < 700 || SensorValue[backLim] > 1800)
	{

		Err = SensorValue[myGyro] + 10;
		IErr = IErr + Err;
		moveRobot(-100 - 1*Err - 0.0001*IErr,-100 + 1*Err + 0.0001*IErr);
	}
	moveRobot(0,0);
	motorReq[rollerMotor] = 0;
	clearTimer(T2);
	while(time1(T2) < 100)
	{
		moveRobot(-50,-50);
	}
	moveRobot(0,0);
	//turn g1 : moving on white line
	while(SensorValue[myGyro] < 300)
	{
		moveRobot(60,-60);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 600)
	{
		Err = SensorValue[myGyro] - 450;
		moveRobot(-0.3*Err,0.3*Err);
	}

	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	clearTimer(T1);
	while(nMotorEncoder[frontRight2] > -750 || time1(T1) < 200)
	{
		Err = SensorValue[myGyro] - 450;
		moveRobot(-60 - 0.8*Err,-60 + 0.8*Err);
	}
	moveRobot(0,0);
	//turn g1 : going to score
	while(SensorValue[myGyro] < 1200)
	{
		moveRobot(60,-60);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1)<1000)
	{
		Err = SensorValue[myGyro] - 1350;
		moveRobot(-0.4*Err,0.4*Err);
	}

	clearTimer(T1);
	while(time1(T1) < 900)
	{
		moveRobot(127,127);
		if(time1(T1) > 500)
		{
			motorReq[MobGoalntake] = -127;
		}
	}
	motorReq[MobGoalntake]  = 0;
	moveRobot(0,0);
	//moveCraneWith(0);
	//clearTimer(T1);
	//while(time1(T1) < 300)
	//{
	//	moveRobot(-70,-70);
	//}
	//moveRobot(0,0);
	//clearTimer(T1);
	//while(time1(T1) < 400)
	//{
	//	moveRobot(70,70);
	//}
	moveRobot(0,0);

	clearTimer(T1);
	while(time1(T1) < 700)
	{
		motorReq[MobGoalntake] = 127;
	}
	motorReq[MobGoalntake] = 0;
	clearTimer(T1);
	nMotorEncoder[frontRight2] = 0;
	while(abs(nMotorEncoder[frontRight2]) < 500 || time1(T1) < 200)
	{
		if(abs(nMotorEncoder[frontRight2])> 100)
		{
			motorReq[MobGoalntake] = -127;
		}
		else
		{
			motorReq[MobGoalntake] = 0;
		}
		moveRobot(-50,-50);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 100)
	{
		moveRobot(50,50);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 500)
	{
		//motorReq[MobGoalntake] = 127;
	}
	motorReq[MobGoalntake] = 0;
	SensorValue[myGyro] = 0;
	//start of wallAjt
	while(SensorValue[myGyro] > -600)
	{
		moveRobot(-50,50);
	}
	moveRobot(0,0);
	while(SensorValue[AdjtSwitch] == 0)
	{
		Err = SensorValue[myGyro] + 900;
		moveRobot(-40 - 0.3*Err, -40 + 0.3*Err);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 200)
	{
		moveRobot(50,50);
	}
	moveRobot(0,0);
	while(SensorValue[myGyro] > -1350)
	{
		moveRobot(-70,0);
	}
	moveRobot(0,0);
	//wallAjt
	clearTimer(T2);
	while(true)
	{
		moveRobot(-40,-40);
		if(time1(T2) > 250 || (SensorValue[LeftBump] == 1 && SensorValue[RightBump] == 1))
		{
			break;
		}
	}
	moveRobot(0,0);
	//moving to g2
	SensorValue[myGyro] = 0;
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] < 50)
	{
		motorReq[MobGoalntake] = -127;
		Err = SensorValue[myGyro];
		moveRobot(60 - 0.5*Err,60 + 0.5*Err);
	}
	moveRobot(0,0);

	nMotorEncoder[frontRight2] = 0;
	clearTimer(T2);
	while(nMotorEncoder[frontRight2] < 1050)
	{
		if(time1(T2) < 400)
		{
			motorReq[MobGoalntake] = -127;
		}
		else
		{
			motorReq[MobGoalntake] = -30;
		}
		Err = SensorValue[myGyro] + 250;
		//Err = SensorValue[myGyro];
		moveRobot(60 - 0.4*Err,60 + 0.4*Err);
	}
	moveRobot(0,0);
	motorReq[MobGoalntake] = 0;
	clearTimer(T2);
	while(time1(T2) < 1500)
	{
		motorReq[MobGoalntake] = 127;
	}
	//g2 on robot
	motorReq[MobGoalntake] = 0;
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] > -400)
	{
		moveRobot(-50,-50);
		turnRollerToEject();
	}
	motorReq[rollerMotor] = 0;
	moveRobot(0,0);
	//going to score g2
	clearTimer(T2);
	while(SensorValue[myGyro] < 1200)
	{
		moveRobot(60,-60);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 800)
	{
		Err = SensorValue[myGyro] - 1350;
		moveRobot(-0.6*Err,0.6*Err);
	}
	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	while(true)
	{
		Err = SensorValue[myGyro] - 1350;
		moveRobot(60-0.5*Err,60+0.5*Err);
		if(nMotorEncoder[frontRight2] > 400)
		{
			motorReq[MobGoalntake] = -127;
		}
		if(nMotorEncoder[frontRight2] > 1400 || (SensorValue[backLim] < 1800 && nMotorEncoder[frontRight2] > 600))
		{
			motorReq[MobGoalntake] = 0;
			moveRobot(0,0);
			break;
		}
	}
	clearTimer(T2);
	while(time1(T2) < 100)
	{
		moveRobot(-50,-50);
	}
	moveRobot(0,0);
	SensorValue[myGyro] = 0;
	//clearTimer(T1);
	//while(time1(T1) < 800)
	//{
	//	motorReq[MobGoalntake] = -127;
	//}
	motorReq[MobGoalntake] = 0;
	nMotorEncoder[frontRight2] = 0;
	SensorValue[myGyro] = 0;
	//g2 scored
	clearTimer(T1);
	while(time1(T1) < 150)
	{
		motorReq[MobGoalntake] = 127;
		moveRobot(-127,-127);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 700)
	{
		motorReq[MobGoalntake] = 127;
		moveRobot(-100,-20);
	}
	moveRobot(0,0);
	//going to g3
	motorReq[MobGoalntake] = 0;
	while(true)
	{
		Err = SensorValue[myGyro];
		moveRobot(-60-0.5*Err,-60 +0.5*Err);
		if(SensorValue[backLim2] < 1800)
		{
			moveRobot(0,0);
			break;
		}
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1)<100)
	{
		moveRobot(50,50);
	}
	moveRobot(0,0);
	// turn to get g3
	while(SensorValue[myGyro] > -700)
	{
		moveRobot(-50,50);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 800)
	{
		motorReq[MobGoalntake] = -127;
		Err = SensorValue[myGyro] + 900;
		moveRobot(-0.5*Err,0.5*Err);
	}
	//going to g3
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] < 900)
	{
		if(nMotorEncoder[frontRight2] <200)
		{
			motorReq[MobGoalntake] = -127;
		}
		else
		{
			motorReq[MobGoalntake] = 0;
		}
		Err = SensorValue[myGyro] + 900;
		moveRobot(60-0.5*Err,60+0.5*Err);
	}
	moveRobot(0,0);

	clearTimer(T2);
	while(time1(T2) < 1250)
	{
		motorReq[MobGoalntake] = 127;
	}
	motorReq[MobGoalntake] = 0;
	//new add

	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] > -400)
	{
		Err = SensorValue[myGyro] + 900;
		moveRobot(-60-0.5*Err,-60+0.5*Err);
	}
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] < 600)
	{
		Err = SensorValue[myGyro] + 1300;
		moveRobot(60-0.5*Err,60+0.5*Err);
	}
	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] > -300)
	{
		Err = SensorValue[myGyro] + 1350;
		moveRobot(-60-0.5*Err,-60+0.5*Err);
	}
	moveRobot(0,0);
	//end new add
	//g3 on robot
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2]>-200)
	{
		moveRobot(-50,-50);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1)<100)
	{
		moveRobot(50,50);
	}
	moveRobot(0,0);
	//nMotorEncoder[frontRight2] = 0;
	//while(nMotorEncoder[frontRight2] > -1400)
	//{
	//	Err = SensorValue[myGyro] + 900;
	//	moveRobot(-60-0.5*Err,-60+0.5*Err);
	//}
	while(SensorValue[myGyro] < -100)
	{
		moveRobot(60,-60);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 800)
	{
		Err = SensorValue[myGyro];
		moveRobot(-0.5*Err,0.5*Err);
	}
	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	while(true)
	{
		Err = SensorValue[myGyro];
		moveRobot(70-0.5*Err,70+0.5*Err);
		if(nMotorEncoder[frontRight2] > 1000)
		{
			motorReq[MobGoalntake] = -127;
		}
		if(nMotorEncoder[frontRight2] > 1700 || (SensorValue[backLim] < 1800 && nMotorEncoder[frontRight2] > 1000))
		{
			motorReq[MobGoalntake] = 0;
			moveRobot(0,0);
			break;
		}

	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 100)
	{
		moveRobot(-50,-50);
	}
	moveRobot(0,0);
	//clearTimer(T2);
	//while(time1(T2) < 1000)
	//{
	//	motorReq[MobGoalntake] = -127;
	//}
	//g3 scored
	motorReq[MobGoalntake]= 0;
	moveRobot(0,0);
	//start pushing cones back
	SensorValue[myGyro] = 0;
	nMotorEncoder[frontRight2] = 0;
	while(abs(nMotorEncoder[frontRight2]) < 200)
	{
		Err = SensorValue[myGyro] ;
		moveRobot(-70-0.5*Err,-70+0.5*Err);
	}
	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	while(abs(nMotorEncoder[frontRight2]) < 2600)
	{
		Err = SensorValue[myGyro] - 180;
		moveRobot(-70-0.5*Err,-70+0.5*Err);
	}
	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	while(abs(nMotorEncoder[frontRight2]) < 0)
	{
		Err = SensorValue[myGyro];
		moveRobot(-70-0.5*Err,-70+0.5*Err);
	}
	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	while(abs(nMotorEncoder[frontRight2]) < 0)
	{
		Err = SensorValue[myGyro]-50;
		moveRobot(-70-0.5*Err,-70+0.5*Err);
	}
	moveRobot(0,0);
	while(SensorValue[myGyro] < 100)
	{
		moveRobot(70,-70);
	}
	clearTimer(T2);
	while(time1(T2) < 600)
	{
		motorReq[MobGoalntake] = -127;
		Err = SensorValue[myGyro] - 450;
		moveRobot(-0.5*Err,0.5*Err);
	}
	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] < 800)
	{
		Err = SensorValue[myGyro] - 480;
		moveRobot(70-0.5*Err,70+0.5*Err);
	}
	moveRobot(0,0);

	clearTimer(T2);
	while(time1(T2) < 1250)
	{
		motorReq[MobGoalntake] = 127;
	}
	motorReq[MobGoalntake] = 0;
	nMotorEncoder[frontRight2] = 0;
	while(abs(nMotorEncoder[frontRight2]) < 500)
	{
		Err = SensorValue[myGyro] - 450;
		moveRobot(-70-0.5*Err,-70+0.5*Err);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 100)
	{
		moveRobot(50,50);
	}
	moveRobot(0,0);
	while(SensorValue[myGyro] < 1200)
	{
		moveRobot(70,-70);
	}
	clearTimer(T1);
	while(time1(T1) < 800)
	{
		Err = SensorValue[myGyro]-1350;
		moveRobot(-0.5*Err,0.5*Err);
	}
	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] < 1000)
	{
		Err = SensorValue[myGyro] - 1350;
		moveRobot(70-0.5*Err,70+0.5*Err);
	}
	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	clearTimer(T1);

	while(time1(T1) < 1000)
	{
		Err = SensorValue[myGyro] - 1950;
		moveRobot(-0.5*Err,0.5*Err);
	}
	moveRobot(0,0);
	clearTimer(T2);
	while(time1(T2) < 300){}
	clearTimer(T1);
	while(time1(T1) < 900)
	{
		moveRobot(127,127);
		if(time1(T1) > 500)
		{
			motorReq[MobGoalntake] = -127;
		}
	}
	motorReq[MobGoalntake]  = 0;
	moveRobot(0,0);
	//moveCraneWith(0);
	//clearTimer(T1);
	//while(time1(T1) < 300)
	//{
	//	moveRobot(-70,-70);
	//}
	//moveRobot(0,0);
	//clearTimer(T1);
	//while(time1(T1) < 400)
	//{
	//	moveRobot(70,70);
	//}
	//moveRobot(0,0);

	clearTimer(T1);
	while(time1(T1) < 600)
	{
		motorReq[MobGoalntake] = 127;
	}
	motorReq[MobGoalntake] = 0;
	nMotorEncoder[frontRight2] = 0;
	while(abs(nMotorEncoder[frontRight2]) < 500 || time1(T1) < 200)
	{
		moveRobot(-50,-50);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 100)
	{
		moveRobot(50,50);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 500)
	{
		//motorReq[MobGoalntake] = 127;
	}
	motorReq[MobGoalntake] = 0;
	SensorValue[myGyro] = 0;



	SensorValue[myGyro] = 0;
	while(SensorValue[myGyro] < 600)
	{
		moveRobot(50,-50);
	}
	moveRobot(0,0);
	while(SensorValue[AdjtSwitch2] == 0)
	{
		Err = SensorValue[myGyro] - 900;
		moveRobot(-40 - 0.3*Err, -40 + 0.3*Err);
		motorReq[MobGoalntake] = -127;
	}
	motorReq[MobGoalntake] = 0;
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 200)
	{
		moveRobot(50,50);
	}
	moveRobot(0,0);
	while(SensorValue[myGyro] < 1350)
	{
		moveRobot(0,-70);
	}
	moveRobot(0,0);
	clearTimer(T2);
	while(true)
	{
		moveRobot(-40,-40);
		if(time1(T2) > 250 || (SensorValue[LeftBump] == 1 && SensorValue[RightBump] == 1))
		{
			break;
		}
	}
	moveRobot(0,0);
	SensorValue[myGyro] = 0;
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] < 50)
	{
		Err = SensorValue[myGyro];
		//Err = SensorValue[myGyro];
		moveRobot(50 - 0.5*Err,50 + 0.5*Err);
	}
	moveRobot(0,0);

	nMotorEncoder[frontRight2] = 0;
	clearTimer(T2);
	while(nMotorEncoder[frontRight2] < 1050)
	{
		if(time1(T2) < 100)
		{
			motorReq[MobGoalntake] = -127;
		}
		else
		{
			motorReq[MobGoalntake] = -50;
		}
		Err = SensorValue[myGyro] - 250;
		//Err = SensorValue[myGyro];
		moveRobot(60 - 0.4*Err,60 + 0.4*Err);

	}
	moveRobot(0,0);
	//motorReq[MobGoalntake] = 0;

	//while(SensorValue[backLim] > 1800)
	//{
	//	moveRobot(-50,-50);
	//}
	//moveRobot(0,0);
	//while(SensorValue[myGyro] > 600)
	//{
	//	moveRobot(70,20);
	//}
	//moveRobot(0,0);
	//while(SensorValue[AdjtSwitch] == 0)
	//{
	//	Err = SensorValue[myGyro] - 450;
	//	moveRobot(-40 - 0.3*Err, -40 + 0.3*Err);
	//}
	//moveRobot(0,0);
	//clearTimer(T1);
	//while(time1(T1) < 100)
	//{
	//	moveRobot(50,50);
	//}
	//moveRobot(0,0);
	//while(SensorValue[myGyro] > 0)
	//{
	//	moveRobot(-70,0);
	//}
	//moveRobot(0,0);
	//clearTimer(T2);
	//while(true)
	//{
	//	moveRobot(-40,-40);
	//	if(time1(T2) > 250)
	//	{
	//		break;
	//	}
	//}
	//moveRobot(0,0);


	clearTimer(T2);
	while(time1(T2) < 1250)
	{
		motorReq[MobGoalntake] = 127;
	}
	motorReq[MobGoalntake] = 0;
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] > -100)
	{
		moveRobot(-50,-50);
	}
	moveRobot(0,0);
	clearTimer(T2);
	while(SensorValue[myGyro] > -1200)
	{
		moveRobot(-70,70);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 800)
	{
		Err = SensorValue[myGyro] + 1350;
		moveRobot(-0.6*Err,0.6*Err);
	}
	moveRobot(0,0);
	nMotorEncoder[frontRight2] = 0;
	while(true)
	{
		Err = SensorValue[myGyro] + 1350;
		moveRobot(100-0.5*Err,100+0.5*Err);
		if(nMotorEncoder[frontRight2] > 500)
		{
			motorReq[MobGoalntake] = -127;
		}
		if(nMotorEncoder[frontRight2] > 1400 || (SensorValue[frontLim] < 1800 && nMotorEncoder[frontRight2] > 600))
		{
			moveRobot(0,0);
			break;
		}
	}
	nMotorEncoder[frontRight2] = 0;
	while(nMotorEncoder[frontRight2] < 200)
	{
		moveRobot(100,100);
	}
	moveRobot(0,0);
	clearTimer(T1);
	while(time1(T1) < 300)
	{
		moveRobot(-127,-127);
	}
	moveRobot(0,0);
	motorReq[MobGoalntake] = 0;

	// go to park
	nMotorEncoder[frontRight2] = 0;
	while(abs(nMotorEncoder[frontRight2]) < 800)
	{
		Err = SensorValue[myGyro] + 700;
		moveRobot(-60 -0.5*Err,-60+0.5*Err);
	}
	moveRobot(0,0);
	//SensorValue[myGyro] = 0;
	//clearTimer(T1);
	//while(time1(T1) < 800)
	//{
	//	motorReq[MobGoalntake] = -127;
	//}
	//motorReq[MobGoalntake] = 0;
	//nMotorEncoder[frontRight2] = 0;
	//clearTimer(T1);
	//while(time1(T1)<500)
	//{
	//	moveRobot(-127,-127);
	//}
	//moveRobot(0,0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// 						D R I V E R    S K I L L S							//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	nMotorEncoder[frontRight2] = 0;
	startTask(MotorSlewRateTask);
	while (true)
	{
		test = SensorValue[backLim2];
		if(vexRT[Btn5D])
		{
			turnRollerToEject();
		}
		else
		{
			motorReq[rollerMotor] = 0;
		}
		if(vexRT[Btn6U] && !vexRT[Btn6D])
		{
			MobInUp = true;
			MobInDown = false;

		}
		if(vexRT[Btn6D] && !vexRT[Btn6U])
		{
			MobInUp = false;
			MobInDown = true;
		}
		if(!vexRT[Btn6D] && !vexRT[Btn6U])
		{
			MobInUp = false;
			MobInDown = false;
		}
		if(MobInUp && !MobInDown)
		{
			motorReq[MobGoalntake] = 127;
		}
		else if(!MobInUp && MobInDown)
		{
			motorReq[MobGoalntake] = -127;
		}
		else
		{
			motorReq[MobGoalntake] = 0;
		}
		int a = vexRT[Ch2];
		int b = vexRT[Ch3];
		if(abs(a) < 15){a = 0;}
		if(abs(b) < 15){b = 0;}
		if(a*b < 0){a = 4*a/5;b = 4*b/5;}
		moveRobot(a,b);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void moveClawsTo(int clawsTar)
{
	int Err = SensorValue[clawsPot] - clawsTar;
	moveClawsWith(0.1*Err);
}
void moveClawsWith(int speed)
{
	motorReq[rightClaws] = motorReq[leftClaws] = speed;
}
void moveCraneWith(int speed)
{
	motorReq[craneMotorsExp]= speed;
}
void moveCraneTo(int craneTar)
{
	int Err = -SensorValue[craneEnc] + craneTar;
	moveCraneWith(1.2*Err + 0.00001*IErr);
	if(abs(1.2*Err + 0.00001*IErr) < MAX_CMD)
	{
		IErr = IErr + Err;
	}
}
void moveRobot(int RSpeed, int LSpeed)
{
	motorReq[frontLeftExp] = LSpeed;
	motorReq[backLeft] = LSpeed;
	motorReq[frontRight1] = RSpeed;
	motorReq[frontRight2] = RSpeed;
	motorReq[backRight] = RSpeed;
}
void turnRollerToEat()
{
	motorReq[rollerMotor] = 127;
}
void turnRollerToEject()
{
	motorReq[rollerMotor] = -127;
}