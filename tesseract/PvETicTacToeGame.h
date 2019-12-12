#ifndef PVE_TIC_TAC_TOE_GAME_H
#define PVE_TIC_TAC_TOE_GAME_H
#include "IGameable.h"
#include "Button.h"

    long PlayerColors[2];

    long*** stateMatrix;

    class Player
    {
    public:
        int8_t Num;
        Point3D fCurrentPosition;
        JoyStick* fJoyStick;

        Player(int8_t aNum, JoyStick* aJoyStick)
        {
            Num = aNum;
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

    class ArtificialIntelligence
    {
    private:
      Point3D fBestTurn;
      Player* fPlayer;
      int8_t*** fRiskMatrix;

      void IncRiskMatrix(Point3D at, int8_t val, int8_t dx, int8_t dy, int8_t dz);

    long Calculate(long*** Matrix, uint8_t level, uint8_t MaxLevel, uint8_t& z, int8_t player);
    public:
      ArtificialIntelligence(Player* player);

      void PeopleTurnMaked(Point3D point);
      void MakeTurn(uint8_t z);

      ~ArtificialIntelligence();
    };

    class PvE_TicTacToe_Game : public IGameable
    {
    private:
        Player** fPlayer;
        ArtificialIntelligence* AI;
    int8_t fPlayerNum;
    public:
        PvE_TicTacToe_Game();
        static Point3D* analyzeField(long*** Matrix, Point3D point);
        static bool IsDrawGame(long*** Matrix, int8_t& z);

        void Run(int FirstPlayer, uint32_t FirstColor, uint32_t SecondColor);
        ~PvE_TicTacToe_Game();
    };

    void Player::CheckControls()
    {
        if(fJoyStick->AxisX() > 490) --fCurrentPosition.X;
        if(fJoyStick->AxisX() < -490) ++fCurrentPosition.X;

        if(fJoyStick->AxisY() > 490) ++fCurrentPosition.Y;
        if(fJoyStick->AxisY() < -490) --fCurrentPosition.Y;

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
          elem = PlayerColors[this->Num];
          return true;
        }
        return false;
    }

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

    long ArtificialIntelligence::Calculate(long*** Matrix, uint8_t level, uint8_t MaxLevel, uint8_t& z, int8_t player)
    {
        long resultWeight = 0;
        if(level < MaxLevel)
        {
            int8_t win = 0;

            for(int y = 0; y <= 2; ++y)
                for(int x = 0; x <= 2; ++x)
                {
                    if(Matrix[x][y][z] == 0)
                    {
                        Matrix[x][y][z] = PlayerColors[player];

                        //cube->SetPixelColor(Matrix, 3, 3, 3);
                        //cube->Show();
                        //delay(100);

                        Point3D point;
                        point.X = x;
                        point.Y = y;
                        point.Z = z;

                        Point3D* res = PvE_TicTacToe_Game::analyzeField(Matrix, point);
                        if(res != nullptr)
                        {
                            if(stateMatrix[res[0].X][res[0].Y][res[0].Z] == PlayerColors[1])
                            {
                                win = 1;
                                if(level == 0)
                                {
                                  Matrix[x][y][z] = 0;
                                  return 65535;
                                }
                            }
                            else if(stateMatrix[res[0].X][res[0].Y][res[0].Z] == PlayerColors[0])
                            {
                                win = -1;
                                if(level == 1)
                                {
                                  Matrix[x][y][z] = 0;
                                  fBestTurn.X = x;
                                  fBestTurn.Y = y;
                                  return -500;
                                }
                            }
                            delete[] res;
                        }

                        resultWeight += win;

                        if(win == 0)
                            resultWeight += Calculate(Matrix, level+1, MaxLevel, z, 1-player);

                        Matrix[x][y][z] = 0;

                        if(fBestTurn.X != -1) return -1;
                    }
                }

        }
        return resultWeight;
    }

    void ArtificialIntelligence::MakeTurn(uint8_t z)
    {
        fBestTurn.X = -1;
        fBestTurn.Y = -1;
        fBestTurn.Z = -1;

        long*** CopyStateMatrix = GenerateMatrix(3, 3, 3);
        CopyMatrix(CopyStateMatrix, stateMatrix, 3, 3, 3);

        long*** WeightMatrix    = GenerateMatrix(3, 3, 3);

        for(int y = 0; y <= 2; ++y)
            for(int x = 0; x <= 2; ++x)
            {
                if(CopyStateMatrix[x][y][z] == 0)
                {
                    WeightMatrix[x][y][z] = Calculate(CopyStateMatrix, 0, 5, z, 1);
                }
                else
                {
                    WeightMatrix[x][y][z] = -32768;
                }
            }

        long MaxWeight = -32768;
        for(int y = 0; y <= 2; ++y)
            for(int x = 0; x <= 2; ++x)
            {
                Serial.println((String)WeightMatrix[x][y][z]);
                if(WeightMatrix[x][y][z] > MaxWeight)
                {
                    MaxWeight = WeightMatrix[x][y][z];
                    fPlayer->fCurrentPosition.X = x;
                    fPlayer->fCurrentPosition.Y = y;
                    fPlayer->fCurrentPosition.Z = z;

                    //cube->SetPixelColor(x, y, z, Color::FromRGB(255, 255, 255));
                    //cube->Show();
                    //delay(100);
                }
            }

        Serial.println(fBestTurn.X);
        Serial.println(fBestTurn.Y);

        if(fBestTurn.X != -1)
        {
          fPlayer->fCurrentPosition.X =  fBestTurn.X;
          fPlayer->fCurrentPosition.Y =  fBestTurn.Y;
        }

        fPlayer->TryMakeTurn();

        FreeMatrix(WeightMatrix, 3, 3, 3);
        FreeMatrix(CopyStateMatrix, 3, 3, 3);
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

    PvE_TicTacToe_Game::PvE_TicTacToe_Game()
    {
        fPlayer = new Player*[2];
        for(int i = 0; i < 2; ++i) fPlayer[i] = new Player(i, nullptr);

        AI = new ArtificialIntelligence(fPlayer[1]);
    }

    PvE_TicTacToe_Game::~PvE_TicTacToe_Game()
    {
        int count = sizeof(fPlayer) / sizeof(Player*);
        for(--count ;count > 0; --count) delete fPlayer[count];
        delete[] fPlayer;

        delete AI;
    }

    Point3D* PvE_TicTacToe_Game::analyzeField(long*** Matrix, Point3D point)
    {
        long& Value = Matrix[point.X][point.Y][point.Z];
        long LastValue = 0;
        uint8_t count = 0;
        Point3D* res = new Point3D[3];
        for(int8_t i = 0; i < 3; ++i)
        {
          res[i].X = 0;
          res[i].Y = 0;
          res[i].Z = 0;

        }
        int8_t X;
        int8_t Y;
        int8_t Z;

        for(int8_t dz = -1; dz <= 1; ++dz)
            for(int8_t dy = -1; dy <= 1; ++dy)
                for(int8_t dx = -1; dx <= 1; ++dx)
                    if(dx != 0 || dy != 0 || dz != 0)
                    {
                        long LastValue = 0;
                        count = 0;

                        for(int8_t i = -2; i < 2; ++i)
                        {
                            X = point.X + i * dx;
                            Y = point.Y + i * dy;
                            Z = point.Z + i * dz;

                            if(X >= 0 && X < 3 &&
                               Y >= 0 && Y < 3 &&
                               Z >= 0 && Z < 3)
                               {
                                   //cube->SetPixelColor(X, Y, Z, LastValue);
                                   //cube->Show();
                                   //delay(1);

                                   //Serial.println(res[count].X);
                                   //Serial.println(res[count].Y);
                                   //Serial.println(res[count].Z);

                                   //Serial.println(count);

                                   if (Matrix[X][Y][Z] == LastValue)
                                   {
                                       if(LastValue != 0)
                                       {
                                           ++count;
                                           //Serial.println("$$$");
                                           //Serial.println(count);
                                           //Serial.println("###");
                                                               res[count-1].X = X;
                                                             res[count-1].Y = Y;
                                                             res[count-1].Z = Z;
                                           if(count == 3)
                                           {
                                               //Serial.println("Out with res");
                                               //Serial.println(count);
                                               //Serial.println(res[count-1].X);
                                               //Serial.println(res[count-1].Y);
                                               //Serial.println(res[count-1].Z);
                                               return res;
                                           }
                                       }
                                   }
                                   else
                                   {
                                       LastValue = Matrix[X][Y][Z];
                                       count = 1;
                                                         res[count-1].X = X;
                                                         res[count-1].Y = Y;
                                                         res[count-1].Z = Z;
                                   }
                               }
                        }
                    }
        //Serial.println("Out");

            delete[] res;
        return nullptr;
    }

    bool PvE_TicTacToe_Game::IsDrawGame(long*** Matrix, int8_t& z)
    {
        for(int y = 0; y <= 2; ++y)
            for(int x = 0; x <= 2; ++x)
                if(Matrix[x][y][z] == 0) return false;

        return true;

    }

    void PvE_TicTacToe_Game::Run(const int FirstPlayer, const uint32_t FirstColor, const uint32_t SecondColor)
    {
        Serial.println(F("Game run"));
        Serial.println(FirstPlayer);
        bool gameEnd = false;
        int8_t layer = 0;
        Point3D* WinCombination;

        fPlayerNum = FirstPlayer;
        if(fPlayerNum < 0) fPlayerNum = 0;
        if(fPlayerNum > 1) fPlayerNum = 1;

        PlayerColors[0] = FirstColor;
        PlayerColors[1] = SecondColor;

        fPlayer[0]->fCurrentPosition.X = 1;
        fPlayer[0]->fCurrentPosition.Y = 1;
        fPlayer[0]->fCurrentPosition.Z = 0;

        fPlayer[1]->fCurrentPosition.X = 1;
        fPlayer[1]->fCurrentPosition.Y = 1;
        fPlayer[1]->fCurrentPosition.Z = 0;

        fPlayer[0]->Num = 0;
        fPlayer[0]->fJoyStick = joySticks[0];

        fPlayer[1]->Num = 1;
        fPlayer[1]->fJoyStick = joySticks[1];

        joySticks[0]->ResetClick();
        joySticks[1]->ResetClick();

        stateMatrix = GenerateMatrix(3, 3, 3);
            long*** VisibleMatrix = GenerateMatrix(3, 3, 3);

        ArtificialIntelligence* AI = new ArtificialIntelligence(fPlayer[1]);
        Serial.println(F("AI Created"));

        cube->SetPixelColor(stateMatrix, 3, 3, 3);
        CopyMatrix(VisibleMatrix, stateMatrix, 3, 3, 3);
        VisibleMatrix[fPlayer[fPlayerNum]->fCurrentPosition.X][fPlayer[fPlayerNum]->fCurrentPosition.Y][fPlayer[fPlayerNum]->fCurrentPosition.Z] = (PlayerColors[fPlayer[fPlayerNum]->Num]+0x00010101) & 0x00ffffff;
        cube->SetPixelColor(VisibleMatrix, 3, 3, 3);
        cube->Show();
        
        Serial.println(F("Game loop start"));
        uint8_t blinkCounter = 0;
        while(!gameEnd)
        {
            long PlayerWin = 0;
            fPlayer[0]->fCurrentPosition.Z = layer;

            if(++blinkCounter > 4) blinkCounter = 0;

            switch(fPlayerNum)
            {
            case 0:
                fPlayer[0]->CheckControls();
                if(fPlayer[0]->fJoyStick->Click())
                {
                    Serial.println(F("People make turn"));
                    if(fPlayer[0]->TryMakeTurn())
                    {
                        fPlayerNum = 1 - fPlayerNum;
                        WinCombination = analyzeField(stateMatrix, fPlayer[0]->fCurrentPosition);
                        if(WinCombination != nullptr)
                        {
                            PlayerWin = stateMatrix[WinCombination[0].X][WinCombination[0].Y][WinCombination[0].Z];
                        }
                    }
                }
                break;
            case 1:
                Serial.println(F("AI thinking"));
                AI->MakeTurn(layer);
                Serial.println(F("AI make turn"));
//                 fPlayer[1]->CheckControls();
//                 if(fPlayer[1]->fJoyStick->Click())
//                     if(fPlayer[1]->TryMakeTurn())
//                     {
//                         fPlayerNum = 1 - fPlayerNum;
//                         PlayerWin = analyzeField(stateMatrix, fPlayer[1]->fCurrentPosition);
//                     }
                fPlayerNum = 1 - fPlayerNum;
                WinCombination = analyzeField(stateMatrix, fPlayer[1]->fCurrentPosition);
                if(WinCombination != nullptr)
                {
                    PlayerWin = stateMatrix[WinCombination[0].X][WinCombination[0].Y][WinCombination[0].Z];
                }
                break;
            }

            if(PlayerWin == FirstColor)
            {
                    //lcd.clear();
                    //PrintIn(lcd, 0, 2, F("First player"));
                    //PrintIn(lcd, 1, 6, F("WIN!"));
                    if(++layer > 2) gameEnd = true;
            }
            else if(PlayerWin == SecondColor)
            {
                    //lcd.clear();
                    //PrintIn(lcd, 0, 2, F("Second player"));
                    //PrintIn(lcd, 1, 6, F("WIN!"));
                    if(++layer > 2) gameEnd = true;
            }
            else
            {
                if(IsDrawGame(stateMatrix, layer))
                    if(++layer > 2) gameEnd = true;
            }

                //Serial.println("Show");
                CopyMatrix(VisibleMatrix, stateMatrix, 3, 3, 3);
                if(blinkCounter == 0)
                  VisibleMatrix[fPlayer[fPlayerNum]->fCurrentPosition.X][fPlayer[fPlayerNum]->fCurrentPosition.Y][fPlayer[fPlayerNum]->fCurrentPosition.Z] = (PlayerColors[fPlayer[fPlayerNum]->Num]+0x000F0F0F) & 0x00ffffff;
                else
                  VisibleMatrix[fPlayer[fPlayerNum]->fCurrentPosition.X][fPlayer[fPlayerNum]->fCurrentPosition.Y][fPlayer[fPlayerNum]->fCurrentPosition.Z] = (PlayerColors[fPlayer[fPlayerNum]->Num]+0x00010101) & 0x00ffffff;
                  
                cube->SetPixelColor(VisibleMatrix, 3, 3, 3);
                cube->Show();

                delay(250);

                if(gameEnd)
                {
                    CopyMatrix(VisibleMatrix, stateMatrix, 3, 3, 3);
                    for(int8_t i = 0; i < 3; ++i)
                    {
                        VisibleMatrix[WinCombination[i].X][WinCombination[i].Y][WinCombination[i].Z] = (PlayerColors[fPlayer[fPlayerNum]->Num]+0x00101010) & 0x00ffffff;
                        cube->SetPixelColor(VisibleMatrix, 3, 3, 3);
                        cube->Show();

                        delay(250);
                    }

                    delay(1000);
                }

        }

        delete AI;
        FreeMatrix(VisibleMatrix, 3, 3, 3);
        FreeMatrix(stateMatrix, 3, 3, 3);
    }
#endif
