#include <Wire.h>

#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

Scheduler runner;

// Callback methods prototypes
void t1Callback();
void t2Callback();
void t3Callback();

//Tasks
Task t4();
Task t1(2000, 10, &t1Callback);
Task t2(3000, TASK_FOREVER, &t2Callback);
Task t3(5000, TASK_FOREVER, &t3Callback);

void t1Callback(){
  Serial.print("t1: ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  if (t1.isFirstIteration()){
    runner.addTask(t3);
    t3.enable();
    Serial.println("t1: enabled t3 and added to the chain");
  }

  if (t1.isLastIteration()){
    t3.disable();
    runner.deleteTask(t3);
    t2.setInterval(500);
    Serial.println("t1: disable t3 and delete it from the chain. t2 interval set to 500");
  }
}

void t2Callback(){

  Serial.print("t2: ");
  Serial.print(bme.readPressure()/100.0F);
  Serial.println(" hPa");
}

void t3Callback(){

  Serial.print("t3: ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Scheduler Test");
  while(!bme.begin()){
    Serial.println("Wiring Error!");
  }

  runner.init();
  Serial.println("Initializes Scheduler");

  runner.addTask(t1);
  Serial.println("added t1 Temperature");

  runner.addTask(t2);
  Serial.println("added t2 Pressure");

  delay(5000);

  t1.enable();
  Serial.println("Enabled t1");
  t2.enable();
  Serial.println("Enabled t2");

}

void loop() {
  // put your main code here, to run repeatedly:
  runner.execute();
}
