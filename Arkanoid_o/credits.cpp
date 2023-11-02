#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "menu_header.h"

#define FONT_SIZE 16
#define WIDTH 620
#define HEIGHT 720

void renderCredits(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 255, 255, 255, 255 };
    TTF_Font* creditsFont = TTF_OpenFont("vcr_osd.ttf", FONT_SIZE);

    if (creditsFont == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    std::string creditsText =
        "Original Developers:\n"
        "Taito - The visionary creators of the classic Arkanoid game that inspired this project.\n\n"
        "Designers:\n"
        "Akira Fujita - For the original design concepts that laid the foundation for Arkanoid.\n"
        "Hiroshi Tsujino - Contributing to the visual appeal and creativity of the game.\n\n"
        "Lead Developer:\n"
        "keekys - efforts in combining, rewriting, and adding new lines of code brought this game to life.\n\n"
        "Open Source Contributions:\n"
        "jarreed0 - For the source code that formed the basis of this project.\n"
        "ochanoaji - Providing valuable assistance and support during the development process.\n\n"
        "Library:\n"
        "SDL2 - A crucial component that empowered the game development process.";

    SDL_Surface* creditsSurface = TTF_RenderText_Blended_Wrapped(creditsFont, creditsText.c_str(), textColor, WIDTH);
    SDL_Texture* creditsTexture = SDL_CreateTextureFromSurface(renderer, creditsSurface);

    int creditsWidth = creditsSurface->w;
    int creditsHeight = creditsSurface->h;

    SDL_Rect creditsRect = { (WIDTH - creditsWidth) / 2, (HEIGHT - creditsHeight) / 2, creditsWidth, creditsHeight };

    SDL_RenderCopy(renderer, creditsTexture, nullptr, &creditsRect);

    SDL_RenderPresent(renderer);

    SDL_FreeSurface(creditsSurface);
    SDL_DestroyTexture(creditsTexture);
    TTF_CloseFont(creditsFont);
}

