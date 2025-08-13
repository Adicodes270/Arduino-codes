#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 11;

Servo servo;
int servoPos = 0;
bool sweepForward = true;
int sweepDelay = 30; // Adjust this to change speed

void setup() {
  lcd.init();
  lcd.backlight();
  servo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  servo.write(servoPos);
  int distance = getDistance();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Angle: "); lcd.print(servoPos); lcd.print((char)223);
  lcd.setCursor(0, 1);
  if (distance == -1) lcd.print("No Obj");
  else lcd.print("Dist: "); lcd.print(distance); lcd.print(" cm");

  // Sweep
  if (sweepForward) {
    servoPos++;
    if (servoPos >= 180) sweepForward = false;
  } else {
    servoPos--;
    if (servoPos <= 0) sweepForward = true;
  }
  delay(sweepDelay);
}

int getDistance() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 20000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}
