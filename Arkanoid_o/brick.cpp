#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include "menu_header.h"
#include <cstdlib>
#include <ctime>
#include <cmath>


#define WIDTH 620
#define HEIGHT 720
#define SPEED 9
#define FONT_SIZE 32
#define BALL_SPEED 8
#define SIZE 16
#define COL 7
#define ROW 5
#define PI 3.14
#define SPACING 16

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
bool running;
int frameCount, timerFPS, lastFrame, fps;
SDL_Color color;
SDL_Rect paddle, ball, lives, brick;
float velY, velX;
float gameSpeed = 1.0;
int liveCount;
int score = 0;
bool bricks[ROW * COL];

extern MenuItem selectedMenuItem;
extern bool showMenu;

bool gamePaused = false;
Uint32 gamePauseStartTime = 0;

bool areAllBricksBroken() {
	for (int i = 0; i < COL * ROW; i++) {
		if (bricks[i]) {
			return false;
		}
	}
	return true;
}

void handleMenuInput(SDL_Event& e, bool& running, bool& showMenu);
void renderMenu(SDL_Renderer* renderer);

void generateRandomBricks() {
	srand(time(NULL)); // seed 
	for (int i = 0; i < COL * ROW; i++) {
		bricks[i] = true;
	}

	for (int i = 0; i < COL * ROW; i++) {
		bricks[i] = rand() % 2;
	}
}

void setBricks(int i) {
	brick.x = (i % COL) * (brick.w + SPACING);
	brick.y = (i / COL) * (brick.h + SPACING) + brick.h * 3;
}

void resetBricks() {
	generateRandomBricks();

	if (liveCount <= 0) {
		liveCount = 3;
		score = 0;
	}

	for (int i = 0; i < COL * ROW; i++) {
		if (bricks[i]) {
			setBricks(i);
			brick.w = (WIDTH - (SPACING * COL)) / COL;
			brick.h = 22;
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			if (i % 2 == 0) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(renderer, &brick);
		}
	}

	paddle.x = (WIDTH / 2) - (paddle.w / 2);
	ball.y = paddle.y - (paddle.h * 4);
	velY = BALL_SPEED / 2;
	velX = 0;
	ball.x = WIDTH / 2 - (SIZE / 2);
}


void write(std::string text, int x, int y, int align) {
	SDL_Surface* surface;
	SDL_Texture* texture;
	const char* t = text.c_str();
	surface = TTF_RenderText_Solid(font, t, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect textRect;

	if (align == 1) {
		textRect.x = x;
	}
	else if (align == 2) {
		textRect.x = x - surface->w / 2;
	}
	else {
		textRect.x = x - surface->w;
	}

	textRect.y = y;
	textRect.w = surface->w;
	textRect.h = surface->h;

	SDL_RenderCopy(renderer, texture, NULL, &textRect);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}





void handleBlockBreak() {
	gameSpeed += 0.05; // break block = 5% speed

	// speed limit 200%
	if (gameSpeed > 2.0) {
		gameSpeed = 2.0;
	}
	score += 50;
}

void resetBricksAndGameSpeed() {
	gameSpeed = 1.0; // reset game+speed
	resetBricks();
}

void update() {
	if (liveCount <= 0) {
	}
	else {
		if (SDL_HasIntersection(&ball, &paddle)) {
			double rel = (paddle.x + (paddle.w / 2)) - (ball.x + (SIZE / 2));
			double norm = rel / (paddle.w / 2);
			double bounce = norm * (5 * PI / 12);
			velY = -BALL_SPEED * cos(bounce);
			velX = BALL_SPEED * -sin(bounce);
		}
		if (ball.y <= 0) velY = -velY;
		if (ball.y + SIZE >= HEIGHT) {
			velY = -velY;
			liveCount--;
		}

		if (ball.x <= 0 || ball.x + SIZE >= WIDTH) velX = -velX;

		ball.x += velX * gameSpeed;
		ball.y += velY * gameSpeed;

		if (paddle.x < 0) paddle.x = 0;
		if (paddle.x + paddle.w > WIDTH) paddle.x = WIDTH - paddle.w;

		// all brick broken check
		bool allBricksBroken = true;
		for (int i = 0; i < COL * ROW; i++) {
			setBricks(i);
			if (SDL_HasIntersection(&ball, &brick) && bricks[i]) {
				bricks[i] = 0;
				handleBlockBreak();
				if (ball.x >= brick.x) { velX = -velX; ball.x -= 20; }
				if (ball.x <= brick.x) { velX = -velX; ball.x += 20; }
				if (ball.y <= brick.y) { velY = -velY; ball.y -= 20; }
				if (ball.y >= brick.y) { velY = -velY; ball.y += 20; }
			}
			if (bricks[i]) {
				allBricksBroken = false;
			}
		}
		if (areAllBricksBroken()) {
			resetBricksAndGameSpeed();
		}

		// live count 0 checker
		if (liveCount <= 0 || allBricksBroken) {
		}
	}
}

void input() {
	SDL_Event e;
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&e)) if (e.type == SDL_QUIT) running = false;
	if (keystates[SDL_SCANCODE_ESCAPE]) running = false;
	if (keystates[SDL_SCANCODE_LEFT]) paddle.x -= SPEED * gameSpeed;
	if (keystates[SDL_SCANCODE_RIGHT]) paddle.x += SPEED * gameSpeed;

}
void render() {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
	SDL_RenderClear(renderer);

	frameCount++;
	timerFPS = SDL_GetTicks() - lastFrame;
	if (timerFPS < (1000 / 60)) {
		SDL_Delay((1000 / 60) - timerFPS);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &paddle);
	SDL_RenderFillRect(renderer, &ball);
	write("s: " + std::to_string(score), FONT_SIZE / 2, FONT_SIZE * 0.5, 1);
	write("hp:" + std::to_string(liveCount), WIDTH - FONT_SIZE / 2, FONT_SIZE * 0.5, 3);



	for (int i = 0; i < COL * ROW; i++) {
		setBricks(i);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		if (i % 2 == 0) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		if (bricks[i]) {
			SDL_RenderFillRect(renderer, &brick);
		}
	}

	SDL_RenderPresent(renderer);
}

void handleGameOver() {
	if (liveCount <= 0 && !gamePaused) {
		gamePaused = true;
		gamePauseStartTime = SDL_GetTicks();
	}

	if (gamePaused && SDL_GetTicks() - gamePauseStartTime >= 5000) { //5 sec. delay
		resetBricksAndGameSpeed();
		gamePaused = false;
	}

	// game over msg
	if (gamePaused) {
		write("Game Over", WIDTH / 2, HEIGHT / 2 - FONT_SIZE, 2);
		write("Score: " + std::to_string(score), WIDTH / 2, HEIGHT / 2, 2);
	}
}

int main() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) std::cout << "Failed at SDL_Init()" << std::endl;
	if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
	SDL_SetWindowTitle(window, "Arkanoid");
	TTF_Init();
	font = TTF_OpenFont("font.ttf", FONT_SIZE);
	running = 1;
	static int lastTime = 0;
	color.r = color.g = color.b = 255;
	paddle.h = 12; paddle.w = WIDTH / 4;
	paddle.y = HEIGHT - paddle.h - 32;
	ball.w = ball.h = SIZE;
	brick.w = (WIDTH - (SPACING * COL)) / COL;
	brick.h = 22;

	resetBricks();

	while (running) {
		lastFrame = SDL_GetTicks();

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}

			handleMenuInput(e, running, showMenu, renderer);
		}

		SDL_RenderClear(renderer);

		if (!showMenu) {
			if (!gamePaused) {
				update();
				input();
			}
			render();
			handleGameOver();
		}
		else {
			renderMenu(renderer);
		}

		SDL_RenderPresent(renderer);
	}


	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();





	return 0;
}