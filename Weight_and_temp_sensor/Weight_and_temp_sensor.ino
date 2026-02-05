#include "DHT.h"
#include <Servo.h>
#include <HX711_ADC.h>
#include <FlashStorage.h>
#include <WiFiNINA.h>
#include "secrets.h"  // Secrets file for sensitive data

// DHT sensor setup
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Load cell setup
const int HX711_dout = 4;
const int HX711_sck = 5;
HX711_ADC LoadCell(HX711_dout, HX711_sck);

// Servo setup
const int servoPin = 6;
Servo myservo;

// Calibration value storage
struct CalibrationValue {
  float calValue;
};
FlashStorage(calibration_store, CalibrationValue);

// WiFi and server details for IFTTT
WiFiClient client;
char ssid[] = SECRET_SSID;   
char pass[] = SECRET_PASS;
char HOST_NAME[] = "maker.ifttt.com";
String eventPath = "/trigger/Dog has been fed/with/key/iEJ_AQmrMgct8AEcOfZJZU3Z-8unWogVxkAOuooPm2t";

unsigned long t = 0;
bool newDataReady = false;
bool isServoAt90 = false;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx, HX711, Servo, and IFTTT test"));

  // Initialize DHT sensor
  dht.begin();

  // Initialize load cell
  LoadCell.begin();

  CalibrationValue storedCal = calibration_store.read();
  float calibrationValue = (storedCal.calValue > 0) ? storedCal.calValue : 315.16;
  unsigned long stabilizingtime = 2000;
  bool _tare = true;
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  } else {
    LoadCell.setCalFactor(calibrationValue);
    Serial.println("Startup is complete");
  }

  // Initialize the servo
  myservo.attach(servoPin);
  myservo.write(0); // Start servo at 0 degrees

  // Initialize WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}


void sendIFTTTNotification(float weight) {
  if (client.connect(HOST_NAME, 80)) {
    Serial.println("Connected to server");

    // Send HTTP request to trigger the IFTTT event
    client.println("GET " + eventPath + "?value1=" + String(weight, 1) + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();

    // Wait for the server response and read it
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
    Serial.println("Disconnected");
  } else {
    Serial.println("Connection failed");
  }
}

void loop() {
  // Read DHT sensor data
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();
  float temperatureF = dht.readTemperature(true);
  int amount = 400 - 295; // Dispensed amount minus percentage difference 

  // Check if DHT read failed
  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Check for new data from the load cell
  if (LoadCell.update()) newDataReady = true;

  if (newDataReady) {
    if (millis() > t + 100) {
      float weight = LoadCell.getData();
      newDataReady = false;
      t = millis();

      // Combine all sensor data into a single string
      String data = "Weight: " + String(weight, 1) + " g, ";
      data += "Humidity: " + String(humidity, 2) + "%, ";
      data += "Temp: " + String(temperatureC, 2) + "C";


      Serial.println(data);

      // Control the servo based on the weight
      if (weight > amount && !isServoAt90) {
        myservo.write(90); // Move servo to 90 degrees quickly
        isServoAt90 = true; // Mark the servo as at 90 degrees
        sendIFTTTNotification(weight); // Send IFTTT notification
      } else if (weight < amount && isServoAt90) {
        delay(15000);
        myservo.write(5); // Move servo back to 0 degrees quickly
        isServoAt90 = false; // Mark the servo as not at 90 degrees
      }
    }
  }

  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }


  if (LoadCell.getTareStatus()) {
    Serial.println("Tare complete");
  }

  delay(100);
}
