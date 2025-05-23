#include <FastLED.h> // http://librarymanager/All#FASTLED
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

#define LED_PIN     4
#define NUM_LEDS    21
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define TRIGGER_PIN 8
#define ECHO_PIN 7 

#define MAX_DISTANCE 200  // Distance maximale réaliste en cm
#define MIN_DISTANCE 10  // Distance minimale avant alerte rouge

CRGB leds[NUM_LEDS];

const int trigPin = TRIGGER_PIN;  
const int echoPin = ECHO_PIN;
float duration, distance;   
float scaleFactor = MAX_DISTANCE / NUM_LEDS;  // Chaque LED représente une portion de la distance

void setup() {
  delay(1000); // power-up safety delay
  Serial.begin(115200);
  Serial.println("Démarrage du capteur");

  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT); 

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // Mesurer la distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.00343) / 2; // Calcul de la distance en cm
  distance = constrain(distance, 0, MAX_DISTANCE);

  if (distance <= 0) {
    // Aucun objet détecté -> LEDs en vert, suppression du message "Distance : 0 cm"
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    Serial.println("Aucun objet détecté");
  } else {
    Serial.print("Distance : ");
    Serial.print(distance);
    Serial.println(" cm");
    // Serial.print("Durée de l'impulsion : ");
    // Serial.println(duration);


    int numLitLeds = map(distance, MIN_DISTANCE, MAX_DISTANCE, 1, NUM_LEDS);

    for (int i = 0; i < NUM_LEDS; i++) {
      // Ajout de transitions progressives
      if (i < numLitLeds / 2) {
        leds[i] = blend(CRGB::Green, CRGB::Orange, map(i, 0, NUM_LEDS / 2, 0, 255)); // Transition verte → orange
      } else if (i < numLitLeds) {
        leds[i] = blend(CRGB::Orange, CRGB::Red, map(i, NUM_LEDS / 2, NUM_LEDS, 0, 255)); // Transition orange → rouge
      } else {
        leds[i] = CRGB::Green; // LEDs restantes restent vertes
      }
    }
  }

  FastLED.show();
  delay(200); // Mise à jour rapide pour fluidité

}

