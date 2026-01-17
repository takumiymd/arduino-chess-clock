// Author:Takumi Yamada
// Since:December 17, 2025
// Project: Chess Clock

// TM1637Dsiplay
// https://github.com/avishorp/TM1637/blob/master/TM1637Display.h
#include <TM1637Display.h>

// white side display
const uint8_t whiteCLK = 2;
const uint8_t whiteDIO = 3;
// black side display
const uint8_t blackCLK = 4;
const uint8_t blackDIO = 5;

// White Button
const int whiteButton = 8;
// Reset Button
const int resetButton = 9;
// Black Button
const int blackButton = 10;
// Buzzer
// const int buzzerButton = 11;

TM1637Display whiteTm(whiteCLK, whiteDIO);
TM1637Display blackTm(blackCLK, blackDIO);

// 5 minutes
int whiteTime = 500;
int blackTime = 500;

uint8_t dots = 0b01000000;

// time management
unsigned long previousMillis = 0;
const unsigned long interval = 1000;

// state management
bool whiteRunning = false;
bool blackRunning = false;

void setup() {
  whiteTm.setBrightness(7, true);
  blackTm.setBrightness(7, true);

  pinMode(whiteButton, INPUT_PULLUP); // White Button
  pinMode(resetButton, INPUT_PULLUP); // Rest Button
  pinMode(blackButton, INPUT_PULLUP); // Black Button
  // pinMode(buzzerButton, OUTPUT);      // Buzzer

  whiteTm.clear();
  blackTm.clear();

  whiteTm.showNumberDecEx(whiteTime, dots, true);
  blackTm.showNumberDecEx(blackTime, dots, true);
}

void loop() {
  unsigned long currentMillis = millis();

  // Red button(Rest Button) pressed
  if (digitalRead(resetButton) == LOW) {

    //  Reset the both timer to initial value of 5:00
    whiteTime = 500;
    blackTime = 500;

    previousMillis = millis();

    whiteRunning = false;
    blackRunning = false;

    displayHelper(whiteTm, whiteTime);
    displayHelper(blackTm, blackTime);
  }

  // Black button pressed then white runs
  if (digitalRead(blackButton) == LOW) {
    whiteRunning = true;
    blackRunning = false;
  }

  // White button pressed then black runs
  if (digitalRead(whiteButton) == LOW) {
    whiteRunning = false;
    blackRunning = true;
  }
  // White countdown
  if (whiteRunning && currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    whiteTime = countDownHelper(whiteTime);
    displayHelper(whiteTm, whiteTime);

    if (whiteTime == 0)
      whiteRunning = false;
  }

  // Black countdown
  if (blackRunning && currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    blackTime = countDownHelper(blackTime);
    displayHelper(blackTm, blackTime);

    if (blackTime == 0)
      blackRunning = false;
  }

  // helper method for countdown until 00:00
  int countDownHelper(int timeValue) {
    int minute = timeValue / 100;
    int second = timeValue % 100;

    if (minute == 0 && second == 0)
      return 0;

    if (second == 0) {
      minute--;
      second = 59;
    } else {
      second--;
    }

    return minute * 100 + second;
  }

  // helper method for displaying the time
  void displayHelper(TM1637Display & display, int timeValue) {
    display.showNumberDecEx(timeValue, dots, true);
  }
