#include "Arduino.h"
#include "Visualizer.h"
#include "cube.h"
#include "Button.h"

#define BUTTON_COUNT 3

#define BUTTON_LEFT  0
#define BUTTON_RIGHT 1
#define BUTTON_ENTER 2

Button** buttons;
Visualizer* visualizer;
Cube* cube;

void ScanButtons()
{
	for(int i = 0; i < BUTTON_COUNT; ++i) buttons[i]->ScanState();    
}

#include "FactoryToos.h"

void setup() 
{
    buttons = new Button*[BUTTON_COUNT];
    visualizer = new Visualizer(8, 27);
    cube = new Cube(3, 3, 3, visualizer);
    for(int i = 0; i < BUTTON_COUNT; ++i) buttons[i] = new Button(2+i, false);

    pinMode(13, OUTPUT);
    randomSeed(22957);
}

void loop() 
{
	IBasicToos* toos = FactoryToos::Get();
	int res = toos->Run();
	while(true)
	{
    if(res) visualizer->SetAllPixelColor(255, 255, 255);
		visualizer->SetAllPixelColor(random(255), random(255), random(255));
		visualizer->Show();
    delay(1000);
	}
}
