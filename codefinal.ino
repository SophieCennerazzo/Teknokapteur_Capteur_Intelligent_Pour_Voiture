#include <FastLED.h>

#define LED_PIN     4
#define NUM_LEDS    21
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define TRIGGER_PIN 8
#define ECHO_PIN 7 

#define MAX_DISTANCE 200  
#define MIN_DISTANCE 2   

CRGB leds[NUM_LEDS];

void setup() {
  delay(1000);
  Serial.begin(115200);
  while (!Serial) { delay(10); } // Attendre que le moniteur série soit prêt
  
  Serial.println("Démarrage du capteur");

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  float duration = pulseIn(ECHO_PIN, HIGH);
  float distance = (duration * 0.0343) / 2;  
  distance = constrain(distance, 0, MAX_DISTANCE);

  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance == 0) {
    // Affichage entièrement en vert si aucun objet n'est détecté
    fill_solid(leds, NUM_LEDS, CRGB::Green);
  } else {
    // Calcul du nombre de LEDs à afficher
    int numLitLeds = map(distance, MIN_DISTANCE, MAX_DISTANCE, 1, NUM_LEDS);

    for (int i = 0; i < NUM_LEDS; i++) {
      if (i < numLitLeds) {
        leds[i] = CRGB::Red; // LEDs deviennent rouges progressivement
      } else {
        leds[i] = CRGB::Green; // LEDs restantes restent vertes
      }
    }
  }

  FastLED.show();
  delay(100);
}

