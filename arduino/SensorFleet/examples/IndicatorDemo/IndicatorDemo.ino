#include <Arduino.h>
#include <SensorFleet.h>
// Potentiometer simulates one sensor. An LED indicates out-of-range values.
#if defined(ARDUINO_ARCH_ESP32)
const int SENSOR_POT=34,ALERT_LED=18;
const double ADC_MAX=4095.0;
#else
const int SENSOR_POT=A0,ALERT_LED=8;
const double ADC_MAX=1023.0;
#endif
sensorfleet::Fleet fleet;
void setup(){
 Serial.begin(115200);pinMode(ALERT_LED,OUTPUT);digitalWrite(ALERT_LED,LOW);
 fleet.register_device("soil-1",20,80);
}
void loop(){
 double value=analogRead(SENSOR_POT)*100.0/ADC_MAX;
 sensorfleet::Result r=fleet.ingest("soil-1",value,millis()/1000UL);
 digitalWrite(ALERT_LED,r.status==sensorfleet::Status::Alert?HIGH:LOW);
 sensorfleet::Summary s=fleet.summary("soil-1");
 Serial.print("value=");Serial.print(value);
 Serial.print(" readings=");Serial.print(s.count);
 Serial.print(" alerts=");Serial.println(s.alerts);
 delay(1000);
}
