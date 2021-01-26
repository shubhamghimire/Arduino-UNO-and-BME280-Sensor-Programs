#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1017.25)
Adafruit_BME280 bme;

Scheduler ts;

// Callback methods prototypes
void InitializeCallback();
bool SensorEnable();
void SensorDisable();
void DataOn();
void DataOff();

// Tasks
Task tInitialize(10000, TASK_FOREVER, &InitializeCallback, &ts, true);
Task tSensorStatus(2000, TASK_ONCE, NULL, &ts, false, &SensorEnable, &SensorDisable);
Task tData(TASK_IMMEDIATE, TASK_FOREVER, NULL, &ts, false, NULL, &DataOff);



void InitializeCallback(){
  //Serial.println("In InitializeCallback");
  tSensorStatus.restartDelayed();
}

bool SensorEnable(){

  //Serial.println("In SensorEnable");
  tData.setInterval(500 + random(501));
  tData.setCallback(&DataOn);
  tData.enable();

  return true;
  
}

void SensorDisable(){

  //Serial.println("In Sensor Disable");
  tData.disable();
  
}

void DataOn(){
  
  //Serial.println("In DataOn");
  Serial.print(bme.readTemperature());
  Serial.print(" *C \t");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
  tData.setCallback(&DataOff);
  
}

void DataOff(){
  //Serial.println("In DataOff");
  tData.setCallback(&DataOn);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial.println("Scheduler Test");
  while(!bme.begin()){
    Serial.println("Wiring Error!");
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  ts.execute();

}
