/*
  FRC Motor Controller Testbed

  Written by Samuel Moore (Alumni Team 1714)

  This project serves as a prototyping platform to test many
  different FRC mechanisms. It can run up to four motors and
  one pneumatic solenoid simultaniously, all without the
  user having to write any code for their mechanisms. Each
  motor is controlled via a potentiometer, and their speeds
  are displayed on an I2C LiquidCrystal display.

*/

// Required libraries
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <Servo.h> // Library for motors
/*
  Motor object declaration

  NOTE: FRC motor controllers require a 50Hz PWM signal.
  This can be set manualy, but I've found it easier
  to set the motor controller PWM signals as Servo
  objects, as most servos also use a 50Hz PWM signal.
  
*/
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

// Display object declaration
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

// Motor pins
const int motor1Pin = 11;
const int motor2Pin = 6;
const int motor3Pin = 5;
const int motor4Pin = 3;

// Potentiometer pins
const int potent1Pin = A0;
const int potent2Pin = A1;
const int potent3Pin = A2;
const int potent4Pin = A3;


/*
  Centered variables

  These are global variables that represent the state of each potentiometer
  true = the potentiometer is in the center position
  false = the potentiometer is not in the center position
*/
bool centered1 = false;
bool centered2 = false;
bool centered3 = false;
bool centered4 = false;

/*
  Potentiometer centered value

  This is the value that the potentiometer should output when in the centered position
*/
const int midValue = 1500;

// potentiometer deadzone value
int deadzone = 25;

// whether or not to force all potentiometers to be centered on startup before motors can be used
bool forceCenter = true;


// run once on startup
void setup() {

  // setup serial
  Serial.begin(9600);

  // setup display
  lcd.init();
  lcd.backlight();
  
  // set motor pins
  pinMode(potent1Pin, INPUT);
  pinMode(potent2Pin, INPUT);
  pinMode(potent3Pin, INPUT);
  pinMode(potent4Pin, INPUT);

  // attach motor pins to motor objects
  motor1.attach(motor1Pin);
  motor2.attach(motor2Pin);
  motor3.attach(motor3Pin);
  motor4.attach(motor4Pin);

  // force potentiometers to all be centered before moving motors
  if (forceCenter == true) {
    centerStartup();
  }
}

// run periodically
void loop() {

  // get motor speeds
  int motor1Speed = getSpeed(0);
  int motor2Speed = getSpeed(1);
  int motor3Speed = getSpeed(2);
  int motor4Speed = getSpeed(3);
  
  // display motor speeds
  displaySpeed(motor1Speed, motor2Speed, motor3Speed, motor4Speed);

  // run motors at inputed speeds
  drive(motor1Speed, motor2Speed, motor3Speed, motor4Speed);

}





/*
  This function will read a potentiometer and map it between the motor controller PWM range with the deadband.

  Inputs:

    potentPin (int)
    The pin number of the potentiometer

  Outputs:

    motorSpeed (int)
    The Pulse Width Modulation (PWM) value cooresponding to the potentiometer value
*/
int getSpeed(int potentPin) {

  // map potentiometer value in PWM range
  int calibratedSpeed = map(analogRead(potentPin), 0, 1023, 1000, 2000);

  // apply deadband
  if (calibratedSpeed >= midValue - deadzone && calibratedSpeed <= midValue + deadzone) {
    calibratedSpeed = midValue;
  }

  return calibratedSpeed;
  
}

/*
  This function will drive all four motors at a desired speed.

  Inputs:

    motor1Speed (int)
    PWM value sent to motor 1

    motor2Speed (int)
    PWM value sent to motor 2

    motor3Speed (int)
    PWM value sent to motor 3

    motor4Speed (int)
    PWM value sent to motor 4

  Outputs:

    none
  
*/
void drive(int motor1Speed, int motor2Speed, int motor3Speed, int motor4Speed) {
  
  // drive motors
  motor1.writeMicroseconds(motor1Speed);
  motor2.writeMicroseconds(motor2Speed);
  motor3.writeMicroseconds(motor3Speed);
  motor4.writeMicroseconds(motor4Speed);
  
  // delay to keep stable PWM signal
  //delay(100); 
  
}

/*
  This function will check to see if a potentiometer is in the center value.

  Inputs:

    potentPin (int)
    The pin number of the potentiometer

  Outputs:

    centered (bool)
    true = potentiometer is centered
    false = potentiometer is not centered

*/
bool centeredCheck(int potentPin) {

  // get potentiometer value
  int speedCheck = getSpeed(potentPin);

  // return if potentiometer is centered
  return (speedCheck >= midValue - deadzone && speedCheck <= midValue + deadzone);
  
}

/*
  This function will display the inputed PWM signal value sent to each motor.

  Inputs:

    motor1Speed (int)
    PWM value sent to motor 1

    motor2Speed (int)
    PWM value sent to motor 2

    motor3Speed (int)
    PWM value sent to motor 3

    motor4Speed (int)
    PWM value sent to motor 4

  Outputs:

    none

*/
void displaySpeed(int motor1Speed, int motor2Speed, int motor3Speed, int motor4Speed) {

  // Map the values from 1000-2000 to -100 to 100
  int mappedValue1 = map(motor1Speed, 1000, 2000, -100, 100);
  int mappedValue2 = map(motor2Speed, 1000, 2000, -100, 100);
  int mappedValue3 = map(motor3Speed, 1000, 2000, -100, 100);
  int mappedValue4 = map(motor4Speed, 1000, 2000, -100, 100);


  // Display value 1 in the first quadrant (top-left)
  lcd.setCursor(0, 0);
  lcd.print("M1:      ");
  lcd.setCursor(4, 0);
  lcd.print(mappedValue1);

  // Display value 2 in the second quadrant (top-right)
  lcd.setCursor(8, 0);
  lcd.print("M2:      ");
  lcd.setCursor(12, 0);
  lcd.print(mappedValue2);

  // Display value 3 in the third quadrant (bottom-left)
  lcd.setCursor(0, 1);
  lcd.print("M3:      ");
  lcd.setCursor(4, 1);
  lcd.print(mappedValue3);

  // Display value 4 in the fourth quadrant (bottom-right)
  lcd.setCursor(8, 1);
  lcd.print("M4:      ");
  lcd.setCursor(12, 1);
  lcd.print(mappedValue4);

}

/*
  This function will instruct the user to center a specific potentiometer.

  Inputs:
    potentPin (int)
    The pin number of the potentiometer

  Outputs:
    
    none

*/
void displayInstruction(int potentPin) {
 
  lcd.setCursor(5, 0);
  lcd.print("CENTER");
  
  lcd.setCursor(0, 1);
  lcd.print("POTENTIOMETER " + String(potentPin));
  
}

/* This function will instruct the user to center each potentiometer until all potentiometers are in the centered position.

  Inputs:
    
    none

  Outputs:
    
    none

*/
void centerStartup() {

  while (centeredCheck(0) == false) {
    displayInstruction(1);  
  }
  lcd.clear();

  while (centeredCheck(1) == false) {
    displayInstruction(2);
  }
  lcd.clear();

  while (centeredCheck(2) == false) {
    displayInstruction(3);  
  }
  lcd.clear();

  while (centeredCheck(3) == false) {
    displayInstruction(4);
  }
  lcd.clear();
  
}
