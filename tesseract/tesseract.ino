#include "Arduino.h"
#include "Visualizer.h"
#include "cube.h"
#include "Button.h"

#define BUTTON_COUNT 2
#define JOY_STICK_COUNT 2

#define BUTTON_LEFT  0
#define BUTTON_RIGHT 1
#define BUTTON_ENTER 2

Button** buttons;
JoyStick** joySticks;
Visualizer* visualizer;
Cube* cube;

void ScanButtons()
{
	for(int i = 0; i < BUTTON_COUNT; ++i) buttons[i]->ScanState();    
  for(int i = 0; i < JOY_STICK_COUNT; ++i) joySticks[i]->ScanState();
}

#include "FactoryToos.h"

void setup() 
{
    buttons = new Button*[BUTTON_COUNT];
    joySticks = new JoyStick*[JOY_STICK_COUNT];
    visualizer = new Visualizer(8, 27);
    cube = new Cube(3, 3, 3, visualizer);
    for(int i = 0; i < BUTTON_COUNT; ++i) buttons[i] = new Button(2+i, false);
    
    for(int i = 0; i < JOY_STICK_COUNT; ++i) joySticks[i] = new JoyStick(A0+2*i, A0+1+2*i, 6+i);
    //joySticks[0] = new JoyStick(A0, A1, 6);
    //joySticks[1] = new JoyStick(A2, A3, 7);

    pinMode(13, OUTPUT);
    randomSeed(22957);
}

void loop() 
{
	IBasicToos* toos = FactoryToos::Get();
	int res = toos->Run();

  int countLeft  = 0;
  int countRight = 0;
	while(true)
	{
    ScanButtons();
    
    //if(joySticks[0]->Click()) ++countLeft;
    //if(joySticks[1]->Click()) ++countRight;
    countLeft  = (joySticks[0]->AxisX()+512)/128;
    countRight = (joySticks[1]->AxisX()+512)/128;

    visualizer->Clear(false);
    visualizer->SetPixelColor(0, countLeft, 255, 1, 1);
    visualizer->SetPixelColor(18, 18+countRight, 1, 1, 255);
    visualizer->Show();
    delay(1);
	}
}
