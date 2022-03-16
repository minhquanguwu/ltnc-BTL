#include <iostream>
#include <SDL.h>
using namespace std;
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT, const string WINDOW_TITLE);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void draw(SDL_Renderer* renderer, int x, int y);
void waitUntilKeyPressed(SDL_Renderer* renderer);

