#ifndef BUTTON_H
#define BUTTON_H

	class Button
	{
	private:
		char fpin;

		char fPressCount;
		char fMaxPressCountToValid;
		bool fClick;

    bool Clicked;

		bool fKeyPress;

		inline void BaseInit(const char& pin, const bool PULLUP)
		{
			fpin = pin;
			fKeyPress = false;
			fPressCount = 0;
      fClick = false;

			if(PULLUP) 
				pinMode(fpin, INPUT_PULLUP);
			else
				pinMode(fpin, INPUT);
		}

	public:

		Button(const char pin, const bool PULLUP)
		{
			BaseInit(pin, PULLUP);
			fMaxPressCountToValid = 15;
		}

		Button(const char pin, const char MaxPressCountToValid, const bool PULLUP)
		{
			BaseInit(pin, PULLUP);
			fMaxPressCountToValid = MaxPressCountToValid;
		}

		void ScanState()
		{
			if ( fClick == (!digitalRead(fpin)) ) 
			{
			    fPressCount = 0;
			}
			else 
			{
			    ++fPressCount;

			    if ( fPressCount >= fMaxPressCountToValid ) 
			    {
             	fKeyPress = fClick;
              Clicked = !fClick;
			      	fClick = !fClick;
			      	fPressCount = 0;
			    }   
			}
		}

		inline bool Pressed()
		{
			return fKeyPress;
		}

   inline bool Click()
   {
    bool tmp = Clicked;
    if(tmp) Clicked = false;
    return tmp;
   }

   void ResetClick()
   {
       fClick = false;
   }

		char pin()
		{
			return fpin;
		}

		~Button()
		{
			
		}
	};

  class JoyStick
  {
    private:
      char fPinAxisX;
      char fPinAxisY;
      char fPinButton;
      int fAxisX;
      int fAxisY;

      Button* button;
    public:
      JoyStick(int PinAxisX, int PinAxisY, char PinButton);
      
      void ScanState();
      
      inline int AxisX()
      {
        return fAxisX;
      }
      inline int AxisY()
      {
        return fAxisY;
      }
      
      inline bool Pressed()
      {
        return button->Pressed();
      }
      inline bool Click()
      {
        return button->Click();
      }
      inline void ResetClick()
      {
        button->ResetClick();
      }
      inline char pinAxisX()
      {
        return fPinAxisX;
      }
      inline char pinAxisY()
      {
        return fPinAxisY;
      }
      inline char pinButton()
      {
        return fPinButton;
      }
      
      ~JoyStick();
  };

  JoyStick::JoyStick(int PinAxisX, int PinAxisY, char PinButton)
  {
    fPinAxisX = PinAxisX;
    fPinAxisY = PinAxisY;
    fPinButton = PinButton;

    pinMode(fPinAxisX, INPUT_PULLUP);
    pinMode(fPinAxisY, INPUT_PULLUP);
    button = new Button(fPinButton, false);
  }
  
  void JoyStick::ScanState()
  {
    fAxisX = analogRead(fPinAxisX) - 512;
    fAxisY = analogRead(fPinAxisY) - 512;
    button->ScanState();
  }

  JoyStick::~JoyStick()
  {
    delete button;
  }
  

#endif
