#ifndef FACTORYGAMES_H
#define FACTORYGAMES_H
#include "IGameable.h"
#include "PvETicTacToeGame.h"

    class FactoryGames
    {
    public:
        static IGameable* Get(int num)
        {
            switch(num)
            {
                case 0:
                    return new PvE_TicTacToe_Game();
                    break;
                case 1:
                    break;
            }
        }
    };
#endif
