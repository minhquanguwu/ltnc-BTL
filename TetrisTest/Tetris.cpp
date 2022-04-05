#include "Tetris.h"
#include <cstdlib>
#include <ctime>
/*
    0 1 2 3
    4 5 6 7
*/
const int Tetris::figure[7][4] =
{
    0,1,2,3, // I
    0,4,5,6, // J
    2,6,5,4, // L
    1,2,5,6, // O
    2,1,5,4, // S
    0,1,5,6, // Z
    1,4,5,6  // T
};
bool Tetris::init(const char* title)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenW, ScreenH, SDL_WINDOW_SHOWN);
        if(window != NULL)
        {
            renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
            if(renderer != NULL)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                SDL_Surface* loadSurf = IMG_Load("background.png");
                background = SDL_CreateTextureFromSurface(renderer,loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("blocks.png");
                block = SDL_CreateTextureFromSurface(renderer,loadSurf);
                SDL_FreeSurface(loadSurf);
                nextTetrimino();
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;

    running = true;
    return true;
}
void Tetris::nextTetrimino()
{
    color = 1 + rand() % 7;
    int n = rand() % 7;
    for(int i = 0; i < 4; i++)
    {
        a[i].x = figure[n][i] % 4;
        a[i].y = figure[n][i] / 4;
    }
}
void Tetris::handleEvent()
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
                rot = true;
                break;
            case SDLK_LEFT:
                dx = -1;
                break;
            case SDLK_RIGHT:
                dx = 1;
                break;
            case SDLK_DOWN:
                delay = 50;
                break;
            case SDLK_ESCAPE:
                running = false;
                break;            }
        default:
            break;
        }
    }
}
void Tetris::setRectPos(SDL_Rect& rect, int x, int y, int w, int h)
{
    rect = {x,y,w,h};
}
void Tetris::moveRectPos(SDL_Rect& rect,int x, int y)
{
    rect.x+=x;
    rect.y+=y;
}
bool Tetris:: check()
{
    for(int i = 0; i < 4; i++)
    {
        if(a[i].x < 0 || a[i].x >= Column || a[i].y >= Line)
            return false;
        else if(field[a[i].y][a[i].x])
            return false;
    }
    return true;
}
void Tetris::gameplay()
{
    SDL_RenderCopy(renderer,background,NULL,NULL);

    // backup
    for(int i = 0; i < 4; i++)
    {
        b[i] = a[i];
    }
    // move
    for(int i = 0; i < 4; i++)
    {
        a[i].x +=dx;
    }
    // rotate
    if(rot == true)
    {
        Point temp = a[2]; // center of rotation
        for(int i = 0; i < 4; i++)
        {
            int m = a[i].y - temp.y;
            int n = a[i].x - temp.x;
            a[i].x = temp.x - m;
            a[i].y = temp.y + n;
        }
    }
    // tick
    if(currentTime - lastTime > delay)
    {
        for(int i = 0; i < 4; i++)
        {
            a[i].y++;
        }
        lastTime = currentTime;
        if(check() == false)
        {
            for(int i = 0; i < 4; i++)
            {
                field[b[i].y][b[i].x] = color;
            }
            nextTetrimino();
        }
    }
    if(check() == false)
    {
        for(int i = 0; i < 4; i++)
        {
            a[i] = b[i];
        }
    }
    // check lines
    int k = Line - 1;
    for(int i = k; i >= 0; i--)
    {
        int cnt = 0;
        for(int j = 0; j < Column; j++)
        {
            if(field[i][j] != 0)
                cnt++;
            field[k][j] = field[i][j];
        }
        if(cnt < Column) k--;
    }
    dx = 0;
    rot = false;
    delay = 1000;

}
void Tetris::updateRender()
{
    // render map
    for(int i = 0; i < Line; i++)
    for(int j = 0; j < Column; j++)
    {
        if(field[i][j] != 0)
        {
            setRectPos(src_rec,field[i][j]*BlockH);
            setRectPos(dst_rec,j*BlockH,i*BlockW);
            moveRectPos(dst_rec,36,204);
            SDL_RenderCopy(renderer,block,&src_rec,&dst_rec);
        }
    }
    for(int i = 0; i < 4; i++)
    {
        setRectPos(src_rec,color*BlockH);
        setRectPos(dst_rec,a[i].x*BlockW,a[i].y*BlockH);
        moveRectPos(dst_rec,36,204);
        SDL_RenderCopy(renderer,block,&src_rec,&dst_rec);
    }
    SDL_RenderPresent(renderer);
}
void Tetris::clean()
{
    SDL_DestroyTexture(block);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();
}
