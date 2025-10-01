#include <Keypad.h>
#include <Servo.h>

Servo door;

const byte ROWS = 4;
const byte COLS = 4; 

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int PIN_SERVO  = 10;
const int PIN_BUZZER = 11;
const int PIN_GREEN  = 12;
const int PIN_RED    = 13;

const int DOOR_CLOSED = 0;  
const int DOOR_OPEN   = 90;
const unsigned long OPEN_MS = 3000;


String PASS = "1234";
String entered = "";

int wrongCount = 0;
const int MAX_WRONG = 3;
unsigned long lockUntil = 60000;


void beep(int f, int ms) { tone(PIN_BUZZER, f, ms); delay(ms + 20); }

void grantAccess() {
  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_GREEN, HIGH);
  beep(1500, 80);
  door.write(DOOR_OPEN);
  unsigned long t0 = millis();
  while (millis() - t0 < OPEN_MS) {}
  door.write(DOOR_CLOSED);
  digitalWrite(PIN_GREEN, LOW);
}

void denyAccess() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_RED, HIGH);
    beep(700, 120);
    digitalWrite(PIN_RED, LOW);
    delay(120);
  }
}

void setup() {
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_RED, OUTPUT);

  door.attach(PIN_SERVO);
  door.write(DOOR_CLOSED);

  // açılış sinyali
  digitalWrite(PIN_GREEN, HIGH); beep(1800, 80); digitalWrite(PIN_GREEN, LOW);
}

void loop() {
  if (millis() < lockUntil) {
    digitalWrite(PIN_RED, HIGH);
    char k = keypad.getKey();
    if (k) beep(400, 40);
    return;
  } else {
    digitalWrite(PIN_RED, LOW);
  }

  char key = keypad.getKey();
  if (!key) return;

  if (key >= '0' && key <= '9') {
    if (entered.length() < 8) {
      entered += key;
      beep(1200, 40);
    }
  } else if (key == '*') {
    entered = "";
    beep(1000, 60);
  } else if (key == '#') {
    if (entered == PASS) {
      grantAccess();
      entered = "";
      wrongCount = 0;
    } else {
      wrongCount++;
      denyAccess();
      entered = "";
      if (wrongCount >= MAX_WRONG) {
        lockUntil = millis() + 10000UL;
        wrongCount = 0;
      }
    }
  } else if (key == 'A') {
    beep(1500, 30);
  } else if (key == 'B') {
    entered = "";
    beep(900, 30);
  }
}


