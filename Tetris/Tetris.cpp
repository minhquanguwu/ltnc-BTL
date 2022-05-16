#include "Tetris.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
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
                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                int initted = IMG_Init(imgFlags);
                if((initted & imgFlags) != imgFlags)
                    std::cout << "Error" << IMG_GetError() << '\n';
                SDL_Surface* loadSurf = IMG_Load("test10.png");
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
                loadSurf = IMG_Load("doublekill.png");
                bgdouble = SDL_CreateTextureFromSurface(renderer,loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("triplekill.png");
                bgtriple = SDL_CreateTextureFromSurface(renderer,loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("monsterkill.png");
                bgmonster = SDL_CreateTextureFromSurface(renderer,loadSurf);
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
    //Init Audio
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    Music = Mix_LoadMUS("Tocca.wav");
    Scratch = Mix_LoadWAV("scratch.wav");
    Double = Mix_LoadWAV("doublekill.wav");
    Triple = Mix_LoadWAV("triplekill.wav");
    Monster = Mix_LoadWAV("monsterkill.wav");
    if( Music == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }
    //Initialize SDL_ttf
    if (TTF_Init() < 0)
    {
        SDL_Log("%s", TTF_GetError());
        return -1;
    }
    Font = TTF_OpenFont("Font2.ttf",35);
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
            case SDLK_p:
                pause = true;
                break;
            case SDLK_x:
                buff = true;
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
            done++;
            score++;
        }
        if(cnt < Column) k--;
    }
    if(done >= 2) cntbuff++;
    if(buff == true && cntbuff > 0)
    {
        score++;
        done = 1;
        cntbuff--;
        for(int i = Line-1; i >=1; i--)
        for(int j = 0; j < Column; j++)
            field[i][j] = field[i-1][j];
    }
    if(done == 2) {
            Mix_PlayChannel(-1,Double,0);
            lastTime1 = currentTime;
            doublekill = true;
    }
    else if(done == 3) {
            Mix_PlayChannel(-1,Triple,0);
            lastTime1 = currentTime;
            triplekill = true;
    }
    else if(done == 4) {
            Mix_PlayChannel(-1,Monster,0);
            lastTime1 = currentTime;
            monsterkill = true;
    }
    else if(done > 0) Mix_PlayChannel(-1,Scratch,0);
    //pause game
    while(pause == true)
    {
        SDL_Event e;
        if(SDL_PollEvent(&e) == 0) SDL_Delay(100);
        else if(e.type == SDL_QUIT) {pause = false; break;}
        else if(e.type == SDL_KEYDOWN)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_p: pause = false; break;
            }
        }
    }
    //gameOver
    for(int i = 0; i < Column; i++)
    {
        if(field[0][i] != 0)
            running = false;
    }
    level = (score/20) + 1;
    dx = 0;
    rot = false;
    buff = false;
    if(500 - (level-1)*40 >= 200) delay = 500 - (level-1)*40;
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
            //moveRectPos(dst_rec,36,204);
            moveRectPos(dst_rec,36,206);

            SDL_RenderCopy(renderer,block,&src_rec,&dst_rec);
        }
    }
    for(int i = 0; i < 4; i++)
    {
        setRectPos(src_rec,color*BlockH);
        // render current Tetrimino
        setRectPos(dst_rec,a[i].x*BlockW,a[i].y*BlockH);
        //moveRectPos(dst_rec,36,204);
        moveRectPos(dst_rec,36,206);
        SDL_RenderCopy(renderer,block,&src_rec,&dst_rec);
        // render next Tetrimino
        setRectPos(dst_rec,c[i].x*BlockW,c[i].y*BlockH);
        moveRectPos(dst_rec,437,800);
        SDL_RenderCopy(renderer,block,&src_rec,&dst_rec);
    }
    //render double kill
    if(doublekill == true)
    {
        setRectPos(dst_rec,250,50,300,300);
        SDL_RenderCopy(renderer,bgdouble,NULL,&dst_rec);
        if(currentTime - lastTime1 > 1000)
        {
            doublekill = false;
        }
    }
    //render triple kill
    if(triplekill == true)
    {
        setRectPos(dst_rec,250,50,300,300);
        SDL_RenderCopy(renderer,bgtriple,NULL,&dst_rec);
        if(currentTime - lastTime1 > 1000)
        {
            triplekill = false;
        }
    }
    //render monsterkill
    if(monsterkill == true)
    {
        setRectPos(dst_rec,250,50,300,300);
        SDL_RenderCopy(renderer,bgmonster,NULL,&dst_rec);
        if(currentTime - lastTime1 > 1000)
        {
            monsterkill = false;
        }
    }
    // render buff
    setRectPos(src_rec,0,0);
    setRectPos(dst_rec,510,615);
    string Res = to_string(cntbuff);
    loadFont1(Res, src_rec, dst_rec);
    // render current Score
    setRectPos(src_rec,0,0);
    setRectPos(dst_rec,500,295);
    Res = to_string(score);
    loadFont1(Res, src_rec, dst_rec);
    // render current Level
    setRectPos(src_rec,0,0);
    setRectPos(dst_rec,500,400);
    Res = to_string(level);
    loadFont1(Res, src_rec, dst_rec);
    // render current Line
    setRectPos(src_rec,0,0);
    setRectPos(dst_rec,500,520);
    Res = to_string(score);
    loadFont1(Res, src_rec, dst_rec);
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
    setRectPos(src_rec,0,0);
    setRectPos(dst_rec,150,50);
    loadFont("START THE GAME <3 ", src_rec, dst_rec);
    setRectPos(dst_rec,130,150);
    loadFont("Press SPACE to continue ", src_rec, dst_rec);
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
    setRectPos(src_rec,0,0);
    setRectPos(dst_rec,150,50);
    loadFont("WANNA PLAY AGAIN ?", src_rec, dst_rec);
    setRectPos(dst_rec,175,100);
    loadFont("YES(1)       NO(2)", src_rec, dst_rec);
    setRectPos(dst_rec,200,850);
    string Res = "YOUR SCORE: " + to_string(score);
    loadFont(Res, src_rec, dst_rec);
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
void Tetris::loadFont(string text, SDL_Rect source, SDL_Rect destination)
{
	SDL_Color fg = { 0, 191, 255 };

	SDL_Surface *surface = TTF_RenderText_Blended(Font, text.c_str(), fg);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	TTF_SizeText(Font, text.c_str(), &source.w, &destination.h);

	destination.w = source.w;
	destination.h = source.h;
	SDL_RenderCopy(renderer, texture, &source, &destination);

}
void Tetris::loadFont1(string text, SDL_Rect source, SDL_Rect destination)
{
	SDL_Color fg = { 255, 0, 0 };

	SDL_Surface *surface = TTF_RenderText_Blended(Font, text.c_str(), fg);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	TTF_SizeText(Font, text.c_str(), &source.w, &destination.h);

	destination.w = source.w;
	destination.h = source.h;
	SDL_RenderCopy(renderer, texture, &source, &destination);

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
    SDL_DestroyTexture(bgdouble);
    SDL_DestroyTexture(bgtriple);
    SDL_DestroyTexture(bgmonster);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(Music);
    Mix_FreeChunk(Scratch);
    Mix_FreeChunk(Triple);
    Mix_FreeChunk(Monster);
}
