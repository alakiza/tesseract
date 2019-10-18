#include "Arduino.h"
#include "Visualizer.h"
#include "Button.h"

#define BUTTON_COUNT 3

#define BUTTON_LEFT  0
#define BUTTON_RIGHT 1
#define BUTTON_ENTER 2

Button** buttons;
Visualizer visualizer(8, 27);
#include "FactoryToos.h"

void ScanButtons()
{
	for(char i = 0; i < BUTTON_COUNT; ++i) buttons[i]->ScanState();
    
}

void setup() 
{
    buttons = new Button*[BUTTON_COUNT];
    for(char i = 0; i < BUTTON_COUNT; ++i) buttons[i] = new Button(4+i, true);

    pinMode(13, OUTPUT);
    randomSeed(22957);
}

void loop() 
{
	IBasicToos* toos = FactoryToos::Get();
	int res = toos->Run();

	while(true)
	{
		ScanButtons();
		if(res) 
			digitalWrite(13, HIGH);
		else
			digitalWrite(13, LOW);
		visualizer.SetAllPixelColor(random(255), random(255), random(255));
		visualizer.Show();
	   delay(100);
	}
}
