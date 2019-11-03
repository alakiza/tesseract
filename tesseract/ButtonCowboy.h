#ifndef COWBOY_H
#define COWBOY_H
#define PLAYER_COUNT 2
#include "basictoos.h"
// int buttonPins[PLAYER_COUNT] = {1, 2}; //пока пины абстрактные
// int ledPins[PLAYER_COUNT] = {3, 4};
class ButtonCowboy : public IBasicToos
{

public:
	ButtonCowboy();
	int Run();
	~ButtonCowboy();
	
};

ButtonCowboy::ButtonCowboy()
{

}

int ButtonCowboy::Run()
{	
	visualizer->SetPixelColor(0, 8, 64, 64, 0);
	visualizer->Show();
	delay(1000);
	visualizer->SetPixelColor(9, 17, 128, 128, 0);
	visualizer->Show();
	delay(1000);
	visualizer->SetPixelColor(18, 26, 255, 255, 0);
	visualizer->Show();
	delay(1000);
	visualizer->SetAllPixelColor(255, 255, 255);
	visualizer->Show();
	delay(1000);
	visualizer->Clear();

	// for (int player = 0; player < PLAYER_COUNT; ++player)  
	// {
 //    	pinMode(ledPins[player], OUTPUT); // устанавливаем режим работы вывода, как "выход"
 //    	pinMode(buttonPins[player], INPUT_PULLUP); //подтягиваем входы
 //  	}
  	//delay(random(2000, 7000)); 
  	while(true)
  	{
  		ScanButtons();
  		if (buttons[0]->Pressed())  // если игрок номер «player» нажал кнопку...
	  	{
	 	  	// // то включаем какой-нибудь светодиод(напимер посеридине) цвет 
	    	// digitalWrite(ledPins[player], HIGH);
	     //  	delay(1000);
	     //  	digitalWrite(ledPins[player], LOW);
	     //  	break; // Есть победитель! Выходим из цикла
	  		return 0;
	    }
	    if (buttons[1]->Pressed())  // если игрок номер «player» нажал кнопку...
	  	{
	  		return 1;
	    }
	    delay(1);
	}
}

ButtonCowboy::~ButtonCowboy()
{
	
}

#endif
