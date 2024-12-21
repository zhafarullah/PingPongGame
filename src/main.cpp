#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <LedControl.h>
#include "pitches.h"
#include "angka.h"
#include "angka2.h" 
#include "sound.h"

#define BUILTIN_LED_PIN 38 
#define buzzerPin 7
#define DIN_PIN 11
#define CS_PIN 10
#define CLK_PIN 9
#define POT1_PIN 6 
#define POT2_PIN 5 
#define BUTTON1_PIN 8
#define BUTTON2_PIN 18 
#define BUTTONp1_PIN 4
#define BUTTONp2_PIN 1
#define ON 32
#define ONX 64
#define OFF 0
#define MAX_DEVICES 4  

MD_Parola display = MD_Parola(MD_MAX72XX::FC16_HW, DIN_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 4);

int ballX = 12, ballY = 3, ballDirX = 1, ballDirY = 1;
int previousSet1 = 0, previousSet2 = 0;
int player1Pos = 3, player2Pos = 3;
int skorp1=0, skorp2=0;
int lastscorer=0;
int set1 = 0, set2 = 0; 

const int moduleMap[] = {
  3, 3, 3, 3, 3, 3, 3, 3,  
  2, 2, 2, 2, 2, 2, 2, 2,  
  1, 1, 1, 1, 1, 1, 1, 1, 
  0, 0, 0, 0, 0, 0, 0, 0   
};

const unsigned long wallDuration = 5000; 
unsigned long wallStartTime = 0;  
unsigned long player1SmashWindowStartTime  = 0, player2SmashWindowStartTime  = 0; 
unsigned long resetDelayStartTime = 0;  
unsigned long normalBallUpdateInterval = 80, smashBallUpdateInterval = 27;    
unsigned long smashWindowStart = 0, smashWindowDuration = 700; 
unsigned long previousMillis = 0;  
unsigned long displayDuration = 3000;  
unsigned long ballUpdateInterval = 80; 
unsigned long lastBallUpdate = 0;

bool player1SmashWindowActive = false, player2SmashWindowActive = false;     
bool smashActive1 = false, smashActive2 = false;  
bool buttonPressed = false;                   
bool ballIsHeld = true;   
bool isFirstRun = true; 
bool showWall = false;
bool soundPlayed = false; 
bool isBallFast = false;
bool lastButton1State = LOW, lastButton2State = LOW; 

void drawWall(int row);
void clearAllDisplays();
void drawPlayer1(int row);
void drawPlayer2(int row);
void drawBall(int x, int y);
void updateBall();

void resetBall() {
  ballX = 16;
  ballY = random(0, 8);

  if (lastscorer == 1) {
    ballDirX = -1;  
  } else if (lastscorer == 2) {
    ballDirX = 1;   
  } else {
    ballDirX = (random(0, 2) == 0) ? 1 : -1; 
  }

  if (ballY == 0) {
    ballDirY = 1;
  } else if (ballY == 7) {
    ballDirY = -1;
  } else {
    ballDirY = (random(0, 2) == 0) ? 1 : -1;
  }

  resetDelayStartTime = millis();
  ballIsHeld = true;
}

void WinDecision() {
  if (set1 >= 3 || set2 >= 3) {
    display.displayClear();
    String winnerMessage = (set1 >= 3) ? "P1 WIN" : "P2 WIN";
    if (!soundPlayed) {
      victory(buzzerPin);  
      soundPlayed = true;  
    }
    display.displayText(winnerMessage.c_str(), PA_CENTER, 50, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    while (!display.displayAnimate()) {
    }
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= displayDuration) {
      display.displayClear();
      ESP.restart();
    }
  }
}

void setup() {
  display.begin();
  display.setIntensity(10);
  display.displayClear();
  for (int i = 0; i < 4; i++) {
    lc.shutdown(i, false); 
    lc.setIntensity(0, 9); 
    lc.setIntensity(1, 8);
    lc.setIntensity(2, 8); 
    lc.setIntensity(3, 8); 
    lc.clearDisplay(i);   
  }

  pinMode(BUTTON1_PIN, INPUT_PULLUP); 
  pinMode(BUTTON2_PIN, INPUT_PULLUP); 
  pinMode(BUILTIN_LED_PIN, OUTPUT); 
  pinMode(BUTTONp1_PIN, INPUT_PULLUP); 
  pinMode(BUTTONp2_PIN, INPUT_PULLUP);
  
  Serial.begin(115200);
  analogReadResolution(12);
  resetBall();
}

bool set1LEDStatus = false;
bool set2LEDStatus = false;

void loop() {
  unsigned long currentMillis = millis();
   if (isFirstRun) {
    display.displayText("ZHAFARULLAH AHMAD", PA_CENTER, 50, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    while (!display.displayAnimate()) {} 

    display.displayReset(); 

    display.displayText("5024221004", PA_CENTER, 50, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    while (!display.displayAnimate()) {} 
    display.displayReset(); 

    isFirstRun = false;
  }

WinDecision();

  if (currentMillis - lastBallUpdate >= ballUpdateInterval) {
    lastBallUpdate = currentMillis;
    updateBall();
  }

  int pot1Value = analogRead(POT1_PIN);
  player1Pos = map(pot1Value, 0, 4095, 0, 5); 
  int pot2Value = analogRead(POT2_PIN);
  player2Pos = map(pot2Value, 0, 4095, 0, 5); 

  set1LEDStatus = (set1 >= 1);
  set2LEDStatus = (set2 >= 1);

  clearAllDisplays(); 
  if (set1LEDStatus) {
    lc.setLed(3, 7, 0, true);
      if (set1>=2){
        lc.setLed(3, 6, 0, true);
          if (set1>=3){
            lc.setLed(3, 5, 0, true);
              if (set1>=4){
                lc.setLed(3, 4, 0, true);
              }
          }
      }  
  } 
  if (set2LEDStatus) {
    lc.setLed(0, 0, 7, true);
      if (set2>=2){
        lc.setLed(0, 1, 7, true);
          if (set2>=3){
            lc.setLed(0, 2, 7, true);
              if (set2>=4){
                lc.setLed(0, 3, 7, true);
              }
          }
      }  
  } 

  drawPlayer1(player1Pos);
  drawPlayer2(player2Pos);
  drawBall(ballX, ballY);
  bool buttonp1State = digitalRead(BUTTONp1_PIN);
  
  bool buttonp2State = digitalRead(BUTTONp2_PIN);

  if (buttonp1State == LOW && skorp1 <= skorp2 - 2) {
    showWall = true;  
    wallStartTime = millis();  
  }

  if (buttonp2State == LOW && skorp2 <= skorp1 - 2) {
    showWall = true;  
    wallStartTime = millis();  
  }

if ((millis() - wallStartTime >= wallDuration) || ballIsHeld) {
  showWall = false;
}

  if (showWall) {
    int wallRow = 0;  
    drawWall(wallRow);
  }

  AngkaMirror(lc, skorp2);
  tampilkanAngka(lc, skorp1);

  if (ballX > 26) {
    skorp1 += 1;
    lastscorer=1;
    resetBall();

    if (skorp1 >= 10 && skorp2 >= 10) {
      if (skorp1 >= skorp2 + 2) {
        set1 += 1;
        skorp1 = 0;
        skorp2 = 0;
      }
    } else if (skorp1 >= 11) {
      set1 += 1;
      skorp1 = 0;
      skorp2 = 0;
    }
    smashActive1 = false;
    smashActive2 = false;
    ballUpdateInterval = normalBallUpdateInterval;
  }

  if (ballX < 5) {
    skorp2 += 1;
    lastscorer=2;
    resetBall();

    if (skorp2 >= 10 && skorp1 >= 10) {
      if (skorp2 >= skorp1 + 2) {
        set2 += 1;
        skorp1 = 0;
        skorp2 = 0;
      }
    } else if (skorp2 >= 11) {
      set2 += 1;
      skorp1 = 0;
      skorp2 = 0;
    }

    smashActive1 = false;
    smashActive2 = false;
    ballUpdateInterval = normalBallUpdateInterval;
  }

  delay(10);
}

void clearAllDisplays() {
  for (int i = 0; i < 4; i++) {
    lc.clearDisplay(i);
  }
}

void drawPlayer1(int row) {
  row = constrain(row, 0, 5);
  lc.setLed(0, row, 0, true);     
  lc.setLed(0, row + 1, 0, true);
  lc.setLed(0, row + 2, 0, true);
}

void drawPlayer2(int row) {
  row = constrain(row, 0, 5);
  lc.setLed(3, row, 7, true);     
  lc.setLed(3, row + 1, 7, true); 
  lc.setLed(3, row + 2, 7, true); 
}

void drawWall(int startRow) {

    for (int i = startRow; i < 8; i++) {  
      lc.setLed(2, i, 7, true);  
    }

    if (ballX == 15 && ballY >= 0 && ballY <= 7) {
        if (ballDirX == 1) {
            ballDirX = -1; 
        } else if (ballDirX == -1) {
            ballDirX = 1; 
        }
        
        int maxYOffset = 3; 
        int minYOffset = -3; 
       
        ballDirY = random(minYOffset, maxYOffset + 1); 

        ballY = constrain(ballY + ballDirY, 0, 7);
    }
}

void drawBall(int x, int y) {
  x = constrain(x, 0, 31);       
  y = constrain(y, 0, 7);        
  int module = moduleMap[x];     
  int localX = x % 8;          
  lc.setLed(module, y, localX, true);
}

void updateBall() {
  if (ballIsHeld) {
    if ((set1 > previousSet1) || (set2 > previousSet2)) {
      setSound(buzzerPin);  
      previousSet1 = set1;  
      previousSet2 = set2;  
    } 
    else if ((skorp1==0)&&(skorp2==0)){

    }else {
      smashSound(buzzerPin);  
    }

    if (millis() - resetDelayStartTime >= 1000) {
      ballIsHeld = false;
    }
  } else {
  ballX += ballDirX;
  ballY += ballDirY;
ballY = constrain(ballY, 0, 7);
  if (ballY <= 0 || ballY >= 7) {
    ballDirY *= -1; 
  }

  bool currentButton1State = digitalRead(BUTTON1_PIN);

if ((ballX >= 23 && ballX <= 25) && 
    (ballY >= player1Pos && ballY <= player1Pos + 2)) {
  player1SmashWindowActive  = true;
  player1SmashWindowStartTime  = millis();
  #ifdef RGB_BUILTIN
  neopixelWrite(RGB_BUILTIN, OFF, ON, OFF);
  #endif
}

if (!player1SmashWindowStartTime  || millis() - player1SmashWindowStartTime > smashWindowDuration) {
  if (!player2SmashWindowActive) { 
    #ifdef RGB_BUILTIN
    neopixelWrite(RGB_BUILTIN, OFF, OFF, OFF);
    #endif
  }
}

 if (player1SmashWindowActive && 
    millis() - player1SmashWindowStartTime <= smashWindowDuration &&
    (ballX >= 23 && ballX <= 25) && 
    (ballY >= player1Pos && ballY <= player1Pos + 2)) {
    if (currentButton1State == HIGH) {
      Serial.println("Player 1 Smash Successful!");
      smashActive1 = true;
      player1SmashWindowActive  = false; 
    }
  } else if (millis() - player1SmashWindowStartTime  > smashWindowDuration) {
    player1SmashWindowActive  = false; 
  }

  if (ballX == 24 && (ballY >= player1Pos && ballY <= player1Pos + 2)) {
    ballDirX *= -1; 
    
    if (ballY == player1Pos) {
      ballDirY = -1;
    } else if (ballY == player1Pos + 1) {
       ballDirY = 0;
    } else if (ballY == player1Pos + 2) {
      ballDirY = -1;
    }

    if(smashActive1){
    ballUpdateInterval = smashBallUpdateInterval;
    ballDirY=0;
    }
    if (smashActive2){
      ballUpdateInterval=normalBallUpdateInterval;
      ballDirY = (random(0, 2) == 0) ? 1 : -1;
      smashActive2=false;
    }

  }

  lastButton1State = currentButton1State; 

  bool currentButton2State = digitalRead(BUTTON2_PIN);

  if ((ballX <= 10 && ballX >= 6) && 
      (ballY >= player2Pos && ballY <= player2Pos + 2)) {
    player2SmashWindowActive  = true;
    player2SmashWindowStartTime  = millis();
    #ifdef RGB_BUILTIN
    neopixelWrite(RGB_BUILTIN, OFF, OFF, ON);
    #endif
  }

if (!player2SmashWindowActive || millis() - player2SmashWindowStartTime > smashWindowDuration) {
  if (!player1SmashWindowActive) { 
    #ifdef RGB_BUILTIN
    neopixelWrite(RGB_BUILTIN, OFF, OFF, OFF);
    #endif
  }
}

 if (player2SmashWindowActive && 
    millis() - player2SmashWindowStartTime <= smashWindowDuration &&
    (ballX <= 10 && ballX >= 6) && 
    (ballY >= player2Pos && ballY <= player2Pos + 2)) {
    if (currentButton2State == HIGH) {
      Serial.println("Player 2 Smash Successful!");
      smashActive2 = true;
      player2SmashWindowActive  = false; 
    }
  } else if (millis() - player2SmashWindowStartTime  > smashWindowDuration) {
    player2SmashWindowActive  = false; 
  }

  if (ballX == 7 && (ballY >= player2Pos && ballY <= player2Pos + 2)) {
    ballDirX *= -1; 

    if (ballY == player2Pos) {
        ballDirY = -1;
    } else if (ballY == player2Pos + 1) {
      ballDirY = 0;
    } else if (ballY == player2Pos + 2) {
      ballDirY = -1;
    }

    if(smashActive2){
      ballUpdateInterval = smashBallUpdateInterval;
      ballDirY=0;
    }
    if(smashActive1){
      ballDirY = (random(0, 2) == 0) ? 1 : -1;
      ballUpdateInterval=normalBallUpdateInterval;
      smashActive1=false;
    }
  }
  lastButton2State = currentButton2State;
}
}