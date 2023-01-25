#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <Servo.h> // Library for motor

Servo motor1;
Servo motor2;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

const int motor1Pin = 9;
const int motor2Pin = 10;

const int potent1Pin = A0;
const int potent2Pin = A1;

bool centered1 = false;
bool centered2 = false;

const int midValue = 1500;

// options
int deadzone = 25;
bool forceCenter = true;



void setup() {

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  
  pinMode(potent1Pin, INPUT);
  pinMode(potent2Pin, INPUT);

  motor1.attach(motor1Pin);
  motor2.attach(motor2Pin);

  if (forceCenter == true) {
    centerStartup();
  }
  
}


void loop() {
  

  int speed1 = getSpeed(1);
  int speed2 = getSpeed(2);
    
  displaySpeed(speed1, speed2);
  drive(speed1, speed2);


}

bool centeredCheck(int motor) {

  int speedCheck = getSpeed(motor);
  bool centered;

  if (speedCheck >= midValue - deadzone && speedCheck <= midValue + deadzone) {

    if (motor == 1) {

      centered1 = true;
      
    } 
    
    else {
      
      centered1 = false;

    }

    if (motor == 2) {

      centered2 = true;
      
    }

    else {

      centered2 = false;
    }
    
  }
  
}


void drive(int motor1Speed, int motor2Speed) {
  
  motor1.writeMicroseconds(motor1Speed);
  motor2.writeMicroseconds(motor2Speed);
  
  delay(100); 
  
}

int getSpeed(int motor) {

  int calibratedSpeed;

  if (motor == 1) {
    
    calibratedSpeed = map(analogRead(potent1Pin), 0, 1023, 1000, 2000);
    
  } else if (motor == 2) {

    calibratedSpeed = map(analogRead(potent2Pin), 0, 1023, 1000, 2000);
  }
  

  if (calibratedSpeed >= midValue - deadzone && calibratedSpeed <= midValue + deadzone) {

    calibratedSpeed = 1500;
  }

  
  return calibratedSpeed;
  
}


void centerStartup() {

  centered1 = false;
  centered2 = false;

  while (centered1 == false) {

    centeredCheck(1);
    displayInstruction(1);
    
  }
  lcd.clear();

  while (centered2 == false) {

    centeredCheck(2);
    displayInstruction(2);
    
  }
  lcd.clear();
  
}


void displaySpeed(int motor1Speed, int motor2Speed) {
  
  lcd.setCursor(0, 0);
  lcd.print("MOTOR 1:");
  
  lcd.setCursor(9, 0);
  lcd.print(motor1Speed);

  lcd.setCursor(0, 1);
  lcd.print("MOTOR 2:");
  
  lcd.setCursor(9, 1);
  lcd.print(motor2Speed);
  
}

void displayInstruction(int motor) {
 
  lcd.setCursor(5, 0);
  lcd.print("CENTER");
  
  lcd.setCursor(0, 1);
  lcd.print("POTENTIOMETER " + String(motor));
  
}
