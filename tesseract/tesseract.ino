#include "Arduino.h"
#include "Visualizer.h"
#include "Button.h"

#define BUTTON_COUNT 3

#define BUTTON_LEFT  0
#define BUTTON_RIGHT 1
#define BUTTON_ENTER 2

class Gereb
{
  private:
  
  public:
  Gereb();
  ~Gereb();
};

Button** buttons;

void ScanButtons()
{
	for(char i = 0; i < BUTTON_COUNT; ++i) buttons[i]->ScanState();
    
}

void setup() 
{
  buttons = new Button*[BUTTON_COUNT];
  for(char i = 0; i < BUTTON_COUNT; ++i) buttons[i] = new Button(4+i);

  pinMode(13, OUTPUT);
}

void loop() 
{
	ScanButtons();
	if(buttons[BUTTON_LEFT]->Pressed()) 
		digitalWrite(13, HIGH);
	else
		digitalWrite(13, LOW);
   delay(1);
}
