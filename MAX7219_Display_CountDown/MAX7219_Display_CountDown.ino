#include <LedControl.h>

// Define the number of devices
#define NUM_DEVICES 2

// Pins for the MAX7219 module
#define DATA_IN 11
#define CLK 13
#define CS 10

// Create a LedControl instance
LedControl lc = LedControl(DATA_IN, CLK, CS, NUM_DEVICES);

// Digit patterns for numbers 0-9
byte numbers[10][8] = {
  {B00111100, B01000010, B01000110, B01001010, B01010010, B01100010, B01000010, B00111100}, // 0
  {B00001000, B00011000, B00101000, B00001000, B00001000, B00001000, B00001000, B01111110}, // 1
  {B00111100, B01000010, B01000010, B00000100, B00001000, B00010000, B00100010, B01111110}, // 2
  {B00111100, B01000010, B00000010, B00011100, B00000010, B00000010, B01000010, B00111100}, // 3
  {B00000100, B00001100, B00010100, B00100100, B01000100, B01111110, B00000100, B00000100}, // 4
  {B01111110, B01000000, B01111100, B00000010, B00000010, B00000010, B01000010, B00111100}, // 5
  {B00111100, B01000010, B01000000, B01111100, B01000010, B01000010, B01000010, B00111100}, // 6
  {B01111110, B01000010, B00000100, B00001000, B00010000, B00010000, B00010000, B00010000}, // 7
  {B00111100, B01000010, B01000010, B00111100, B01000010, B01000010, B01000010, B00111100}, // 8
  {B00111100, B01000010, B01000010, B00111110, B00000010, B00000010, B01000010, B00111100}  // 9
};

void setup() {
  // Initialize the MAX7219 devices
  for (int i = 0; i < NUM_DEVICES; i++) {
    lc.shutdown(i, false);       // Wake up the MAX7219
    lc.setIntensity(i, 8);       // Set brightness level (0 is min, 15 is max)
    lc.clearDisplay(i);          // Clear display register
  }
}

void displayNumber(int num, int device) {
  if (num < 0 || num > 9) return;
  for (int i = 0; i < 8; i++) {
    lc.setRow(device, i, numbers[num][i]);
  }
}

void loop() {
  for (int i = 9; i >= 0; i--) {
    // Display the number on both devices
    displayNumber(i, 0); // Display on the first device
    displayNumber(i, 1); // Display on the second device
    delay(1000); // Wait for 1 second
  }
}
