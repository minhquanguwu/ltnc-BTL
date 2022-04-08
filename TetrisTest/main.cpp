#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Tetris.h"
using namespace std;

int main(int argc, char* argv[])
{
    Tetris* tetris = new Tetris();
    const char* title = "Minhwang";
    srand(time(0));
    while(tetris->restart())
    {
        if(tetris->init(title))
        {
            tetris->gameStart();
            while(tetris->isrunning())
            {
                tetris->setcurrentTime(SDL_GetTicks());
                tetris->check();
                tetris->handleEvent();
                tetris->gameplay();
                tetris->updateRender();
            }
            tetris->gameOver();
        }
        else cout << "Error";
        tetris->clean();
    }
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
    return 0;
}
