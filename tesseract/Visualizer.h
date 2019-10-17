#ifndef VISUALIZER_H
#define VISUALIZER_H
#include "Adafruit_NeoPixel.h";
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

	class Visualizer
	{
	private:
		int fDataPin;
		int fLEDCount;

		Adafruit_NeoPixel* pixels;

	public:
		Visualizer(const int DataPin, const int LEDCount)
		{
			fDataPin = DataPin;
			fLEDCount = LEDCount;

			#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  				clock_prescale_set(clock_div_1);
			#endif

			pixels = new Adafruit_NeoPixel(fLEDCount, fDataPin, NEO_GRB + NEO_KHZ800);
			pixels->begin();
			Clear();
		}

		void Show()
		{
			pixels->show();
		}

		void Clear()
		{
			pixels->clear();
		}

		void SetAllPixelColor(char red, char green, char blue)
		{
			for(int i = 0; i < fLEDCount; ++i)
			{
				SetPixelColor(i, red, green, blue);
			}
		}

		void SetPixelColor(int num, char red, char green, char blue)
		{
			pixels->setPixelColor(num, pixels->Color(red, green, blue));
		}

		void SetPixelColor(int from, int to, char red, char green, char blue)
		{
			for(int i = from; i < to; ++i)
			{
				SetPixelColor(i, red, green, blue);
			}
		}

		~Visualizer()
		{
			//delete pixels;
		}
	};

#endif