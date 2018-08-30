#pragma config(Sensor, dgtl1, fireRelay, sensorDigitalOut)
#pragma config(Motor, port2, rightDriveMotor, tmotorServoContinuousRotation, openLoop, driveRight)
#pragma config(Motor, port3, leftDriveMotor, tmotorServoContinuousRotation, openLoop, driveLeft)
#pragma config(Motor, port4, armMotor, tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard !!*//

int RELAY_DELAY = 100; // milliseconds
const int FIRE_CONFIRM_DELAY = 1000; // milliseconds
const int DB_THRESH = 10;

int fireConfirmTimer;
int fireRelayTimer;
bool fireConfirmTimerRunning = false;
bool fireRelayTimerRunning = false;

float squareInput(float input) {
	return input * abs(input) / 100;
}

float deadband(float input) {
	return (abs(input) >= DB_THRESH) ? input : 0;
}

task main()
{
	while (true) {
	  /**
		 * Open loop arcade drive.
		 */
		float throttle = vexRT[Ch3];
		float turn = vexRT[Ch1];
		int slowturn = vexRT[Btn6U];

		throttle = deadband(throttle);
		turn = deadband(turn);

		if (slowturn == 1) {
			turn /= 2;
		}

		throttle = squareInput(throttle);
		turn = squareInput(turn);

		if (fireConfirmTimerRunning == false) {
			motor[rightDriveMotor] = throttle + turn;
			motor[leftDriveMotor] = throttle - turn;
		}
		else {
			motor[rightDriveMotor] = 0.0;
			motor[leftDriveMotor] = 0.0;
		}

		/**
		 * Open loop arm control.
		 */
		int upBtn = vexRT[Btn5U]; // boolean, 0 or 1
		int downBtn = vexRT[Btn5D]; // boolean, 0 or 1

		if (upBtn == 1) {
			motor[armMotor] = 0.5;
		}
		else if (downBtn == 1) {
			motor[armMotor] = -0.5;
		}
		else {
			motor[armMotor] = 0.0;
		}

		/**
		 * Cannon control.
		 */
		int fireBtn = vexRT[Btn6D];

		if (vexRT[Btn7U] == 1) { RELAY_DELAY = 100; }
		if (vexRT[Btn7R] == 1) { RELAY_DELAY = 200; }
		if (vexRT[Btn7D] == 1) { RELAY_DELAY = 300; }
		if (vexRT[Btn7L] == 1) { RELAY_DELAY = 400; }
		if (vexRT[Btn8U] == 1) { RELAY_DELAY = 500; }
		if (vexRT[Btn8R] == 1) { RELAY_DELAY = 600; }
		if (vexRT[Btn8D] == 1) { RELAY_DELAY = 700; }
		if (vexRT[Btn8U] == 1) { RELAY_DELAY = 800; }

		if (fireBtn == 1 && fireConfirmTimerRunning == false) {
			clearTimer(T1);
			fireConfirmTimer = time1[T1];
			fireConfirmTimerRunning = true;
		}
		else if (fireBtn == 0 && fireConfirmTimerRunning == true && fireRelayTimerRunning == false) {
			fireConfirmTimerRunning = false;
		}

		if (fireConfirmTimer >= FIRE_CONFIRM_DELAY && fireConfirmTimerRunning == true) {
			clearTimer(T2);
			fireRelayTimer = time1(T2);
			fireRelayTimerRunning = true;
			SensorValue[fireRelay] = 1;

			if (fireRelayTimer >= RELAY_DELAY) {
				SensorValue[fireRelay] = 0;
				fireRelayTimerRunning = false;
				fireConfirmTimerRunning = false;
			}
		}
	}
}
