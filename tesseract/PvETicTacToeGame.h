#ifndef PVE_TIC_TAC_TOE_GAME_H
#define PVE_TIC_TAC_TOE_GAME_H
#include "BasicGame.h"
#include "Button.h"

    bool gameEnd = 0;
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
            fCurrentPosition.X = 0;
            fCurrentPosition.Y = 0;
            fCurrentPosition.Z = 0;
        }
        
        ~Player()
        {
            
        }
        
        void CheckControls();
        bool TryMakeTurn(Point3D point);
        
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
    
    void Player::CheckControls()
    {
        
    }
    
    bool Player::TryMakeTurn(Point3D point)
    {
        
    }

    class ArtificialIntelligence
    {
    private:
      Player* fPlayer;
    public:
      ArtificialIntelligence(Player* player);
      
      Point3D Calculate();
      void MakeTurn(Point3D point);

      ~ArtificialIntelligence();
    };
    
    ArtificialIntelligence::ArtificialIntelligence(Player* player)
    {
        
    }
    
    Point3D ArtificialIntelligence::Calculate()
    {
        
    }
    
    void ArtificialIntelligence::MakeTurn(Point3D point)
    {
        
    }
    
    ArtificialIntelligence::~ArtificialIntelligence()
    {
        
    }
    
    class PvE_TicTacToe_Game : public IBasicGame
    {
    private:
        Player** fPlayer;
    public:
        PvE_TicTacToe_Game();
        int analyzeField();
        
        void Run(int FirstPlayer, uint32_t FirstColor, uint32_t SecondColor);
        ~PvE_TicTacToe_Game();
    };

    
    PvE_TicTacToe_Game::PvE_TicTacToe_Game()
    {
        fPlayer = new Player*[2];
        for(int i = 0; i < 2; ++i) fPlayer[i] = new Player(0, nullptr);
    }
    
    PvE_TicTacToe_Game::~PvE_TicTacToe_Game()
    {
        int count = sizeof(fPlayer) / sizeof(Player*);
        for(--count ;count > 0; --count) delete fPlayer[count];
        delete[] fPlayer;
    }
    
    void PvE_TicTacToe_Game::Run(int FirstPlayer, uint32_t FirstColor, uint32_t SecondColor)
    {
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
            for(int i = 0; i < 1; ++i) fPlayer[i]->CheckControls();

            
            
            CopyMatrix(VisibleMatrix, stateMatrix, 3, 3, 3);           
            VisibleMatrix[fPlayer[0]->fCurrentPosition.X][fPlayer[0]->fCurrentPosition.Y][fPlayer[0]->fCurrentPosition.Z] = (fPlayer[0]->Color+0x007f7f7f) & 0x00ffffff;
            cube->SetPixelColor(VisibleMatrix, 3, 3, 3);
            cube->Show();

            delay(100);
        }

        FreeMatrix(VisibleMatrix, 3, 3, 3);
        FreeMatrix(stateMatrix, 3, 3, 3);
    }
#endif
