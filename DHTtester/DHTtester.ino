#include "DHT.h"
#include <Servo.h>  // Include the Servo library

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321
#define LEDPIN 3    // Digital pin connected to the LED

DHT dht(DHTPIN, DHTTYPE);
Servo myServo;  // Create a servo object

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  myServo.attach(9);  // Attaches the servo on pin 9 to the servo object
  myServo.write(0);   // Ensure servo is at 0 degrees at start-up

  pinMode(LEDPIN, OUTPUT);  // Set the LED pin as an output
  digitalWrite(LEDPIN, LOW);  // Ensure LED is off at start-up
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahrenheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  // Control the servo and LED based on the temperature
  if (t > 22) {
    myServo.write(90);  // Turn servo to 90 degrees
    for (int i = 0; i < 6; i++) {  // Flash LED three times (6 toggles)
      digitalWrite(LEDPIN, HIGH);
      delay(250);  // LED on for 250 ms
      digitalWrite(LEDPIN, LOW);
      delay(250);  // LED off for 250 ms
    }
    delay(3000 - 1500);  // Subtract the LED flashing time from the 3-second wait
    myServo.write(0);   // Return servo to 0 degrees
    delay(1000);        // Wait for the servo to reach 0 degrees before the next loop iteration
  }
}
