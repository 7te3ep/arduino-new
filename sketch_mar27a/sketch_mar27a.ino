#include <sensirion_common.h>
#include <sgp_featureset.h>
#include <i2c.h>
#include <sgp30.h>
#include <sensirion_configuration.h>

#include <ChainableLED.h>

#include <Arduino.h>
#include <Servo.h> // Use Servo library, included with IDE


#define NUM_LEDS  5

ChainableLED leds(7, 8, NUM_LEDS);

Servo myServo;
void setup() {
    leds.init();
    s16 err;
    u16 scaled_ethanol_signal, scaled_h2_signal;
    myServo.attach(3); 
    Serial.begin(115200);
    Serial.println("serial start!!");
    pinMode(4, OUTPUT);
    digitalWrite(4, 1);
    delay(500);
    while (sgp_probe() != STATUS_OK) {
        Serial.println("SGP failed");
        while (1);
    }
    err = sgp_measure_signals_blocking_read(&scaled_ethanol_signal,
    &scaled_h2_signal);
    if (err == STATUS_OK) {
        Serial.println("get ram signal!");
    } else {
        Serial.println("error reading signals");
    }
    err = sgp_iaq_init();
}

byte pos = 0;
void loop() {
  s16 err = 0;
  u16 tvoc_ppb, co2_eq_ppm;
  err = sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
  if (err == STATUS_OK) {
      Serial.print("CO2eq Concentration:");
      Serial.println(co2_eq_ppm);
  } else {
      Serial.println("error reading IAQ values\n");
  }
  
  if ( co2_eq_ppm <=400){
    leds.setColorRGB(0, 0, 255, 0); 
    myServo.write(0); // Rotate servo counter clockwise
    delay(100);
  }
  else if (co2_eq_ppm <= 500){
    leds.setColorRGB(0, 255, 135, 0); 
    myServo.write(90); // Rotate servo counter clockwise
    delay(100);
  }
  else if (co2_eq_ppm <= 600){
    leds.setColorRGB(0, 255, 0, 0); 
    myServo.write(160); // Rotate servo counter clockwise
    delay(100);
  }
}
