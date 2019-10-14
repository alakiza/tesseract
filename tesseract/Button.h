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

		inline void BaseInit(const char& pin)
		{
			fpin = pin;
			fKeyPress = false;
			fPressCount = 0;
      fClick = false;

			pinMode(fpin, INPUT);
		}

	public:

		Button(const char pin)
		{
			BaseInit(pin);
			fMaxPressCountToValid = 15;
		}

		Button(const char pin, const char MaxPressCountToValid)
		{
			BaseInit(pin);
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

#endif