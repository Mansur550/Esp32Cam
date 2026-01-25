
// #include "HX711.h"

// #define DT  13
// #define SCK 14
// HX711 scale;

// long rawValue = 0;
// float calibration_factor = 1.0;

// void setup() {
//   Serial.begin(115200);
//   while(!Serial);

//   Serial.println("HX711 Calibration");

//   scale.begin(DT, SCK);
//   scale.tare();

//   Serial.println("Remove all weight and press any key to continue...");
//   while (Serial.available() == 0) delay(3000);
//   Serial.read();
//   scale.tare();
//   Serial.println("Tare done. Place a known weight now...");
// }

// void loop() {
//   rawValue = scale.read_average(10);
//   Serial.print("Raw reading: "); Serial.println(rawValue);

//   float knownWeight = 0;
//   // Wait until valid weight is entered
//   while (knownWeight <= 0.0) {
//     Serial.println("Enter weight of the object in kg (example: 0.5 for 500g): ");
//     while (Serial.available() == 0) delay(100);
//     knownWeight = Serial.parseFloat();
//     Serial.read(); // clear input
//     if (knownWeight <= 0.0) Serial.println("Invalid weight! Try again...");
//   }

//   calibration_factor = rawValue / knownWeight;

//   Serial.println("----------------------------");
//   Serial.print("Raw reading: "); Serial.println(rawValue);
//   Serial.print("Known weight: "); Serial.println(knownWeight, 3);
//   Serial.print("Calculated scale factor: "); Serial.println(calibration_factor, 2);
//   Serial.println("Use this scale factor in your main sketch to convert raw -> kg");
//   Serial.println("----------------------------");

//   Serial.println("Calibration done. Reset to measure weight now.");
//   while(true) delay(1000); // stop here after calibration
// }

#include "HX711.h"

#define DT  13
#define SCK 14
HX711 scale;

float scaleFactor = 353379.56;  // from calibration
long zeroRaw = -31000;          // adjust this based on empty reading

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("HX711 Weight Test with Zero Offset");

  scale.begin(DT, SCK);
  scale.tare(); // zero scale
  Serial.println("Tare done. Place object on scale...");
}

void loop() {
  if (scale.is_ready()) {
    long rawValue = scale.read_average(10);

    // subtract zero offset
    float weightKg = (rawValue - zeroRaw) / scaleFactor;

    Serial.print("Raw value: "); Serial.print(rawValue);
    Serial.print(" | Weight (kg): "); Serial.println(weightKg, 3);

  } else {
    Serial.println("HX711 not ready!");
  }

  delay(1000);
}
