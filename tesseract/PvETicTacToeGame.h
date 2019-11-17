#ifndef PVE_TIC_TAC_TOE_GAME_H
#define PVE_TIC_TAC_TOE_GAME_H
#include "BasicGame.h"
#include "Button.h"

    long*** stateMatrix;
    
    class Player
    {
    public:
        uint32_t Color;
        Point3D fCurrentPosition;
        JoyStick* fJoyStick;
        
        Player(uint32_t aColor, JoyStick* aJoyStick)
        {
            Color = aColor;
            fJoyStick = aJoyStick;
            fCurrentPosition.X = 1;
            fCurrentPosition.Y = 1;
            fCurrentPosition.Z = 0;
        }
        
        ~Player()
        {
            
        }
        
        void CheckControls();
        bool TryMakeTurn();
        
    };
    
    void Player::CheckControls()
    {
        if(fJoyStick->AxisX() > 478) ++fCurrentPosition.X;
        if(fJoyStick->AxisX() < -478) --fCurrentPosition.X;
        
        if(fJoyStick->AxisY() > 478) ++fCurrentPosition.Y;
        if(fJoyStick->AxisY() < -478) --fCurrentPosition.Y;
        
        if(fCurrentPosition.X >= cube->LenX()) fCurrentPosition.X = 0;
        if(fCurrentPosition.X < 0) fCurrentPosition.X = cube->LenX()-1;
        
        if(fCurrentPosition.Y >= cube->LenY()) fCurrentPosition.Y = 0;
        if(fCurrentPosition.Y < 0) fCurrentPosition.Y = cube->LenY()-1;
        
    }
    
    bool Player::TryMakeTurn()
    {
        long& elem = stateMatrix[fCurrentPosition.X][fCurrentPosition.Y][fCurrentPosition.Z];
        if(!elem)
        {
          elem = this->Color;
		  return true;
        }
		return false;
    }

    class ArtificialIntelligence
    {
    private:
      Player* fPlayer;
	  int8_t*** fRiskMatrix;
	  
      void IncRiskMatrix(Point3D at, int8_t val, int8_t dx, int8_t dy, int8_t dz);
      
	  long Calculate(long*** Matrix, uint8_t level);
    public:
      ArtificialIntelligence(Player* player);
      
      void PeopleTurnMaked(Point3D point);
      void MakeTurn();

      ~ArtificialIntelligence();
    };
    
    ArtificialIntelligence::ArtificialIntelligence(Player* player)
    {
        fPlayer = player;
		
        fRiskMatrix = new int8_t**[3];
		for(int i = 0; i < 3; ++i)
        {
			fRiskMatrix[i] = new int8_t*[3];
			for(int j = 0; j < 3; ++j)
			{ 
              fRiskMatrix[i][j] = new int8_t[3];
			  for(int k = 0; k < 3; ++k)
                fRiskMatrix[i][j][k] = 0;
			}
		}
        
        fRiskMatrix[1][1][1] = 1;
    }
    
    void ArtificialIntelligence::IncRiskMatrix(Point3D at, int8_t val, int8_t dx, int8_t dy, int8_t dz)
    {
        do
        {
            at.X += dx;
            at.Y += dy;
            at.Z += dz;
            
            int8_t& elem = fRiskMatrix[at.X][at.Y][at.Z];
            if(elem >= 0)
            {
                elem += val;
            }
            else
            {
                break;
            }
        } while (at.X > 0 && at.Y > 0 && at.Z > 0 &&
                 at.X < 2 && at.Y < 2 && at.Z < 2);
    }
    
    void ArtificialIntelligence::PeopleTurnMaked(Point3D point)
    {
        fRiskMatrix[point.X][point.Y][point.Z] = 0xff;
        for(int8_t dx = -1; dx <= 1; ++dx)
            for(int8_t dy = -1; dy <= 1; ++dy)
                for(int8_t dz = -1; dz <= 1; ++dz)
                    if (point.X + dx >= 0 && point.Y + dy >= 0 && point.Z + dz >= 0 &&
                        point.X + dx <  3 && point.Y + dy <  3 && point.Z + dz <  3)
                        {
                            if(!(
                                ((point.X == 1 && point.Y % 2 == 0) && (dx == 1 || dx == -1) && (dy == 1 || dy == -1)) ||
                                ((point.Y == 1 && point.X % 2 == 0) && (dy == 1 || dy == -1) && (dx == 1 || dx == -1))
                                ))
                            IncRiskMatrix(point, 1, dx, dy, dz);
                        }
    }
    
    long ArtificialIntelligence::Calculate(long*** Matrix, uint8_t level)
    {
        if(--level)
        {
            
        }
        return 0;
    }
    
    void ArtificialIntelligence::MakeTurn()
    {
        long*** CopyStateMatrix = CopyMatrix(stateMatrix);
        long*** WeightMatrix    = GenerateMatrix(3, 3, 3);
        
        for(int y = -1; y <= 1; ++y)
            for(int x = -1; x <= 1; ++x)
            {
                if(CopyStateMatrix[x][y][0] == 0)
                {
                    WeightMatrix[x][y][0] = Calculate(CopyStateMatrix, 27);
                }
                else
                {
                    WeightMatrix[x][y][0] = 0x10000000;
                }
            }
                
        long MaxWeight = 0x10000000;
        for(int y = -1; y <= 1; ++y)
            for(int x = -1; x <= 1; ++x)
            {
                if(WeightMatrix[x][y][0] > MaxWeight)
                {
                    MaxWeight = WeightMatrix[x][y][0];
                    fPlayer->fCurrentPosition.X = x;
                    fPlayer->fCurrentPosition.Y = y;
                    fPlayer->fCurrentPosition.Z = 0;
                }
            }
        
        fPlayer->TryMakeTurn();
        
        FreeMatrix(WeightMatrix);
        FreeMatrix(CopyStateMatrix);
    }
    
    ArtificialIntelligence::~ArtificialIntelligence()
    {
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
              delete[] fRiskMatrix[i][j];
            }
            delete[] fRiskMatrix[i];
        }
        delete[] fRiskMatrix;
    }
    
    class PvE_TicTacToe_Game : public IBasicGame
    {
    private:
        Player** fPlayer;
        ArtificialIntelligence* AI;
		int8_t fPlayerNum;
    public:
        PvE_TicTacToe_Game();
        static long analyzeField(long*** Matrix, Point3D point);
        
        void Run(int FirstPlayer, uint32_t FirstColor, uint32_t SecondColor);
        ~PvE_TicTacToe_Game();
    };

    
    PvE_TicTacToe_Game::PvE_TicTacToe_Game()
    {
        fPlayer = new Player*[2];
        for(int i = 0; i < 2; ++i) fPlayer[i] = new Player(0, nullptr);
        
        AI = new ArtificialIntelligence(fPlayer[1]);
    }
    
    PvE_TicTacToe_Game::~PvE_TicTacToe_Game()
    {
        int count = sizeof(fPlayer) / sizeof(Player*);
        for(--count ;count > 0; --count) delete fPlayer[count];
        delete[] fPlayer;
        
        delete AI;
    }
    
    static long PvE_TicTacToe_Game::analyzeField(long*** Matrix, Point3D point)
    {
        long& Value = Matrix[point.X][point.Y][point.Z];
        long LastValue = 0;
        uint8_t count = 1;
        
        int8_t X;
        int8_t Y;
        int8_t Z;

        for(int8_t dz = -1; dz <= 1; ++dz)
            for(int8_t dy = -1; dy <= 1; ++dy)
                for(int8_t dx = -1; dx <= 1; ++dx)
                    if(dx != 0 || dy != 0 || dz != 0)
                    {
                        long LastValue = 0;
                        count = 1;
                
                        for(int8_t i = -2; i < 2; ++i)
                        {
                            X = point.X + i * dx;
                            Y = point.Y + i * dy;
                            Z = point.Z + i * dz;
                        
                            if(X >= 0 && X < 3 &&
                               Y >= 0 && Y < 3 &&
                               Z >= 0 && Z < 3)
                               {
                                   //cube->SetPixelColor(X, Y, Z, Color::FromRGB(255, 255, 255));
                                   //cube->Show();
                                   //delay(100); 
            
                                   if (Matrix[X][Y][Z] == LastValue) 
                                   {
                                       if(LastValue != 0)
                                       {
                                           ++count;
                                           if(count == 3)
                                               return LastValue;
                                       }
                                   }
                                   else
                                   {
                                       LastValue = Matrix[X][Y][Z];
                                       count = 1;
                                   }
                               }
                        }
                    }
        return 0;
    }
    
    void PvE_TicTacToe_Game::Run(const int FirstPlayer, const uint32_t FirstColor, const uint32_t SecondColor)
    {
        bool gameEnd = false;
        
		fPlayerNum = FirstPlayer;
		if(fPlayerNum < 0) fPlayerNum = 0;
		if(fPlayerNum > 1) fPlayerNum = 1;
		
        fPlayer[0]->Color = FirstColor;
        fPlayer[0]->fJoyStick = joySticks[0];
        
        fPlayer[1]->Color = SecondColor;
        fPlayer[1]->fJoyStick = joySticks[1];
        
        stateMatrix = GenerateMatrix(3, 3, 3);
		    long*** VisibleMatrix = GenerateMatrix(3, 3, 3);
        
        cube->SetPixelColor(stateMatrix, 3, 3, 3);
        cube->Show();
        
        while(!gameEnd)
        {
            long PlayerWin = 0;
            
			switch(fPlayerNum)
			{
			case 0:	
				fPlayer[0]->CheckControls();
				if(fPlayer[0]->fJoyStick->Click()) 
					if(fPlayer[0]->TryMakeTurn())
                    {
						fPlayerNum = 1 - fPlayerNum;
                        PlayerWin = analyzeField(fPlayer[0]->fCurrentPosition);
                    }
				break;
			case 1:
				fPlayer[1]->CheckControls();
				if(fPlayer[1]->fJoyStick->Click()) 
					if(fPlayer[1]->TryMakeTurn())
                    {
						fPlayerNum = 1 - fPlayerNum;
                        PlayerWin = analyzeField(fPlayer[1]->fCurrentPosition);
                    }
				break;
			}
            
            if(PlayerWin == FirstColor)
            {
                    lcd.clear();
                    PrintIn(lcd, 0, 2, F("First player"));
                    PrintIn(lcd, 1, 6, F("WIN!"));
                    gameEnd = true;
            }
            if(PlayerWin == SecondColor)
            {
                    lcd.clear();
                    PrintIn(lcd, 0, 2, F("Second player"));
                    PrintIn(lcd, 1, 6, F("WIN!"));
                    gameEnd = true;
            }
			
			CopyMatrix(VisibleMatrix, stateMatrix, 3, 3, 3);           
			VisibleMatrix[fPlayer[fPlayerNum]->fCurrentPosition.X][fPlayer[fPlayerNum]->fCurrentPosition.Y][fPlayer[fPlayerNum]->fCurrentPosition.Z] = (fPlayer[fPlayerNum]->Color+0x007f7f7f) & 0x00ffffff;
			cube->SetPixelColor(VisibleMatrix, 3, 3, 3);
			cube->Show();

			delay(100);
        }

        FreeMatrix(VisibleMatrix, 3, 3, 3);
        FreeMatrix(stateMatrix, 3, 3, 3);
    }
#endif
