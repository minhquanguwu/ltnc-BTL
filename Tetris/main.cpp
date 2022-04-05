#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_init.h"
#include <time.h>
#include <cstdlib>

using namespace std;

const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "minhwanguwu";
const int M = 20;
const int N = 10;
const int STEP = 30;
struct point{
  int x, y;
  int status = 0;
  int type;
} a[4],b[4];
int field[M][N] = {0};
int piece[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};
void DrawMap(SDL_Renderer* renderer)
{
    for(int i = 0; i <= 300; i+=30)
    {
        SDL_SetRenderDrawColor(renderer,255,255,255,0);
        SDL_RenderDrawLine(renderer,i,0,i,STEP*(M+1));
    }
    for(int i = 0; i <=630; i+=30)
    {
        SDL_SetRenderDrawColor(renderer,255,255,255,0);
        SDL_RenderDrawLine(renderer,0,i,STEP*N,i);
    }
    SDL_RenderPresent(renderer);
}
void moved(SDL_Renderer* renderer, point a[])
{
    SDL_Event e;
    while (true) {
        if(SDL_PollEvent(&e) == 0) SDL_Delay(100);
        else if(e.type == SDL_QUIT) break;
        else if(e.type == SDL_KEYDOWN){
            switch (e.key.keysym.sym){
                case SDLK_RIGHT:
                    cout << SDL_GetKeyName(e.key.keysym.sym) << endl;
                    for(int i = 0; i < 4; i++)
                        a[i].x+=STEP;break;
                case SDLK_LEFT:
                    cout << SDL_GetKeyName(e.key.keysym.sym) << endl;
                    for(int i = 0; i < 4; i++)
                        a[i].x-=STEP;break;
                case SDLK_UP:
                    switch(a[0].type){
                        case 0:{
                            switch(a[0].status){
                                case 0:
                                    a[2].x+=STEP;a[2].y-=STEP;
                                    a[0].x-=STEP;a[0].y+=STEP;
                                    a[3].x+=2*STEP;a[3].y-=2*STEP; break;
                                case 1:
                                    a[2].x-=STEP;a[2].y+=STEP;
                                    a[0].x+=STEP;a[0].y-=STEP;
                                    a[3].x-=2*STEP;a[3].y+=2*STEP; break;
                            }
                            a[0].status++;
                            if(a[0].status == 2) a[0].status=0; break;
                        }
                        case 1:{
                            switch(a[0].status){
                                case 0:
                                    a[2].x+=STEP;a[2].y-=STEP;
                                    a[0].x+=STEP;a[0].y+=STEP;
                                    a[3].y-=2*STEP; break;
                                case 1:
                                    a[2].x-=STEP;a[2].y+=STEP;
                                    a[0].x-=STEP;a[0].y-=STEP;
                                    a[3].y+=2*STEP; break;
                            }
                            a[0].status++;
                            if(a[0].status == 2) a[0].status=0; break;
                        }
                        case 2:{
                            switch(a[0].status){
                                case 0:
                                    a[2].x+=STEP;a[2].y-=STEP;
                                    a[0].x+=STEP;a[0].y+=STEP;
                                    a[3].y-=2*STEP; break;
                                case 1:
                                    a[2].x-=STEP;a[2].y+=STEP;
                                    a[0].x-=STEP;a[0].y-=STEP;
                                    a[3].y+=2*STEP; break;
                            }
                            a[0].status++;
                            if(a[0].status == 2) a[0].status=0; break;
                        }
                        case 3:{
                            switch(a[0].status){
                                case 0:
                                    a[2].x+=STEP;a[2].y-=STEP;
                                    a[0].x+=STEP;a[0].y+=STEP;
                                    a[3].x-=STEP;a[3].y-=STEP; break;
                                case 1:
                                    a[2].x+=STEP;a[2].y+=STEP;
                                    a[0].x-=STEP;a[0].y+=STEP;
                                    a[3].x+=STEP;a[3].y-=STEP; break;
                                case 2:
                                    a[2].x-=STEP;a[2].y+=STEP;
                                    a[0].x-=STEP;a[0].y-=STEP;
                                    a[3].x+=STEP;a[3].y+=STEP; break;
                                case 3:
                                    a[2].x-=STEP;a[2].y-=STEP;
                                    a[0].x+=STEP;a[0].y-=STEP;
                                    a[3].x-=STEP;a[3].y+=STEP; break;
                            }
                            a[0].status++;
                            if(a[0].status == 4) a[0].status=0; break;
                        }
                        case 4:{
                            switch(a[0].status){
                                case 0:
                                    a[0].x+=2*STEP;
                                    a[1].x+=STEP;a[1].y+=STEP;
                                    a[3].x-=STEP;a[3].y-=STEP; break;
                                case 1:
                                    a[0].x-=STEP;a[0].y+=2*STEP;
                                    a[1].x-=2*STEP;a[1].y+=STEP;
                                    a[2].x-=STEP;
                                    a[3].y-=STEP; break;
                                case 2:
                                    a[0].x-=STEP;a[0].y-=STEP;
                                    a[1].y-=2*STEP;
                                    a[2].x+=STEP;a[2].y-=STEP;
                                    a[3].x+=2*STEP; break;
                                case 3:
                                    a[0].y-=STEP;
                                    a[1].x+=STEP;
                                    a[2].y+=STEP;
                                    a[3].x-=STEP;a[3].y+=2*STEP; break;
                            }
                            a[0].status++;
                            if(a[0].status == 4) a[0].status=0; break;
                        }
                        case 5:{
                            switch(a[0].status){
                                case 0:
                                    a[0].x-=STEP;
                                    a[1].y-=STEP;
                                    a[2].x+=STEP;a[2].y-=2*STEP;
                                    a[3].x+=2*STEP;a[3].y-=STEP; break;
                                case 1:
                                    a[0].y+=2*STEP;
                                    a[1].x-=STEP;a[1].y+=STEP;
                                    a[2].x-=2*STEP;
                                    a[3].x-=STEP;a[3].y-=STEP; break;
                                case 2:
                                    a[0].x+=2*STEP;a[0].y-=STEP;
                                    a[1].x+=STEP;
                                    a[2].y+=STEP;
                                    a[3].x-=STEP; break;
                                case 3:
                                    a[0].x-=STEP;a[0].y-=STEP;
                                    a[2].x+=STEP;a[2].y+=STEP;
                                    a[3].y+=2*STEP; break;
                            }
                            a[0].status++;
                            if(a[0].status == 4) a[0].status=0; break;
                        }
                } break;
                case SDLK_ESCAPE:
                return;
            }
            SDL_SetRenderDrawColor(renderer,0,0,0,0);
            SDL_RenderClear(renderer);
            DrawMap(renderer);
            for(int i = 0; i < 4; i++){
                draw(renderer,a[i].x,a[i].y);
            }
        }
    }
}

bool GameOver()
{
    for(int i=0; i<4; i++){
        if(a[i].y >630) return true;
    }
    return false;
}
int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SDL_SetRenderDrawColor(renderer,255,255,255,0);
    SDL_RenderClear(renderer);
    srand(time(0));
    int n = rand() % 7;
    for(int i = 0; i < 4; i++){
        a[i].x = piece[n][i] % 2;
        a[i].y = piece[n][i] / 2;
        a[i].x*=STEP;
        a[i].y*=STEP;
        a[i].type = n;
        cout << a[i].x << " " << a[i].y << endl;
        draw(renderer,a[i].x,a[i].y);
    }
    while(!GameOver())
    {
        moved(renderer,a);
        //SDL_Delay(1000);
    }
    //moved(renderer,a);
    quitSDL(window, renderer);
    return 0;
}
