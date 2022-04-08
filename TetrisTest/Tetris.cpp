#include "Tetris.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;
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
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenW, ScreenH, SDL_WINDOW_SHOWN);
        if(window != NULL)
        {
            renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
            if(renderer != NULL)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                int imgFlags = IMG_INIT_PNG;
                int initted = IMG_Init(imgFlags);
                if((initted & imgFlags) != imgFlags)
                    std::cout << "Error" << IMG_GetError() << '\n';
                SDL_Surface* loadSurf = IMG_Load("background.png");
                background = SDL_CreateTextureFromSurface(renderer,loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("blocks.png");
                block = SDL_CreateTextureFromSurface(renderer,loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("gameStart.jpg");
                bggameStart = SDL_CreateTextureFromSurface(renderer,loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("gameOver.png");
                bggameOver = SDL_CreateTextureFromSurface(renderer,loadSurf);
                SDL_FreeSurface(loadSurf);
                firstInit();
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
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    Music = Mix_LoadMUS("Tocca.wav");
    Scratch = Mix_LoadWAV("scratch.wav");
    if( Music == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }
    running = true;
    score = 0;
    return true;
}
void Tetris::nextTetrimino()
{
    color = 1 + rand() % 7;
    int n = rand() % 7;
    for(int i = 0; i < 4; i++)
    {
        a[i] = c[i];
        c[i].x = figure[n][i] % 4;
        c[i].y = figure[n][i] / 4;
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
                delay = 30;
                break;
            case SDLK_ESCAPE:
                running = false;
                break;
            case SDLK_9:
                //If there is no music playing
                if( Mix_PlayingMusic() == 0 )
                {
                    //Play the music
                    Mix_PlayMusic( Music, -1 );
                }
                //If music is being played
                else
                {
                    //If the music is paused
                    if( Mix_PausedMusic() == 1 )
                    {
                        //Resume the music
                        Mix_ResumeMusic();
                    }
                    //If the music is playing
                    else
                    {
                    //Pause the music
                    Mix_PauseMusic();
                    }
                }
                break;
            }
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
bool Tetris::check()
{
    for(int i = 0; i < 4; i++)
    {
        if(a[i].x < 0 || a[i].x >= Column || a[i].y >= Line || a[i].y < 0)
            return false;
        else if(field[a[i].y][a[i].x] != 0)
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
    if(check() == false)
    {
        for(int i = 0; i < 4; i++)
        {
            a[i] = b[i];
        }
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
    if(check() == false)
    {
        for(int i = 0; i < 4; i++)
        {
            a[i] = b[i];
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
    }
    if(check() == false)
    {
        for(int i = 0; i < 4; i++)
        {
            a[i] = b[i];
            field[b[i].y][b[i].x] = color;
        }
         nextTetrimino();
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
        if(cnt == Column)
        {
            done = 1;
            score++;
        }
        if(cnt < Column) k--;
    }
    if(done == 1) Mix_PlayChannel(-1,Scratch,0);
    for(int i = 0; i < Column; i++)
    {
        if(field[0][i] != 0)
            running = false;
    }
    dx = 0;
    rot = false;
    delay = 500;
    done = 0;
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
        // render current Tetrimino
        setRectPos(dst_rec,a[i].x*BlockW,a[i].y*BlockH);
        moveRectPos(dst_rec,36,204);
        SDL_RenderCopy(renderer,block,&src_rec,&dst_rec);
        // render next Tetrimino
        setRectPos(dst_rec,c[i].x*BlockW,c[i].y*BlockH);
        moveRectPos(dst_rec,445,250);
        SDL_RenderCopy(renderer,block,&src_rec,&dst_rec);
    }
    SDL_RenderPresent(renderer);
}
void Tetris::firstInit()
{
    for(int i = 0; i < Line; i++)
    for(int j = 0; j < Column; j++)
        field[i][j]=0;
    int n = rand() % 7;
    for(int i = 0; i < 4; i++)
    {
        c[i].x = figure[n][i] % 4;
        c[i].y = figure[n][i] / 4;
    }
}
void Tetris::gameStart()
{
    SDL_Event e;
    SDL_RenderCopy(renderer,bggameStart,NULL,NULL);
    SDL_RenderPresent(renderer);
    while(true)
    {
        if(SDL_PollEvent(&e) == 0) SDL_Delay(100);
        else if(e.type == SDL_QUIT) break;
        else if(e.type == SDL_KEYDOWN)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    return;
                case SDLK_SPACE:
                    Mix_PlayMusic(Music,-1);
                    return;
            }
            SDL_RenderPresent(renderer);
        }
    }
}
void Tetris::gameOver()
{
    SDL_RenderCopy(renderer,bggameOver,NULL,NULL);
    cout << score;
    SDL_RenderPresent(renderer);
    SDL_Event e;
    while(true)
    {
        if(SDL_PollEvent(&e) == 0) SDL_Delay(100);
        else if(e.type == SDL_QUIT) break;
        else if(e.type == SDL_KEYDOWN)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_1:
                    play = true;
                    return;
                case SDLK_2:
                    play = false;
                    return;
                case SDLK_ESCAPE:
                    play = false;
                    return;
                default:
                    break;
            }
            SDL_RenderPresent(renderer);
        }
    }
}
bool Tetris::restart()
{
    return play;
}
void Tetris::clean()
{
    SDL_DestroyTexture(block);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(bggameStart);
    SDL_DestroyTexture(bggameOver);
    SDL_DestroyRenderer(renderer);
    Mix_FreeMusic(Music);
    Mix_FreeChunk(Scratch);
}
