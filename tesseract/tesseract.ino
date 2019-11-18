#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 
#include <MsTimer2.h>
#include "Visualizer.h"
#include "cube.h"
#include "Button.h"

#define BUTTON_COUNT 2
#define JOY_STICK_COUNT 2

#define BUTTON_LEFT  0
#define BUTTON_RIGHT 1
#define BUTTON_ENTER 2

#define LCD_ADDR B00100111


LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);
void PrintIn(const LiquidCrystal_I2C& alcd, const char row, const char column, const String message)
{
  alcd.setCursor(column, row);
  alcd.print(message);
}

Button** buttons;
JoyStick** joySticks;
Visualizer* visualizer;
Cube* cube;

inline void ScanButtons()
{
	for(int i = 0; i < BUTTON_COUNT; ++i) buttons[i]->ScanState();    
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
    buttons = new Button*[BUTTON_COUNT];
    for(int i = 0; i < BUTTON_COUNT; ++i) buttons[i] = new Button(2+i, false);
    
    joySticks = new JoyStick*[JOY_STICK_COUNT];
    for(int i = 0; i < JOY_STICK_COUNT; ++i) joySticks[i] = new JoyStick(A0+2*i, A0+1+2*i, 6+i);
    
    visualizer = new Visualizer(8, 27);
    cube = new Cube(3, 3, 3, visualizer);
    
    //joySticks[0] = new JoyStick(A0, A1, 6);
    //joySticks[1] = new JoyStick(A2, A3, 7);
    lcd.init();
    lcd.backlight();
    PrintIn(lcd, 0, 0, F("First String"));
    PrintIn(lcd, 0, 1, F("Second String"));

    MsTimer2::set(5, TimerInterrupt);
    MsTimer2::start();
    
    pinMode(13, OUTPUT);
	//randomSeed(32064);
    randomSeed(analogRead(A7));
}

void loop() 
{
	IToss* toos = FactoryToos::Get();
	int res = toos->Run();
  delete toos;

  IGameable* game = FactoryGames::Get(0);
  game->Run(res, 0x00007F00, 0x007F0000);
//  int countLeft  = 0;
//  int countRight = 0;
//	while(true)
//	{
    //if(joySticks[1]->Click()) ++countRight;
//    countLeft  = (joySticks[0]->AxisX()+512)/127;
//    countRight = (joySticks[1]->AxisX()+512)/127;
//    cube->Clear(false);
//    for(int z = 0; z < 3; ++z)
//      for(int y = 0; y < 3; ++y)
//        for(int x = 0; x < 3; ++x)
//        {
//          static int count = 0;
//          MsTimer2::stop();
//          if(joySticks[0]->Click()) 
//          {
//            lcd.clear();
//            lcd.setCursor(0, 0);
//            lcd.print(String(count++));
//            lcd.setCursor(0, 1);
//            lcd.print(F("Press any key..."));
//          } 
//          cube->SetPixelColor(x, y, z, Color::FromRGB(255, 255, 255));
//          cube->Show();
//          MsTimer2::start();
//          delay(50);
//        }
//    visualizer->Clear(false);
//    visualizer->SetPixelColor(0, countLeft, 255, 1, 1);
//    visualizer->SetPixelColor(18, 18+countRight, 1, 1, 255);
//    visualizer->Show();
//    delay(1);
//	}
 delete game;
 //delete cube;
 //delete visualizer;
 //for(int i = 0; i < JOY_STICK_COUNT; ++i) delete joySticks[i];
 //delete[] joySticks;
 //for(int i = 0; i < BUTTON_COUNT; ++i) delete buttons[i];
 //delete[] buttons;
}
