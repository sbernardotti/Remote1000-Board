// Remote1000 Board v1.0
// https://github.com/sbernardotti/Remote1000-Board

#define INNER_A_PIN 4
#define INNER_B_PIN 5
#define SW_PIN 6
#define OUTER_A_PIN 7
#define OUTER_B_PIN 8

#define SW_INTERVAL 250

void updateOuterEncoder() {
  static unsigned char oldAB = 3;
  static char encVal = 0;
  static const char encStates[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

  oldAB <<= 2;

  if (digitalRead(OUTER_A_PIN)) oldAB |= 0x02;
  if (digitalRead(OUTER_B_PIN)) oldAB |= 0x01;

  encVal += encStates[(oldAB & 0x0f)];

  if (encVal > 3) {
    Serial.write("l");
    encVal = 0;
  } else if (encVal < -3) {
    Serial.write("r");
    encVal = 0;
  }
}

void updateInnerEncoder() {
  static unsigned char oldAB = 3;
  static char encVal = 0;
  static const char encStates[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

  oldAB <<= 2;

  if (digitalRead(INNER_A_PIN)) oldAB |= 0x02;
  if (digitalRead(INNER_B_PIN)) oldAB |= 0x01;

  encVal += encStates[(oldAB & 0x0f)];

  if (encVal > 3) {
    Serial.write("R");
    encVal = 0;
  } else if (encVal < -3) {
    Serial.write("L");
    encVal = 0;
  }
}

void updateSwitch() {
  static int oldState = 0;
  static unsigned long lastUpdate = millis();
  int state = digitalRead(SW_PIN);
  if (state != oldState) {
    unsigned long now = millis();
    oldState = state;
    if (state == LOW && now - lastUpdate >= SW_INTERVAL) {
      Serial.write("p");
    }
    lastUpdate = now;
  }
}

void setup() {
  pinMode(OUTER_A_PIN, INPUT_PULLUP);
  pinMode(OUTER_B_PIN, INPUT_PULLUP);
  pinMode(INNER_A_PIN, INPUT_PULLUP);
  pinMode(INNER_B_PIN, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  updateOuterEncoder();
  updateInnerEncoder();
  updateSwitch();
}
