#include <HX711_ADC.h>
#include <FlashStorage.h>
#include <Servo.h>

// Pins:
const int HX711_dout = 4; // MCU > HX711 dout pin
const int HX711_sck = 5; // MCU > HX711 sck pin
const int servoPin = 6; // Pin connected to the servo signal line

// HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

// Create a structure to hold the calibration value
struct CalibrationValue {
  float calValue;
};

// Declare a FlashStorage object for the calibration value
FlashStorage(calibration_store, CalibrationValue);

// Servo object
Servo myservo;

unsigned long t = 0;
bool isServoAt90 = false; // Tracks whether the servo is currently at 90 degrees

void setup() {
  Serial.begin(57600); 
  delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();

  // Fetch calibration value from flash storage or use a default
  CalibrationValue storedCal = calibration_store.read();
  float calibrationValue = (storedCal.calValue > 0) ? storedCal.calValue : 433.55;

  unsigned long stabilizingtime = 2000; // Precision improvement after power-up
  boolean _tare = true; // Perform tare in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  } else {
    LoadCell.setCalFactor(calibrationValue); // Set the calibration factor
    Serial.println("Startup is complete");
  }

  // Initialize the servo
  myservo.attach(servoPin);
  myservo.write(0); // Start servo at 0 degrees
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500; // Set interval for serial output

  // Check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // Get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float weight = LoadCell.getData();
      Serial.print("Load cell output value: ");
      Serial.println(weight);
      newDataReady = 0;
      t = millis();

      // Control the servo based on the weight
      if (weight > 6.0 && !isServoAt90) {
        myservo.write(90); // Move servo to 90 degrees
        isServoAt90 = true; // Mark the servo as at 90 degrees
      } else if (weight < 6.0 && isServoAt90) {
        myservo.write(0); // Move servo back to 0 degrees
        isServoAt90 = false; // Mark the servo as not at 90 degrees
      }
    }
  }

  // Receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  // Check if the last tare operation is complete:
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}
