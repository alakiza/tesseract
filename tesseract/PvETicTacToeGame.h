#ifndef PVE_TIC_TAC_TOE_GAME_H
#define PVE_TIC_TAC_TOE_GAME_H
#include "IGameable.h"
#include "Button.h"

    long PlayerColors[2];

    long*** stateMatrix;

    Point3D* analyzeField(long*** Matrix, Point3D point)
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
                                   if (Matrix[X][Y][Z] == LastValue)
                                   {
                                       if(LastValue != 0)
                                       {
                                           ++count;
                                           res[count-1].X = X;
                                           res[count-1].Y = Y;
                                           res[count-1].Z = Z;
                                           if(count == 3)
                                           {
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

            delete[] res;
        return nullptr;
    }

    bool IsDrawGame(long*** Matrix, int8_t& z)
    {
        for(int y = 0; y <= 2; ++y)
            for(int x = 0; x <= 2; ++x)
                if(Matrix[x][y][z] == 0) return false;

        return true;
    }

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

        void CheckControls()
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

        bool TryMakeTurn()
        {
            long& elem = stateMatrix[fCurrentPosition.X][fCurrentPosition.Y][fCurrentPosition.Z];
            if(!elem)
            {
              elem = PlayerColors[this->Num];
              return true;
            }
            return false;
        }

        ~Player()
        {

        }

    };

    class ArtificialIntelligence
    {
    private:
        Point3D fBestTurn;
        Player* fPlayer;

        long Calculate(long*** Matrix, uint8_t level, uint8_t MaxLevel, uint8_t& z, int8_t player)
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

                            Point3D point;
                            point.X = x;
                            point.Y = y;
                            point.Z = z;

                            Point3D* res = analyzeField(Matrix, point);
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
    public:

        ArtificialIntelligence(Player* player)
        {
            fPlayer = player;
        }

        void MakeTurn(uint8_t z)
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

        ~ArtificialIntelligence()
        {

        }
    };

    class PvE_TicTacToe_Game : public IGameable
    {
    private:
        Player** fPlayer;
        ArtificialIntelligence* AI;
        int8_t fPlayerNum;
    public:
        PvE_TicTacToe_Game()
        {
            fPlayer = new Player*[2];
            for(int i = 0; i < 2; ++i) fPlayer[i] = new Player(i, nullptr);

            AI = new ArtificialIntelligence(fPlayer[1]);
        }

        ~PvE_TicTacToe_Game()
        {
            int count = sizeof(fPlayer) / sizeof(Player*);
            for(--count ;count > 0; --count) delete fPlayer[count];
            delete[] fPlayer;

            delete AI;
        }

        void Run(int FirstPlayer, uint32_t FirstColor, uint32_t SecondColor);
    };

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
                    lcd.clear();
                    PrintIn(lcd, 0, 2, F("First player"));
                    PrintIn(lcd, 1, 6, F("WIN!"));
                    if(++layer > 2) gameEnd = true;
            }
            else if(PlayerWin == SecondColor)
            {
                    lcd.clear();
                    PrintIn(lcd, 0, 2, F("Second player"));
                    PrintIn(lcd, 1, 6, F("WIN!"));
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
                    Serial.println(F("Game ended"));
                    CopyMatrix(VisibleMatrix, stateMatrix, 3, 3, 3);
                    Serial.println(F("StateMatrix Copy"));
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
        Serial.println(F("AI deleted"));
        FreeMatrix(VisibleMatrix, 3, 3, 3);
        Serial.println(F("Visible Matrix freed"));
        FreeMatrix(stateMatrix, 3, 3, 3);
        Serial.println(F("State Matrix freed"));
    }
#endif
