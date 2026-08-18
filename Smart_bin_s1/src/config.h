#pragma once

// Pin map (canonical wiring)
#define PIN_ULTRASONIC_TRIG 7
#define PIN_ULTRASONIC_ECHO 8
#define PIN_SERVO_SIG       9

// Lid angles
#define LID_CLOSED_ANGLE 0
#define LID_OPEN_ANGLE   100

// Detection thresholds (cm) with hysteresis
#define DETECT_ENTRY_CM 25.0f
#define DETECT_EXIT_CM  30.0f

// Timing
#define ULTRASONIC_INTERVAL_MS 70UL     // >= 60 ms for HC-SR04
#define OPEN_HOLD_MS           3000UL   // keep open for 3s after last presence
#define SERVO_STEP_INTERVAL_MS 15UL     // smooth movement speed

// Ultrasonic pulse timeout
#define ECHO_TIMEOUT_US 30000UL

// Simple moving-average filter
#define DIST_FILTER_SAMPLES 5
