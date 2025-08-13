#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 11;

Servo servo;
const int servoSpeed = 120;
const unsigned long rotationDuration = 2000;
unsigned long rotationStartTime;

void setup() {
  lcd.init();
  lcd.backlight();
  servo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.write(servoSpeed);
  rotationStartTime = millis();
}

void loop() {
  unsigned long elapsed = (millis() - rotationStartTime) % rotationDuration;
  int estimatedAngle = map(elapsed, 0, rotationDuration, 0, 360);
  int distance = getDistance();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Angle: "); lcd.print(estimatedAngle); lcd.print((char)223);

  lcd.setCursor(12, 0);
  if (servoSpeed > 90) lcd.print("CW ");
  else if (servoSpeed < 90) lcd.print("CCW");
  else lcd.print("STP");

  lcd.setCursor(0, 1);
  lcd.print("Dist: ");
  if (distance == -1) {
    lcd.print("No Obj");
  } else {
    lcd.print(distance); lcd.print(" cm");
  }

  delay(100);
}

int getDistance() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 20000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}
