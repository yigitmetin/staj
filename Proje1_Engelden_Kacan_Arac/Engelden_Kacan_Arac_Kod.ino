#include <Servo.h>

Servo servo;
int trigPin = 9;
int echoPin = 10;
int servoPin = 11;

int motor1A = 5;
int motor1B = 6;
int motor2A = 3;
int motor2B = 4;

long duration;
int distance;

void setup() {
  servo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);

  servo.write(90);
  delay(500);
}

void loop() {
  distance = getDistance();
  if (distance > 20) {
    ileri();
  } else {
    dur();
    servo.write(30);
    delay(500);
    int leftDist = getDistance();
    
    servo.write(150);
    delay(500);
    int rightDist = getDistance();
    
    servo.write(90);

    if (leftDist > rightDist) {
      solaDon();
    } else {
      sagaDon();
    }
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int cm = duration * 0.034 / 2;
  return cm;
}

void ileri() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void dur() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, LOW);
}

void solaDon() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
  delay(400);
  dur();
}

void sagaDon() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
  delay(400);
  dur();
}
