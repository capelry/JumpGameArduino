#include "FastLED.h"

CRGB leds[40];
int r, b, g;
int charPos;
int charJump = 22;
int charStand = 17;

long boxLength;
long boxHeight;
int boxCooldown;
int gameSpace[40];

bool lose;
bool win;

// play input
const int buttonPin = 2;
int buttonState = 0;

int gameSpeed;
int frameCount;
int gameSpeedMod;
int gameSpeedWin;


void setup() { 
  FastLED.addLeds<NEOPIXEL, 7>(leds, 40); 
  // initialize serial to read unconnected analog pin for true random input
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(buttonPin, INPUT);
}
void loop() { 

  // Initialize LEDS and gameSpace to 0
  for(int i=0; i<40; i++)
        {
          leds[i].setRGB(0, 0, 0);
          gameSpace[i] = 0;
        }

  boxLength = 0;
  boxCooldown = 0;

  gameSpeed = 100; //ms
  gameSpeedMod = 10; // amount of increase in speed
  gameSpeedWin = 10; // win condition
  frameCount = 0;
  
  win = false;
  lose = false;
  while (not lose && not win)
  {

    // count frames to increment game speed
    frameCount += 1;
    
    // check user input
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW)
    {
      charPos = charStand;
    }
    else if (buttonState == HIGH)
    {
      charPos = charJump;
    }
    
    // if current box run out create new box
    if (boxLength == 0 && boxCooldown == 0)
    {
      // set box length between 1 and 5
      boxLength = random(1, 6);
      // set box height between 0 and 1
      boxHeight = random(2);
  
    }
  
  
    // shift box in game space
    // level 0
    for (int i=19; i>0; i--)
    {
      gameSpace[i] = gameSpace[i-1];
    }
    // level 1
    for (int i=20; i<39; i++)
    {
      gameSpace[i] = gameSpace[i+1];
    }
    
    // add box to game space
    if (boxLength > 0)
    {
      if (boxHeight == 0)
      {
        // start of box at height 0 is index 0
        gameSpace[0] = 1;
      }
      else if (boxHeight == 1)
      {
        // start of heigh 1 is 19 due to array wrapping in LED
        gameSpace[39] = 1;
      }
      // reduce box length
      boxLength -= 1;
      // if end of box start box cooldown
      if (boxLength == 0)
      {
        boxCooldown = random(3,10);
      }
    }
    else if (boxLength == 0)
    {
      // clear box and reduce cooldown
      gameSpace[0] = 0;
      gameSpace[39] = 0;
      boxCooldown -= 1;
    }
  
    // draw box to leds
    for (int i = 0; i < 40; i++)
    {
      if (gameSpace[i] == 1)
      {
        leds[i].setRGB(0, 0, 50);
      }
      else if (gameSpace[i] == 0)
      {
        leds[i].setRGB(0, 0, 0);
      }
    }

    // draw char to leds
    leds[charPos].setRGB(0, 50, 0);
  
  
  
    FastLED.show();
    delay(100);

    // check lose condition
    if (gameSpace[charPos] == 1)
    {
      lose = true;
    }
  
    // game speed
    delay(gameSpeed);
  
    if (frameCount == 20)
    {
      frameCount = 0;
      gameSpeed -= gameSpeedMod;
    }
  
    if (gameSpeed <= gameSpeedWin)
    {
      win = true;
    }
  
  
  }

  
  
  if (lose)
  {
    // lose animation

    for (int i = 0; i < 3; i++)
    {
      for (int j=0; j < 40; j++)
      {
        leds[j].setRGB(50, 0, 0);
      }
      FastLED.show();
      delay(200);

      for (int j=0; j < 40; j++)
      {
        leds[j].setRGB(0, 0, 0);
      }
      FastLED.show();
      delay(200);
    }
    
  }

  if (win)
  {
    // lose animation

    for (int i = 0; i < 3; i++)
    {
      for (int j=0; j < 40; j++)
      {
        leds[j].setRGB(0, 50, 0);
      }
      FastLED.show();
      delay(200);

      for (int j=0; j < 40; j++)
      {
        leds[j].setRGB(0, 0, 0);
      }
      FastLED.show();
      delay(200);
    }
    
  }
}
