#ifndef COWBOY_H
#define COWBOY_H
#define PLAYER_COUNT 2
#include "IToss.h"
// int buttonPins[PLAYER_COUNT] = {1, 2}; //пока пины абстрактные
// int ledPins[PLAYER_COUNT] = {3, 4};
class ButtonCowboy : public IToss
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
  //lcd.clear();
  //PrintIn(lcd, 0, 0, F("       3        "));
	visualizer->SetPixelColor(0, 8, 32, 32, 0);
	visualizer->Show();
	delay(1000);
  
  //lcd.clear();
  //PrintIn(lcd, 0, 0, F("       2        "));
	visualizer->SetPixelColor(9, 17, 64, 64, 0);
	visualizer->Show();
	delay(1000);
  
  //lcd.clear();
  //PrintIn(lcd, 0, 0, F("       1        "));
	visualizer->SetPixelColor(18, 26, 96, 96, 0);
	visualizer->Show();
	delay(1000);
  
  //lcd.clear();
  //PrintIn(lcd, 0, 0, F("     PUSH!      "));
	visualizer->SetAllPixelColor(255, 255, 255);
	visualizer->Show();

	// for (int player = 0; player < PLAYER_COUNT; ++player)  
	// {
 //    	pinMode(ledPins[player], OUTPUT); // устанавливаем режим работы вывода, как "выход"
 //    	pinMode(buttonPins[player], INPUT_PULLUP); //подтягиваем входы
 //  	}
  	//delay(random(2000, 7000)); 
    joySticks[0]->ResetClick();
    joySticks[1]->ResetClick();
    int res = -1;
  	while(res == -1)
  	{
      Serial.println(res);
  		if (joySticks[0]->Pressed())  // если игрок номер «player» нажал кнопку...
	  	{
	 	  	// // то включаем какой-нибудь светодиод(напимер посеридине) цвет 
	    	// digitalWrite(ledPins[player], HIGH);
	     //  	delay(1000);
	     //  	digitalWrite(ledPins[player], LOW);
	     //  	break; // Есть победитель! Выходим из цикла
	  		res = 0;
	    }
	    if (joySticks[1]->Pressed())  // если игрок номер «player» нажал кнопку...
	  	{
	  		res = 1;
	    }
     Serial.println(res);
      delay(1);
	}
  Serial.println("OK");
  joySticks[0]->ResetClick();
  joySticks[1]->ResetClick();
  //lcd.clear();
  visualizer->Clear(true);
  return res;
}

ButtonCowboy::~ButtonCowboy()
{
	
}

#endif
