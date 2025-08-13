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

byte bar[8] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, bar);
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
  if (distance == -1) {
    lcd.print("No Obj");
  } else {
    int bars = map(distance, 0, 50, 10, 0);
    for (int i = 0; i < bars; i++) lcd.write(byte(0));
  }

  // Sweep
  if (sweepForward) {
    servoPos++;
    if (servoPos >= 180) sweepForward = false;
  } else {
    servoPos--;
    if (servoPos <= 0) sweepForward = true;
  }
  delay(30);
}

int getDistance() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 20000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}
