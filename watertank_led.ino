#include <NewPing.h>
int i = 0; //counter
int meting = 0;
int metingen = 0; 
float water_vol = 1.0;

unsigned long interval_led = 1000;
unsigned long interval_measurement = 5000;
unsigned long previousMillisMeasurement = 0;
unsigned long previousMillisLED = 0;
bool ledstate = false;

const int depth_put = 75; // diepte van de put (afstand overloop tot bodem in cm)
const int height_sensor = 00; //hoogte van sensor boven max waterlevel (cm)
const int TRIGGER_PIN =  13;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int ECHO_PIN  = 12;  // Arduino pin tied to echo pin on the ultrasonic sensor.
const int MAX_DISTANCE = 200 ;// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  //pinnen leds definëren
  for (int pin = 2; pin<7; pin++){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long TimeMeasurement = currentMillis - previousMillisMeasurement;
  unsigned long TimeLed = currentMillis - previousMillisLED;
   
  if (TimeMeasurement >= interval_measurement){
    meting = sonar.ping_cm();
    
    if (meting != 0) {
      //Serial.println("meting != 0");
      metingen = metingen + meting;
      i = i + 1;
    }
    previousMillisMeasurement = millis();
  }

  if (i == 10) {
    i = 0;
    int dist = metingen/10;
    float free_space = dist - height_sensor;
    water_vol = 1.0 - (free_space/depth_put); //relatief resterend volume water
    if (water_vol < 0){
      water_vol = 0; 
    }
    metingen = 0;
   }
  
  if (water_vol >= 0.8){
      for (int pin = 2; pin<7; pin++){
      digitalWrite(pin, HIGH);
    }
  }
  else if (water_vol < 0.8 && water_vol >= 0.6){
    digitalWrite(2, LOW);
    for (int pin = 3; pin<7; pin++){
      digitalWrite(pin, HIGH);
    }
  }
  else if (water_vol < 0.6 && water_vol >= 0.4){      
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
  }
  else if (water_vol < 0.4 && water_vol >= 0.2){
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
  }
  else if (water_vol < 0.2 && water_vol >= 0.1){
    for (int pin = 2; pin<6; pin++){
          digitalWrite(pin, LOW);
        }
    digitalWrite(6, HIGH);
  }
  else if (water_vol < 0.1){
    if (TimeLed >= interval_led){
      for (int pin = 2; pin<6; pin++){
            digitalWrite(pin, LOW);
          }
      ledstate = ! ledstate;
      digitalWrite(6, ledstate);
      previousMillisLED = millis();
    }
  }
 }
