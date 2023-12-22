#include <TFT_eSPI.h>
#include "star.c"
#include "bitmaps1.c"
#include <time.h>
using namespace std;

//create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();


// Setting the joystick pins here so we can easily change them
#define JOYSTICK_X_PIN A7
#define JOYSTICK_Y_PIN A6
#define JOYSTICK_BUTTON_PIN 34

int BFState = 0;
int BLState = 0;
int BUState = 0;
int BDState = 0;
float BRState = 0;
// Setting common colors
#define BLACK 0x000000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

int GameReady = 0;
int menuDone = 0;
int PlayerReady = 0;
int countA = 0;
int shipx = 0;
int x = 0;
int cola = 0;
int count = 0;
int e1 = 0;
int e2 = 0;
int e3 = 0;
int e4 = 0;
int check1;
int check2;
int check3;
int check4;
int check5;
int check6;
int check7;
int check8;
int gameLose = 0;
int shootCount = 0;
int enemy1x;
int enemy1y;
int score = 0;
int enemy2x = 0;
int enemy2y = 0;
int enemy3x = 0;
int enemy3y = 0;
int enemy4x = 0;
int enemy4y = 0;
int button_value = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  button_value = digitalRead(JOYSTICK_BUTTON_PIN);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(150, 100);
  tft.setTextColor(BLUE);
  tft.setTextSize(3);
  tft.println("SPACE BLITZ");
  tft.pushImage(220,180,40,60,ship);
  delay(3000);
  tft.fillScreen(TFT_BLACK);
  
  
  
}


void loop() {
  // put your main code here, to run repeatedly:
  // drawing stars
  srand ( time(0) );
  tft.drawBitmap(10, 20, star, 4, 4, WHITE);
  tft.drawBitmap(200, 65, star, 1, 1, WHITE);
  tft.drawBitmap(360, 100, star, 1, 1, WHITE);
  tft.drawBitmap(100, 110, star, 1, 1, WHITE);
  tft.drawBitmap(80, 140, star, 1, 1, WHITE);
  tft.drawBitmap(400, 165, star, 1, 1, WHITE);
  tft.drawBitmap(95, 200, star, 1, 1, WHITE);
  tft.drawBitmap(65, 210, star, 1, 1, WHITE);
  tft.drawBitmap(425, 38, star, 1, 1, WHITE);
  tft.drawBitmap(470, 300, star, 1, 1, WHITE);
  tft.drawBitmap(30, 180, star, 1, 1, WHITE);
  tft.drawBitmap(220, 220, star, 1, 1, WHITE);
  tft.drawBitmap(170, 58, star, 1, 1, WHITE);
  tft.drawBitmap(350, 270, star, 1, 1, WHITE);
  tft.drawBitmap(320, 250, star, 1, 1, WHITE);
  tft.drawBitmap(380, 170, star, 1, 1, WHITE);
  tft.drawBitmap(125, 150, star, 1, 1, WHITE);
  tft.drawBitmap(20, 120, star, 1, 1, WHITE);
  tft.drawBitmap(120, 250, star, 1, 1, WHITE);
  tft.drawBitmap(45, 100, star, 1, 1, WHITE);
  tft.drawBitmap(95, 10, star, 1, 1, WHITE);
  tft.drawBitmap(105, 65, star, 1, 1, WHITE);
  tft.drawBitmap(140, 270, star, 1, 1, WHITE);
  tft.drawBitmap(168, 69, star, 1, 1, WHITE);
  tft.drawBitmap(195, 130, star, 1, 1, WHITE);
  tft.drawBitmap(215, 265, star, 1, 1, WHITE);
  tft.drawBitmap(238, 288, star, 1, 1, WHITE);
  tft.drawBitmap(256, 120, star, 1, 1, WHITE);
  tft.drawBitmap(283, 110, star, 4, 4, WHITE);
  tft.drawBitmap(305, 25, star, 1, 1, WHITE);
  tft.drawBitmap(338, 155, star, 1, 1, WHITE);
  tft.drawBitmap(365, 17, star, 1, 1, WHITE);
  tft.drawBitmap(390, 315, star, 1, 1, WHITE);
  tft.drawBitmap(465, 200, star, 1, 1, WHITE);
  tft.drawBitmap(228, 180, star, 1, 1, WHITE);
  tft.drawBitmap(411, 100, star, 1, 1, WHITE);
  tft.drawBitmap(5, 300, star, 1, 1, WHITE);
  tft.drawBitmap(334, 29, star, 1, 1, WHITE);
  tft.drawBitmap(120, 55, star, 1, 1, WHITE);
  tft.drawBitmap(468, 3, star, 1, 1, WHITE);
  tft.drawBitmap(470, 288, star, 1, 1, WHITE);
  //drawing stars ends



  BFState = digitalRead(JOYSTICK_BUTTON_PIN);
  if (analogRead(JOYSTICK_X_PIN) < 100 or analogRead(JOYSTICK_X_PIN) > 800) {
    BRState = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -1, 1);
  }
  if (analogRead(JOYSTICK_Y_PIN) < 100 or analogRead(JOYSTICK_Y_PIN) > 800) {
    BUState = map(analogRead(JOYSTICK_Y_PIN), 0, 1023, 1, -1);
  }

  tft.fillRect(cola, 260, 40, 60, TFT_BLACK);
  //drawing player
  if (BRState == 1) {
    cola = cola + 4;
    if (cola >= 200) {
      cola = 200;
    }
  }
  if (BRState == -1) {
    cola = cola - 4;
    if (cola <= -240) {
      cola = -240;
    }
  }
  tft.pushImage(cola + 240, 260, 40, 60, ship);

  //drawing enemy1 :)
  if (e1 == 0) {
    enemy1x = random(0, 441);
    enemy1y = -40;
    tft.pushImage(enemy1x, enemy1y, 40, 40, satelite);
    e1 = 1;
  }

  if (e1 == 1) {
    tft.fillRect(enemy1x, enemy1y, 40, 40, TFT_BLACK);
    enemy1y = enemy1y + 3;
    tft.pushImage(enemy1x, enemy1y, 40, 40, satelite);
    if (enemy1y >= 340) {
      e1 = 0;
      score = score + 1;
      printScore();
    }
  }
  //drawing enemy2 :))))))
  if (e2 == 0) {
    enemy2x = -40;
    enemy2y = random(80,250);
    tft.pushImage(enemy2x, enemy2y, 40, 40, sideways_satelite);
    e2 = 1;
  }
  if (e2 == 1) {
    tft.fillRect(enemy2x, enemy2y, 40, 40, TFT_BLACK);
    enemy2y = enemy2y + 4;
    enemy2x = enemy2x + random(3,6);
    tft.pushImage(enemy2x, enemy2y, 40, 40, sideways_satelite);
    if (enemy2y>= 320) {
      e2 = 0;
      score = score + 1;
      printScore();
    }
  }
  // drawing enemy 3 :))))
  if (e3 == 0) {
    enemy3x = random(0, 441);
    enemy3y = -230;
    tft.pushImage(enemy3x, enemy3y, 12, 30, asteroid);
    e3 = 1;
  }
  if (e3 == 1) {
    tft.fillRect(enemy3x, enemy3y, 12, 30, TFT_BLACK);
    enemy3y = enemy3y + 6;
    tft.pushImage(enemy3x, enemy3y, 12, 30, asteroid);
    if (enemy3y >= 340) {
      e3 = 0;
      score = score + 1;
      printScore();
    }
  }
  // drawing enemy 4 :))
  if (e4 == 0) {
    enemy4x = 500;
    enemy4y = random(80,180);
    tft.pushImage(enemy4x, enemy4y, 40, 40, asteroid);
    e4 = 1;
  }
  if (e4 == 1) {
    tft.fillRect(enemy4x, enemy4y, 40, 40, TFT_BLACK);
    enemy4y = enemy4y + 4;
    enemy4x = enemy4x - random(4,8);
    tft.pushImage(enemy4x, enemy4y, 40, 40, sideways_satelite);
    if (enemy4y>= 320) {
      e4 = 0;
      score = score + 1;
      printScore();
    }
  }


  if ((enemy1x-20) <= (cola + 260) && (cola + 260) <= (enemy1x + 60)) {
    if (223 <= enemy1y && enemy1y <= 320) {
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(150, 140);
      tft.setTextColor(RED);
      tft.setTextSize(3);
      tft.println("Game Over!");
      tft.setCursor(235,200);
      tft.println(score);
      delay(5000);
      e1 = 0;
      e2 = 0;
      e3 = 0;
      e4 = 0;
      cola = -20;
      tft.fillScreen(TFT_BLACK);
      score = 0;
      loop();      
    }
  }
  if ((enemy2x-20) <= (cola + 260) && (cola + 260) <= (enemy2x + 60)) {
    if (223 <= enemy2y && enemy2y <= 320) {
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(150, 140);
      tft.setTextColor(RED);
      tft.setTextSize(3);
      tft.println("Game Over!");
      tft.setCursor(235,200);
      tft.println(score);
      delay(5000);
      e1 = 0;
      e2 = 0;
      e3 =0;
      e4 = 0;
      cola = -20;
      tft.fillScreen(TFT_BLACK);
      score = 0;
      loop();
    }
  }
  if ((enemy3x-15) <= (cola + 260) && (cola + 260) <= (enemy3x + 15)) {
    if (230 <= enemy3y && enemy3y <= 320) {
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(150, 140);
      tft.setTextColor(RED);
      tft.setTextSize(3);
      tft.println("Game Over!");
      tft.setCursor(235,200);
      tft.println(score);
      delay(5000);
      e1 = 0;
      e2 = 0;
      e3 = 0;
      e4 = 0;
      cola = -20;
      tft.fillScreen(TFT_BLACK);
      score = 0;
      loop();
    }
  }
  if ((enemy4x-20) <= (cola + 260) && (cola + 260) <= (enemy4x + 30)) {
    if (230 <= enemy4y && enemy4y <= 315) {
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(150, 140);
      tft.setTextColor(RED);
      tft.setTextSize(3);
      tft.println("Game Over!");
      tft.setCursor(235,200);
      tft.println(score);
      delay(5000);
      e1 = 0;
      e2 = 0;
      e3 = 0;
      e4 = 0;
      cola = -20;
      tft.fillScreen(TFT_BLACK);
      score = 0;
      loop();
    }
  }

}


//functions
int player(float BRState, int BFState, int cola) {
  if (BRState == 1) {
    cola = cola + 4;
    if (cola >= 200) {
      cola = 200;
    }
  }
  if (BRState == -1) {
    cola = cola - 4;
    if (cola <= -200) {
      cola = -200;
    }
  }
  tft.pushImage(cola + 240, 260, 40, 60, ship);

  return cola;
}

int enemy1(int en1) {
  if (en1 == 0) {
    enemy1x = random(0, 441);
    enemy1y = -40;
    tft.fillCircle(enemy1x, enemy1y, 20, TFT_RED);
    e1 = 1;
  }

  if (en1 = 1) {
    enemy1y = enemy1y + 3;
    tft.fillCircle(enemy1x, enemy1y, 20, TFT_RED);
  }
}

void printScore(){
  tft.fillRect(448, 4, 20, 10, TFT_BLACK);
  tft.setCursor(400,5);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.println("Score:");
  tft.setCursor(450,5);
  tft.println(score);
}