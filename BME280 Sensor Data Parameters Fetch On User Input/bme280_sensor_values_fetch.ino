#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

void setup() {
  Serial.begin(9600);
  bool status;

  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");

  }
  Serial.println("Enter 1 for continuous values, 2 to Stop program and 3 for only certain values");

}
char rx_byte = 0;
void loop() {


  if (Serial1.available() > 0) {

    rx_byte = Serial1.read();
    //Serial.println(rx_byte);

    if (rx_byte == '1') {
      start();

    }
    else if (rx_byte == '2') {
      stopLoop();
    }
    else if (rx_byte == '3') {
      userValues();
    }
    else {
      Serial.println("");
    }


  }
  delay(500);
}

void start() {
  while (1) {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    loop();
  }

}
void stopLoop() {
  for (;;) {
    loop();
  }


}

void userValues() {
  char incomingByte = 0;
  int num = 0;
  if (Serial2.available() > 0) {

   incomingByte = Serial2.read();
   num = incomingByte - '0';
    //Serial.println(rx_byte);
    for (int i = 1; i <= num; i++) {
      Serial.print("Temperature = ");
      Serial.print(bme.readTemperature());
      Serial.println(" *C");  
      delay(1000);
    }
    Serial.println("\n");
    
  loop();
}
}
