#define TRIG_PIN 5
#define ECHO_PIN 18
#define RELAY_PIN 23
#define MODE_PIN 19
#define MANUAL_PIN 21
#define LED_PIN 22

const float EMPTY_DISTANCE = 100.0;
const float FULL_DISTANCE = 10.0;

const float MOTOR_ON_LEVEL = 20.0;
const float MOTOR_OFF_LEVEL = 90.0;

bool motorState = false;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MODE_PIN, INPUT);
  pinMode(MANUAL_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println("SMART WATER TANK CONTROLLER");
  Serial.println("----------------------------");
}

void loop() {

  int mode = digitalRead(MODE_PIN);

  if (mode == LOW) {

    // AUTOMATIC MODE

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0) {
      motorState = false;
      Serial.println("Sensor Error!");
    }
    else {
      float distance = duration * 0.0343 / 2;

      if (distance < FULL_DISTANCE - 1 ||
          distance > EMPTY_DISTANCE + 2) {

        motorState = false;
        Serial.println("Invalid sensor reading!");
      }
      else {
        float level =
          ((EMPTY_DISTANCE - distance) /
          (EMPTY_DISTANCE - FULL_DISTANCE)) * 100;

        level = constrain(level, 0, 100);

        Serial.print("Water Level: ");
        Serial.print(level);
        Serial.println("%");

        if (level <= MOTOR_ON_LEVEL) {
          motorState = true;
        }
        else if (level >= MOTOR_OFF_LEVEL) {
          motorState = false;
        }
      }
    }

    Serial.println("MODE: AUTO");

  }
  else {

    // MANUAL MODE

    int manualCommand = digitalRead(MANUAL_PIN);

    if (manualCommand == HIGH) {
      motorState = true;
    }
    else {
      motorState = false;
    }

    Serial.println("MODE: MANUAL");
  }

  // OUTPUT CONTROL

  digitalWrite(RELAY_PIN, motorState ? HIGH : LOW);
  digitalWrite(LED_PIN, motorState ? HIGH : LOW);

  Serial.print("MOTOR STATUS: ");
  Serial.println(motorState ? "ON" : "OFF");

  Serial.println("----------------------------");

  delay(1000);
}