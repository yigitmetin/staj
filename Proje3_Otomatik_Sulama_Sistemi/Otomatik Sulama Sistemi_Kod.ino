const int PIN_SOIL = A0;
const int PIN_COIL = 7;     

const bool COIL_ACTIVE_HIGH = true;

int DRY_THRESHOLD = 620;
int HYSTERESIS    = 40;

const unsigned long MIN_ON_MS  = 4000;
const unsigned long MIN_OFF_MS = 4000;

bool pumpOn = false;
unsigned long lastToggleMs = 0;

void setup() {
  pinMode(PIN_COIL, OUTPUT);
  pumpOff();
  Serial.begin(9600);
  delay(300);
}

void loop() {
  int raw = analogRead(PIN_SOIL);
  Serial.print("Soil raw: "); Serial.println(raw);

  int onTh  = DRY_THRESHOLD;
  int offTh = DRY_THRESHOLD - HYSTERESIS;

  unsigned long now = millis();

  if (!pumpOn && raw >= onTh && (now - lastToggleMs) >= MIN_OFF_MS) {
    pumpOn = true;
    pumpOnFn();
    lastToggleMs = now;
    Serial.println(">>> POMPA AÇILDI");
  }
  else if (pumpOn && raw <= offTh && (now - lastToggleMs) >= MIN_ON_MS) {
    pumpOn = false;
    pumpOff();
    lastToggleMs = now;
    Serial.println("<<< POMPA KAPANDI");
  }

  delay(500);
}

void pumpOnFn() {
  if (COIL_ACTIVE_HIGH) digitalWrite(PIN_COIL, HIGH);
  else                  digitalWrite(PIN_COIL, LOW);
}

void pumpOff() {
  if (COIL_ACTIVE_HIGH) digitalWrite(PIN_COIL, LOW);
  else                  digitalWrite(PIN_COIL, HIGH);
}
