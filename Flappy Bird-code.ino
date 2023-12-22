#include <TFT_eSPI.h>
#include <SPI.h>
#include <EEPROM.h>
#include "bird.c"
#include <stdint.h>
//==== Creating Objects
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

//==== Defining Fonts
#include <TFT_eSPI.h> // Hardware-specific library
int x, y; // Variables for the coordinates where the display has been pressed
#define TFT_SKYBLUE tft.color565(135, 206, 235)

// Pillar object
#define PILLAR_WIDTH 50
#define MIN_PILLAR_HEIGHT 40
#define PILLAR_GAP 80
int last_xP = 0;
int last_yP = 0;

//defining note definitions
#define NOTE_C4 262
#define NOTE_E4 330
#define NOTE_G4 392
#define NOTE_F4 349
#define NOTE_A4 440
#define NOTE_C5 523
#define NOTE_B4 494
#define NOTE_D5 587
#define NOTE_F5 698
#define NOTE_G5 784


// Floppy Bird
int xP = 479;
int yP = 150;
int yB = 145;
int movingRate = 60;
int fallRateInt = 0;
float fallRate = 0;
int score = 0;
const int button = 11;
const int superjump_button = 6;
const int button_reset = 7;
int buttonState = 0;
int lastSpeedUpScore = 0;
int highestScore;
boolean gameStarted = false;
int last_yB = 145;
const int buzzerPin = 3;
const int button_gameover = 2;
const int frameRate = 300; // 60 frames per second
float movingRateModifier;
int speaker =3;
const int pause_button =5; // Replace with the actual pin number for the pause button
bool isPaused = false;


int noteToFrequency(char note, int octave) {
  const int noteValues[] = {261, 293, 329, 349, 392, 440, 493};
  const char noteNames[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};

  for (int i = 0; i < sizeof(noteNames); i++) {
    if (note == noteNames[i]) {
      return noteValues[i] * pow(2, octave - 4);
    }
  }
  return 0; // Invalid note name
}

volatile bool buttonPressed = false;
volatile bool superjumpPressed = false;

void button_ISR() {
  buttonPressed = true;
}

void superjump_button_ISR() {
  superjumpPressed = true;
}


void setup() {
  // Initiate display
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  int timePerFrame = 500 / frameRate;
  movingRateModifier = (float)timePerFrame / 12.0;
  attachInterrupt(digitalPinToInterrupt(button), button_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(superjump_button), superjump_button_ISR, FALLING);

  pinMode(button, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(superjump_button, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(button_reset, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(buzzerPin, OUTPUT);
  pinMode(button_gameover, INPUT_PULLUP); // Use internal pull-up resistor <-- Add this line
pinMode(pause_button, INPUT_PULLUP);



  highestScore = EEPROM.read(0); // Read the highest score from the EEPROM
  
  Serial.begin(9600);

  initiateGame();
}
volatile bool pauseButtonPressedState = false;
volatile unsigned long lastPauseButtonPress = 0;

void pauseButtonPressedISR() {
  if (millis() - lastPauseButtonPress > 50) {
    pauseButtonPressedState = !pauseButtonPressedState;
  }
  lastPauseButtonPress = millis();
}

void loop() {
  
  if (isPaused) {
    delay(50); // Add a small delay to reduce CPU usage while paused
    if (!digitalRead(pause_button) && !pauseButtonPressedState) {
      pauseButtonPressedState = true;
      tft.fillScreen(TFT_SKYBLUE); // Clear the "PAUSED" text -----
    } else if (digitalRead(pause_button) && pauseButtonPressedState) {
      pauseButtonPressedState = false;
      isPaused = false;
    }
    return;
  }

  updateButtonStates(); // Check the button states, including the pause button

  // The rest of your game logic goes here...

  xP = xP - (movingRate * movingRateModifier); // xP - x coordinate of the pilars; range: 479 - (-51)
   //delay( 100/ frameRate)
  //delay( 0);
  drawPillars(xP, yP); // Draws the pillars

  // Draw the floor
  tft.fillRect(0, 295, 480, 35, 0x228B22); // Dark green
  tft.fillRect(0, 285, 480, 10, 0x7E0); // Light green
  tft.fillRect(0, 284, 480, 1, TFT_BLACK); // Black line

  // yB - y coordinate of the bird which depends on the value of the fallingRate variable
  yB += fallRate;
  fallRate = 7; // Set the fall rate to a constant value

  // Erase the previous position of the bird
  eraseBird(last_yB);

  // Erase the previous position of the bird
  eraseBird(last_yB);

  last_yB = yB;

  // Checks for collision
 if (yB >= 256 || yB <= 0) { // top and bottom
    gameOver();
  }
  if ((xP <= 85) && (xP >= 5) && (yB <= yP - 10)) { // upper pillar
    gameOver();
  }
  if ((xP <= 85) && (xP >= 5) && (yB + 30 >= yP + PILLAR_GAP)) { // lower pillar
    gameOver();
  }
  // Add this condition within the loop() function
  if (!digitalRead(button_gameover)) {
    gameOver();
  }

  // Add this condition within the loop() function
  if (!digitalRead(button_gameover)) {
    gameOver();
  }

  // Draws the bird
  drawBird(yB);

  // After the pillar has passed through the screen
  if (xP <= -51) {
    xP = 479; // Resets xP to 479
    yP = rand() % 100 + 20; // Random number for the pillars height
    score++; // Increase score by one
    playScoreSound(); // Play score sound
  }

  // After each five points, increases the moving rate of the pillars
  if ((score - lastSpeedUpScore) == 1) {
    lastSpeedUpScore = score;
    movingRate++;
  }
}


//initiate game function
void menu() {
  tft.fillScreen(TFT_BLACK); // Clear the screen with a black background

  // Draw the game title "Flying Pikachu"
  tft.setTextColor(TFT_WHITE); // Set text color to white
  tft.setTextSize(4); // Set text size to 2
  tft.setTextDatum(TC_DATUM); // Set text datum to top-center
  tft.drawString("Flappy Bird", tft.width() / 2, 60); // Draw the title at the top-center of the screen

  // Draw the "HOLD TO START" message
  tft.setTextColor(TFT_YELLOW); // Set text color to yellow
  tft.setTextSize(2); // Set text size to 1
  tft.setTextDatum(MC_DATUM); // Set text datum to middle-center
  tft.drawString("HOLD TO START", tft.width() / 2, tft.height() / 2)+ 230; // Draw the message at the middle-center of the screen

  // Draw credits
  tft.setCursor(10, 300);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.print("Developed by Azmath");
}



void initiateGame() {
  // Display the menu
  menu();

  // Debouncing variables
  unsigned long lastButtonPress = 0;
  bool buttonPressed = false;
  const unsigned long debounceDelay = 10;

  // Wait until we press the button to start the game
  while (!gameStarted) {
    playInitSound();
    // Add a short delay between each sound repetition
    delay(100);

    // Reset highest score
    if (!digitalRead(button_reset) && !buttonPressed) {
      highestScore = 0;
      tft.fillRect(180, 5, 150, 22, TFT_SKYBLUE);
      tft.setTextColor(TFT_BLACK, TFT_SKYBLUE);
      tft.drawNumber(highestScore, 180, 5);

      buttonPressed = true;
      lastButtonPress = millis();
    }

    // Start the game
    if (!digitalRead(button) && !buttonPressed) {
      gameStarted = true;
      tft.fillRect(0, 0, 480, 25, TFT_SKYBLUE);

      buttonPressed = true;
      lastButtonPress = millis();
    }

    // Check if the debounce delay has passed
    if (buttonPressed && (millis() - lastButtonPress) > debounceDelay) {
      buttonPressed = false;
    }
  }

  // Clear the screen before starting the game
  tft.fillScreen(TFT_SKYBLUE);
}


   

void drawBird(int y) {
  tft.pushImage(50, y, 30, 30, bird, TFT_BLACK);
}
void drawPillars(int x, int y) {
  // Erase the old pillars
  tft.fillRect(last_xP + 51, 0, 10, 284, TFT_SKYBLUE);

  // Draw the new pillars
  if (y > 0 && y < 265) {
    // Upper pillar
    tft.fillRect(x, 0, 50, y - 10, TFT_GREEN);
    tft.drawRect(x, 0, 50, y - 10, TFT_BLACK);  // Outline
    tft.fillRoundRect(x, y - 10, 50, 10, 4, TFT_DARKGREEN);
    tft.drawRoundRect(x, y - 10, 50, 10, 4, TFT_BLACK);  // Outline

    // Lower pillar
    tft.fillRect(x, y + 80, 50, 284 - (y + 80), TFT_GREEN);
    tft.drawRect(x, y + 80, 50, 284 - (y + 80), TFT_BLACK);  // Outline
    tft.fillRoundRect(x, y + 80 - 10, 50, 10, 4, TFT_DARKGREEN);
    tft.drawRoundRect(x, y + 80 - 10, 50, 10, 4, TFT_BLACK);  // Outline
  } else if (y >= 265) {
    //Upper pillar
    tft.fillRect(x, 0, 50, 255, TFT_GREEN);
    tft.drawRect(x, 0, 50, 255, TFT_BLACK);  // Outline
    tft.fillRoundRect(x, 255, 50, 10, 4, TFT_DARKGREEN);
    tft.drawRoundRect(x, 255, 50, 10, 4, TFT_BLACK);  // Outline

    // No lower pillar
  } else {
    // No upper pillar
    tft.fillRect(x, y + 80, 50, 320 - (y + 80), TFT_GREEN);
    tft.drawRect(x, y + 80, 50, 320 - (y + 80), TFT_BLACK);  // Outline
    tft.fillRoundRect(x, y + 80 - 10, 50, 10, 4, TFT_DARKGREEN);
    tft.drawRoundRect(x, y + 80 - 10, 50, 10, 4, TFT_BLACK);  // Outline
  }

  // Update the pillars' last position
  last_xP = x;

  // Draw the score
  tft.setTextColor(TFT_WHITE, TFT_SKYBLUE);
  tft.setTextSize(2);
  tft.setTextPadding(0);
  tft.drawString("Score: " + String(score),60, 10);
}

void playJumpSound() {
  tone(buzzerPin, noteToFrequency('A', 5), 80);
}
void playSuperJumpSound() {
  tone(buzzerPin, noteToFrequency('C', 6), 100);
}

void playScoreSound() {
  tone(buzzerPin, 1000, 100);
}


void playGameOverSound() {
  char notes[] = {'A', 'G', 'F', 'E', 'D', 'C', 'B', 'A'};
  int duration = 200;

  for (int i = 0; i < sizeof(notes)/sizeof(notes[0]); i++) {
    tone(buzzerPin, noteToFrequency(notes[i], 4), duration);
    delay(duration * 1.5);
  }
}

void eraseBird(int y) {
  tft.fillRect(50, y, 30, 30, TFT_SKYBLUE);
}
void playInitSound() {
  int melody[] = {
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5,
    NOTE_F4, NOTE_A4, NOTE_C5, NOTE_F5,
    NOTE_G4, NOTE_B4, NOTE_D5, NOTE_G5,
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5
  };

  int noteDurations[] = {
    8, 8, 8, 8,
    8, 8, 8, 8,
    8, 8, 8, 8,
    4, 4, 4, 4
  };

  for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(speaker, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(speaker);
  }
}



void updateButtonStates() {
  static unsigned long lastPauseButtonPressTime = 0;
  static unsigned long lastJumpButtonPressTime = 0;
  static unsigned long lastSuperJumpButtonPressTime = 0;
  const unsigned long debounceDelay = 100;

  unsigned long currentTime = millis();

  if (!digitalRead(button) && (currentTime - lastJumpButtonPressTime > debounceDelay)) {
    lastJumpButtonPressTime = currentTime;
    if (!isPaused) {
      fallRate = -20;
      playJumpSound();
    }
  }

  if (!digitalRead(pause_button) && (currentTime - lastPauseButtonPressTime > debounceDelay)) {
  lastPauseButtonPressTime = currentTime;
  isPaused = !isPaused;
  if (isPaused) {
    tft.setCursor(tft.width() / 2 - 40, tft.height() / 2); 
    playScoreSound();
    tft.setTextColor(TFT_YELLOW);
    tft.setTextSize(2);
    tft.print("PAUSED");
  } else {
    tft.fillRect(tft.width() / 2 - 40, tft.height() / 2 - 10, 80, 20, TFT_BLACK);
 // Adjusted the position of the rectangle to cover the text
    tft.fillRect(0, 0, tft.width(), tft.height(), TFT_SKYBLUE); // Added this line to clear the screen
  }
}

  if (!digitalRead(superjump_button) && (currentTime - lastSuperJumpButtonPressTime > debounceDelay)) {
    lastSuperJumpButtonPressTime = currentTime;
    if (!isPaused) {
      fallRate = -70;
      playScoreSound();
    }
  }
}





// //======== gameOver() - Custom Function
void gameOver() {
  playGameOverSound(); // Play game over buzzer sound
  
  delay(200);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("GAME OVER", tft.width() / 2, tft.height() / 2 - 20);
  tft.drawString("Score: " + String(score), tft.width() / 2, tft.height() / 2 + 10);

  tft.setTextSize(4);
  tft.setTextDatum(MC_DATUM);
  for (int i = 3; i > 0; i--) {
    tft.fillScreen(TFT_BLACK);
    tft.drawString(String(i), tft.width() / 2, tft.height() / 2);
    delay(500);
  }

  tft.fillScreen(TFT_BLACK);

  if (score > highestScore) {
    highestScore = score;
    EEPROM.write(0, highestScore);
  }
  

  // Resets the variables to start position values
  xP = 479;
  yB = 145;
  fallRate = 0;
  score = 0;
  lastSpeedUpScore = 0;
  movingRate = 10;
  gameStarted = false;

  // Restart game
  tft.fillScreen(TFT_BLACK);
  initiateGame();
  
}
