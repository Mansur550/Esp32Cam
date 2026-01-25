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
