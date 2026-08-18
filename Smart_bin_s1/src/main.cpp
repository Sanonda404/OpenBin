// #include <Arduino.h>

// // put function declarations here:
// int myFunction(int, int);

// void setup() {
//   // put your setup code here, to run once:
//   int result = myFunction(2, 3);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }


#include <Arduino.h>
#include <Servo.h>
#include "config.h"

Servo lidServo;

int currentAngle = LID_CLOSED_ANGLE;
int targetAngle = LID_CLOSED_ANGLE;

unsigned long lastUltrasonicMs = 0;
unsigned long lastServoStepMs = 0;
unsigned long lastPresenceMs = 0;

// Filter buffer
float distBuffer[DIST_FILTER_SAMPLES] = {0};
uint8_t distCount = 0;
uint8_t distIndex = 0;

bool personDetected = false;

float readDistanceCmRaw() {
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);

  unsigned long duration = pulseIn(PIN_ULTRASONIC_ECHO, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) {
    return -1.0f; // timeout / invalid
  }

  float distanceCm = (duration * 0.0343f) * 0.5f;
  if (distanceCm < 2.0f || distanceCm > 400.0f) {
    return -1.0f; // out of HC-SR04 practical range
  }

  return distanceCm;
}

float updateDistanceFilter(float newSample) {
  if (newSample > 0.0f) {
    distBuffer[distIndex] = newSample;
    distIndex = (distIndex + 1) % DIST_FILTER_SAMPLES;
    if (distCount < DIST_FILTER_SAMPLES) {
      distCount++;
    }
  }

  if (distCount == 0) {
    return -1.0f;
  }

  float sum = 0.0f;
  for (uint8_t i = 0; i < distCount; i++) {
    sum += distBuffer[i];
  }
  return sum / distCount;
}

void updateServoMotion() {
  unsigned long now = millis();
  if (now - lastServoStepMs < SERVO_STEP_INTERVAL_MS) {
    return;
  }
  lastServoStepMs = now;

  if (currentAngle < targetAngle) {
    currentAngle++;
    lidServo.write(currentAngle);
  } else if (currentAngle > targetAngle) {
    currentAngle--;
    lidServo.write(currentAngle);
  }
}

void setup() {
  pinMode(PIN_ULTRASONIC_TRIG, OUTPUT);
  pinMode(PIN_ULTRASONIC_ECHO, INPUT);
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);

  lidServo.attach(PIN_SERVO_SIG);
  lidServo.write(LID_CLOSED_ANGLE);

  Serial.begin(115200);
  Serial.println(F("Smart dustbin lid started."));
}

void loop() {
  unsigned long now = millis();

  // Read ultrasonic sensor at safe interval
  if (now - lastUltrasonicMs >= ULTRASONIC_INTERVAL_MS) {
    lastUltrasonicMs = now;

    float rawCm = readDistanceCmRaw();
    float filteredCm = updateDistanceFilter(rawCm);

    // Hysteresis-based presence detection
    if (filteredCm > 0.0f) {
      if (!personDetected && filteredCm <= DETECT_ENTRY_CM) {
        personDetected = true;
      } else if (personDetected && filteredCm > DETECT_EXIT_CM) {
        personDetected = false;
      }
    }

    if (personDetected) {
      lastPresenceMs = now;
      targetAngle = LID_OPEN_ANGLE;
    } else {
      if (now - lastPresenceMs >= OPEN_HOLD_MS) {
        targetAngle = LID_CLOSED_ANGLE;
      }
    }

    Serial.print(F("raw_cm="));
    Serial.print(rawCm, 1);
    Serial.print(F(" filtered_cm="));
    Serial.print(filteredCm, 1);
    Serial.print(F(" detected="));
    Serial.print(personDetected ? F("YES") : F("NO"));
    Serial.print(F(" target="));
    Serial.print(targetAngle);
    Serial.print(F(" current="));
    Serial.println(currentAngle);
  }

  // Smooth open/close motion
  updateServoMotion();
}
