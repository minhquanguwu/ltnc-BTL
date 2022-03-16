#include <SDL.h>
#include "SDL_init.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "minhwanguwu";

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer,
             const int SCREEN_WIDTH, const int SCREEN_HEIGHT, const string WINDOW_TITLE)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);
    // Mo cua so ve theo kich thuoc da chon
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);
    // Lay but ve
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void draw(SDL_Renderer* renderer, int x, int y)
{
    const int RECT_SIZE = 50;
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = RECT_SIZE;
    rect.h = RECT_SIZE;

    SDL_SetRenderDrawColor(renderer,255,255,255,0);
    SDL_SetRenderDrawColor(renderer,0,0,255,0);
    SDL_RenderFillRect(renderer,&rect);
    SDL_RenderPresent(renderer);
}

void waitUntilKeyPressed(SDL_Renderer* renderer)
{
    int x = SCREEN_WIDTH/2;
    int y = SCREEN_HEIGHT/2;
    const int step = 50;
    SDL_Event e;
    while (true) {
        if(SDL_PollEvent(&e) == 0) SDL_Delay(100);
        else if(e.type == SDL_QUIT) break;
        else if(e.type == SDL_KEYDOWN){
            switch (e.key.keysym.sym){
                case SDLK_UP:
                    cout << SDL_GetKeyName(e.key.keysym.sym) << endl;
                    y-=step; break;
                case SDLK_DOWN:
                    cout << SDL_GetKeyName(e.key.keysym.sym) << endl;
                    y+=step; break;
                case SDLK_RIGHT:
                    cout << SDL_GetKeyName(e.key.keysym.sym) << endl;
                    x+=step; break;
                case SDLK_LEFT:
                    cout << SDL_GetKeyName(e.key.keysym.sym) << endl;
                    x-=step; break;
                case SDLK_ESCAPE:
                    return;
            }
            draw(renderer,x,y);
        }
    }
}
