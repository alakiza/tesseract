#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MsTimer2.h>
#include "Visualizer.h"
#include "cube.h"
#include "Button.h"

//#define BUTTON_COUNT 2
#define JOY_STICK_COUNT 2

#define BUTTON_LEFT  0
#define BUTTON_RIGHT 1
#define BUTTON_ENTER 2

#define LCD_ADDR B00100111


//LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);
//void PrintIn(const LiquidCrystal_I2C& alcd, const uint8_t row, const uint8_t column, const String message)
//{
//  alcd.setCursor(column, row);
//  alcd.print(message);
//}

//Button** buttons;
JoyStick** joySticks;
Visualizer* visualizer;
Cube* cube;

inline void ScanButtons()
{
	//for(int i = 0; i < BUTTON_COUNT; ++i) buttons[i]->ScanState();
  for(int i = 0; i < JOY_STICK_COUNT; ++i) joySticks[i]->ScanState();
}

void TimerInterrupt()
{
  ScanButtons();
}

#include "factoryToss.h"
#include "factoryGames.h"

void setup()
{
    //buttons = new Button*[BUTTON_COUNT];
    //for(int i = 0; i < BUTTON_COUNT; ++i) buttons[i] = new Button(2+i, false);

    joySticks = new JoyStick*[JOY_STICK_COUNT];
    for(int i = 0; i < JOY_STICK_COUNT; ++i) joySticks[1-i] = new JoyStick(A0+2*i, A0+1+2*i, 6+i);

    visualizer = new Visualizer(8, 27);
    cube = new Cube(3, 3, 3, visualizer);
//    lcd.init();
//    lcd.backlight();
//    PrintIn(lcd, 0, 0, F("First String"));
//    PrintIn(lcd, 0, 1, F("Second String"));

    MsTimer2::set(5, TimerInterrupt);
    MsTimer2::start();

    pinMode(13, OUTPUT);
    randomSeed(analogRead(A7));

    Serial.begin(9600);
}

void loop()
{
	IToss* toos = FactoryToos::Get();
	int res = toos->Run();
  delete toos;

  IGameable* game = FactoryGames::Get(0);
  Serial.println(F("Game created"));
  game->Run(res, 0x00000700, 0x00070000);
  delete game;
}
