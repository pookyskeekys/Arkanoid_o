#ifndef MENU_HEADER_H
#define MENU_HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum MenuItem { START, LEADERBOARDS, CREDITS, EXIT };

extern MenuItem selectedMenuItem;
extern bool showMenu;

void handleMenuInput(SDL_Event& e, bool& running, bool& showMenu, SDL_Renderer* renderer);
void renderMenu(SDL_Renderer* renderer);
void renderCredits(SDL_Renderer* renderer);

#endif
