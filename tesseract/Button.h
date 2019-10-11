#ifndef BUTTON_H
#define BUTTON_H

	class Button
	{
	private:
		char fpin;

		char fPressCount;
		char fMaxPressCountToValid;
		bool fState;

		bool fKeyPress;

		inline void BaseInit(const char& pin)
		{
			fpin = pin;
			fKeyPress = false;
			fPressCount = 0;

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
			if ( fState == (!digitalRead(fpin)) ) 
			{
			    fPressCount = 0;
			}
			else 
			{
			    ++fPressCount;

			    if ( fPressCount >= fMaxPressCountToValid ) 
			    {
            		fKeyPress = fState;
			      	fState = !fState;
			      	fPressCount = 0;
			    }   
			}
		}

		bool Pressed()
		{
			return fKeyPress;
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
