#ifndef LCD_HELPER_H
#define LCD_HELPER_H

class IBasicGame
{
public:
  virtual void Run(int FirstPlayer, uint32_t FirstColor, uint32_t SecondColor) = 0;
};
  
#endif
