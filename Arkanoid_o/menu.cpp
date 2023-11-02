#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "menu_header.h"

#define FONT_SIZE 32
#define WIDTH 620
#define HEIGHT 720
#define VERTICAL_SPACING 50
#define NUM_MENU_ITEMS 4

MenuItem selectedMenuItem = START;
bool showMenu = true;

void renderMenuItem(SDL_Renderer* renderer, const char* text, bool isSelected, int yPos);
void renderMenu(SDL_Renderer* renderer);


void handleMenuInput(SDL_Event& e, bool& running, bool& showMenu, SDL_Renderer* renderer) {
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            if (showMenu) {
                selectedMenuItem = static_cast<MenuItem>((selectedMenuItem - 1 + NUM_MENU_ITEMS) % NUM_MENU_ITEMS);
            }
            break;
        case SDLK_DOWN:
            if (showMenu) {
                selectedMenuItem = static_cast<MenuItem>((selectedMenuItem + 1) % NUM_MENU_ITEMS);
            }
            break;
            case SDLK_RETURN:
            if (showMenu) {
                switch (selectedMenuItem) {
                case START:
                    showMenu = false;
                    break;
                case LEADERBOARDS:
                    system("start https://pookyskeekys.github.io/");
                    break;
                case CREDITS:
                    renderCredits(renderer);
                    SDL_Delay(15000);
                    break;
                case EXIT:
                    running = false;
                    break;
                }
            }
            break;
        }
    }
}

void renderMenu(SDL_Renderer* renderer) {
    if (showMenu) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
        SDL_RenderClear(renderer);

        const char* startText = "Start";
        const char* leaderboardsText = "Leaderboards";
        const char* creditsText = "Credits";
        const char* exitText = "Exit";

        int yPos = HEIGHT / 2;

        renderMenuItem(renderer, startText, (selectedMenuItem == START), yPos);
        yPos += VERTICAL_SPACING;

        renderMenuItem(renderer, leaderboardsText, (selectedMenuItem == LEADERBOARDS), yPos);
        yPos += VERTICAL_SPACING;

        renderMenuItem(renderer, creditsText, (selectedMenuItem == CREDITS), yPos);
        yPos += VERTICAL_SPACING;

        renderMenuItem(renderer, exitText, (selectedMenuItem == EXIT), yPos);

        SDL_Color textColor = { 255, 255, 255, 255 };
        TTF_Font* gameFont = TTF_OpenFont("font.ttf", FONT_SIZE * 2);

        if (gameFont == nullptr) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        }
        else {
            std::string gameText = "ARKANOID";
            SDL_Surface* gameSurface = TTF_RenderText_Solid(gameFont, gameText.c_str(), textColor);
            SDL_Texture* gameTexture = SDL_CreateTextureFromSurface(renderer, gameSurface);

            int gameWidth = gameSurface->w;
            int gameHeight = gameSurface->h;

            SDL_Rect gameRect = { (WIDTH - gameWidth) / 2, HEIGHT / 4, gameWidth, gameHeight };

            SDL_RenderCopy(renderer, gameTexture, nullptr, &gameRect);

            SDL_FreeSurface(gameSurface);
            SDL_DestroyTexture(gameTexture);
            TTF_CloseFont(gameFont);
        }

        SDL_RenderPresent(renderer);
    }
}


void renderMenuItem(SDL_Renderer* renderer, const char* text, bool isSelected, int yPos) {
    SDL_Color textColor = { 255, 255, 255, 255 };
    TTF_Font* menuItemFont = TTF_OpenFont("font.ttf", FONT_SIZE);
    TTF_Font* symbolFont = TTF_OpenFont("pixel.ttf", FONT_SIZE);

    if (menuItemFont == nullptr || symbolFont == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(menuItemFont, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    SDL_Rect textRect = { (WIDTH - textWidth) / 2, yPos, textWidth, textHeight };

    if (isSelected) {
        const char* selectionSymbol = "> ";
        SDL_Surface* symbolSurface = TTF_RenderText_Solid(symbolFont, selectionSymbol, textColor);
        SDL_Texture* symbolTexture = SDL_CreateTextureFromSurface(renderer, symbolSurface);

        int symbolWidth = symbolSurface->w;
        int symbolHeight = symbolSurface->h;

        SDL_Rect symbolRect = { textRect.x - symbolWidth, yPos, symbolWidth, symbolHeight };

        textRect.x += 10;

        SDL_RenderCopy(renderer, symbolTexture, nullptr, &symbolRect);

        SDL_FreeSurface(symbolSurface);
        SDL_DestroyTexture(symbolTexture);
    }

    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(menuItemFont);
    TTF_CloseFont(symbolFont);
}


