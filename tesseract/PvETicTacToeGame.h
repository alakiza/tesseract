#ifndef PVE_TIC_TAC_TOE_GAME_H
#define PVE_TIC_TAC_TOE_GAME_H
#include "BasicGame.h"
#include "Button.h"

    bool gameEnd;
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
    };
    
    void Player::CheckControls()
    {
        if(fJoyStick->AxisX() > 478) ++fCurrentPosition.X;
        if(fJoyStick->AxisX() < -478) --fCurrentPosition.X;
        
        if(fJoyStick->AxisY() > 478) ++fCurrentPosition.Y;
        if(fJoyStick->AxisY() < -478) --fCurrentPosition.Y;
        
        if(fCurrentPosition.X >= cube->LenX()) fCurrentPosition.X = 0;
        if(fCurrentPosition.Y >= cube->LenY()) fCurrentPosition.Y = 0;
        
    }
    
    class PvE_TicTacToe_Game : public IBasicGame
    {
    private:
        Player** fPlayer;
    public:
        PvE_TicTacToe_Game();
        
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
        delete[] fPLayer;
    }
    
    void PvE_TicTacToe_Game::Run(int FirstPlayer, uint32_t FirstColor, uint32_t SecondColor)
    {
        fFirstColor = FirstColor;
        fSecondColor = SecondColor;
        
        stateMatrix = GenerateMatrix(3, 3, 3);
		long*** VisibleMatrix = GenerateMatrix(3, 3, 3);
        
        cube->SetPixelColor(stateMatrix, 3, 3, 3);
        cube->Show();
        
        while(!gameEnd)
        {
            VisibleMatrix[
            cube->SetPixelColor(stateMatrix, 3, 3, 3);
            cube->Show();
        }
    }
#endif
