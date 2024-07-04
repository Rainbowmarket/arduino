#include "FastLED.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define NUM_LEDS 45
#define DATA_PIN 2
#define SCORE_PIN 6
#define SCORE_LEDS 4
#define PUSH_BUTTON 4
#define RX_PIN 1
#define TX_PIN 0

CRGB leds[NUM_LEDS];
CRGB sleds[SCORE_LEDS];

bool reachedEnd = false;
byte gameState = 0;
int period = 1000;
unsigned long time_now = 0;
byte Position = 0;
byte level = 0;
const byte ledSpeed[4] = {50, 40, 35, 20};

// Debounce
bool findRandom = false;
byte spot = 0;

SoftwareSerial mySoftwareSerial(RX_PIN, TX_PIN); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, SCORE_PIN, GRB>(sleds, SCORE_LEDS);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  //mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  
 // if (!myDFPlayer.begin(mySoftwareSerial)) {
 //   Serial.println(F("Unable to begin:"));
 //   Serial.println(F("1. Please recheck the connection!"));
 //   Serial.println(F("2. Please insert the SD card!"));
 //   while (true);
 // }
  
  //Serial.println(F("DFPlayer Mini online."));
  //myDFPlayer.volume(10);  // Set volume value. From 0 to 30

 // Serial.println("Reset");
}

void loop() {
  switch (gameState) {
    case 0:
      attractMode();
      break;
    case 1:
    case 2:
    case 3:
    case 4:
      playLevel(gameState - 1);
      break;
    case 98:
      winner();
      break;
    case 99:
      loser();
      break;
  }
}

void attractMode() {
  fill_rainbow(leds, NUM_LEDS, 0, 15);
  fill_rainbow(sleds, SCORE_LEDS, 0, 7);
  FastLED.show();
  if (digitalRead(PUSH_BUTTON) == LOW) {
    startGame();
  }
}

void startGame() {
  Position = 0;
  findRandom = true;
  delay(500);
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
    delay(40);
    FastLED.show();
  }
  for (byte i = 0; i < SCORE_LEDS; i++) {
    sleds[i].setRGB(0, 0, 0);
    delay(100);
    FastLED.show();
  }
  gameState = 1;
  //myDFPlayer.play(1);  // Play game start audio
}

void playLevel(byte levelIndex) {
  period = ledSpeed[levelIndex];
  if (millis() > time_now + period) {
    time_now = millis();
    if (findRandom) {
      spot = random(44) + 3;
      findRandom = false;
    }
    leds[spot - 1].setRGB(255, 140, 0);
    leds[spot].setRGB(0, 255, 0);
    leds[spot + 1].setRGB(255, 140, 0);
    sleds[levelIndex].setRGB(0, 0, 255);
    PlayGame(spot - 1, spot + 1);
  }
  if (digitalRead(PUSH_BUTTON) == LOW) {
    delay(300);
    if (Position > spot - 1 && Position < spot + 3) {
      level = gameState;
      gameState = 98;
    } else {
      gameState = 99;
    }
  }
}

void PlayGame(byte bound1, byte bound2) {
  leds[Position].setRGB(255, 0, 0);
  if (Position < bound1 + 1 || Position > bound2 + 1) {
    leds[Position - 1].setRGB(0, 0, 0);
  }
  FastLED.show();
  Position++;
  if (Position >= NUM_LEDS) {
    leds[Position - 1].setRGB(0, 0, 0);
    Position = 0;
  }
}

void winner() {
  //myDFPlayer.play(3);  // Play game winner audio
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < NUM_LEDS; j++) {
      leds[j].setRGB(0, 255, 0);
    }
    FastLED.show();
    delay(500);
    clearLEDS();
    FastLED.show();
    delay(500);
  }
  findRandom = true;
  Position = 0;
  gameState = level + 1;
  if (gameState > 4) {
    gameState = 0;
  }
}

void loser() {
  //myDFPlayer.play(2);  // Play game out audio
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < NUM_LEDS; j++) {
      leds[j].setRGB(255, 0, 0);
    }
    FastLED.show();
    delay(500);
    clearLEDS();
    FastLED.show();
    delay(500);
  }
  gameState = 0;
}

void clearLEDS() {
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
}

void winAll() {
  // Implementation for winning all levels
}
