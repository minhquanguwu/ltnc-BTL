#pragma once
#ifndef TETRIS_H_
#define TETRIS_H_

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>

class Tetris
{
public:
    Tetris()
    {
    }
    ~Tetris()
    {
    }
    void setcurrentTime(Uint32 t)
    {
        currentTime = t;
    }
    bool isrunning()
    {
        return running;
    }
    bool init(const char* title);
    void nextTetrimino();
    bool check();
    bool restart();
    void handleEvent();
    void setRectPos(SDL_Rect& rect,int x = 0, int y = 0, int w = BlockW, int h = BlockH);
    void moveRectPos(SDL_Rect& rect,int x, int y);
    void gameplay();
    void updateRender();
    void firstInit();
    void gameStart();
    void gameOver();
    void clean();

private:
    enum {ScreenW = 600, ScreenH = 960};
    enum {BlockW = 36, BlockH = 36};
    enum {Line = 20, Column = 10};
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* background = NULL, *block = NULL, *bggameOver = NULL, *bggameStart = NULL;
    Mix_Music *Music = NULL;
    Mix_Chunk *Scratch = NULL;
    SDL_Rect src_rec = {0,0,BlockW,BlockH}, dst_rec = {0,0,BlockW,BlockH};

    bool running = false;
    int field[Line][Column] = {0};
    static const int figure[7][4];
    struct Point
    {
        int x,y;
    } a[4], b[4], c[4];
    int done = 0;
    int color = 1;
    int score = 0;
    int dx = 0;
    bool rot = false;
    bool play = true;
    unsigned int delay = 300;
    Uint32 lastTime = 0, currentTime = 0;
};

#endif // TETRIS_H_H
