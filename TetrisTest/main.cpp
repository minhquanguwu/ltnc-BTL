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
    if(tetris->init(title))
    {
        while(tetris->isrunning())
        {
            tetris->setcurrentTime(SDL_GetTicks());
            tetris->check();
            tetris->handleEvent();
            tetris->gameplay();
            tetris->updateRender();
        }
    }
    else cout << "Error";
    tetris->clean();
    return 0;
}
