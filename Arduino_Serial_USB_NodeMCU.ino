/**********************************
 * IOT Project With Arduino & Blynk 
 * ********************************
 * Project : System Control percentage of Oxygen 
 *           in the body using IOT
 * Group   : 4
 * Authors : Hosam Elnabawy, 
 *           Alaa Ashraf,
 *          
 * 
 ***********************************/


#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

PulseOximeter pox;

// WIFI Credentials 
char auth[] = "qDNllb3ZCZGV-l1YJ5401k7NPfinYYiw";
char ssid[] = "Hosam";
char pass[] = "Hoss135791113";



#define REPORTING_PERIOD_MS     3000
#define BLYNK_RED       "#D3435C"
#define LED D5 
 
WidgetLED spo2StatusLed(V4);



uint32_t tsLastReport = 0;
float BPM, SpO2;


void onBeatDetected()
{
    ;
}

// Listen For Blynk V4 which is connected to a push Button

BLYNK_WRITE(V4){
  
//Serial.println("READ From V4");
  int pinValue = param.asInt();
  // Get the Button Value from Blynk App 
  if(pinValue == 1){
    spo2StatusLed.on();
    digitalWrite(LED, HIGH);
  }
  if(pinValue == 0){
    spo2StatusLed.off();
    digitalWrite(LED, LOW);
  }
}


void setup()
{
    Serial.begin(115200);
    Serial.print("Initializing pulse oximeter..");

    pinMode(LED, OUTPUT);
    
    // Initialize the PulseOximeter instance
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
        digitalWrite(1,HIGH);
    }
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);

    // Starting bylink
    Blynk.begin(auth, ssid, pass);
}
 
void loop()
{
 
    
    // Make sure to call update as fast as possible
    pox.update();
    Blynk.run();

    // Getting Sensor Data ( Heart rate,  Oxygen Percentage)
    SpO2 = pox.getSpO2();
    BPM = pox.getHeartRate();
    
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

        // Display Sensor Info in Serial For Better Debugging
        Serial.print("BPM: ");
        Serial.print(BPM);
        //blue.println("\n");
        
        Serial.print("  ||   SpO2: ");
        Serial.print(SpO2);
        Serial.print("%");
        Serial.println("\n");
        Blynk.virtualWrite(V2, BPM);
        Blynk.virtualWrite(V3, SpO2);
      

       // For Normal People Oxygen Percentage varies from 95% - 100%
       // 
//          if (SpO2 < 95 && SpO2 != 0) {
//            spo2StatusLed.on();
//            digitalWrite(LED, HIGH);
//          } else {
//            spo2StatusLed.off();
//            digitalWrite(LED, LOW);
//          }

        tsLastReport = millis();
        
    }
 
}
