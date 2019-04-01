/**
 *
 *****************************************************************
 * IOT Challenges - https://instagram.com/iotchallenges          *
 *****************************************************************
 * Evandro Fruhauf                                               *
 *  GitHub - https://github.com/evandrodp                        *
 *  LinkedIn - http://linkedin.com/in/evandrofruhauf             *
 *****************************************************************
 * This code can contain parts from other open-sources projects, *
 * whenever possible it will be quoted.                          *
 *****************************************************************
 *
 * Seven Segments Display
 *
 * Challenge: Make the buttons works with the Seven segments display
 *
 * This code uses Bounce2 and SevSeg libraries.
 *
**/

#include <Arduino.h>
#include <Bounce2.h>
#include "SevSeg.h"

// Set BUTTONS
#define BUTTON_RED 10
#define BUTTON_YELLOW 11
#define BUTTON_GREEN 12
#define BUTTON_BLUE 13

// Instantiate a Bounce object
Bounce debouncerRed = Bounce();
Bounce debouncerYellow = Bounce();
Bounce debouncerGreen = Bounce();
Bounce debouncerBlue = Bounce();
int debouncems = 15;

SevSeg sevseg; //Instantiate a seven segment controller object
int number = 0;
bool active = false;

void setup() {
  // initialise BUTTONS digital pin as input pullup (Ground)
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  // initialise Debounce on the buttons
  debouncerRed.attach(BUTTON_RED);
  debouncerRed.interval(debouncems);
  debouncerYellow.attach(BUTTON_YELLOW);
  debouncerYellow.interval(debouncems);
  debouncerGreen.attach(BUTTON_GREEN);
  debouncerGreen.interval(debouncems);
  debouncerBlue.attach(BUTTON_BLUE);
  debouncerBlue.interval(debouncems);

  // initialise the Segment
  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = {5, 4, 3, 8, 9, 6, 7, 2}; // {A, B, C, D, E, F, G, DP} // http://www.circuitbasics.com/arduino-7-segment-display-tutorial/
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays);
  sevseg.setBrightness(90);
}

void checkButton(Bounce& debouncer, int action) {
  debouncer.update();
  int value = debouncer.fell();
  if (value == HIGH) {
    switch (action) {
      case 0:
        if (active) {
          sevseg.blank();
          active = false;
        } else {
          number = 0;
          active = true;
        }
        break;
      case 1:
        number = 0;
        break;
      case 2:
        if (number > 0) {
          number--;
        }
        break;
      case 3:
        if (number < 9) {
          number++;
        }
        break;
    }
  }
}

void loop() {
  // Check if the system is on/off on Red Button
  checkButton(debouncerRed, 0);
  if (active) {
    // Check the commands
    checkButton(debouncerYellow, 1);
    checkButton(debouncerGreen, 2);
    checkButton(debouncerBlue, 3);
    // Show the number
    sevseg.setNumber(number, 0);
  }
  sevseg.refreshDisplay();
}
