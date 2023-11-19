#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
bool gamerunning = true;


const int GRAVITY = 50;

//This is a 2d vector struct that holds player attributes 
typedef struct {
	float x;
	float y;
}Vector2d;

//This player struct holds all our player information
struct Player {
	Vector2d pos;
	Vector2d vel;
	Vector2d acc;
	SDL_Rect rect;
	bool inAir = true;
};

Player player;

//little obstacles we created 
SDL_Rect obstacle = { 500,400,250,20 };
SDL_Rect obstacle2 = { 120,300,180,20 };
SDL_Rect obstacle3 = { 20,100,140,20 };


//vector created so it can hold the platfrom data
vector <SDL_Rect> obstacles;

void Init() {
	//Error handling statement which will show us what went wrong if anything did
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		cout << "SDL has failed" << SDL_GetError() << endl;
	}
	// This is where we input our windows title, x position , y postion , heaight and width
	window = SDL_CreateWindow("The Conquest", 400, 100, 800, 600, 0);
	if (window == NULL) {
		cout << "Window failed to work" << SDL_GetError() << endl;
	}
	//Renders the window with the specifications we inputed for the window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		cout << "renderer failed" << SDL_GetError() << endl;
	}

	//Our players data
	player.pos.x = 350.f;
	player.pos.y = 100.f;
	player.vel.x = 0.f;
	player.vel.y = 300.f;
	player.acc.x = 350.f;
	player.acc.y = 1400.f;



	player.rect = { (int)player.pos.x, (int)player.pos.y,32,32 };

	//We emplace the obstacles onto the vector 
	obstacles.emplace_back(obstacle);
	obstacles.emplace_back(obstacle2);
	obstacles.emplace_back(obstacle3);




}

void HandleEvents(float dt) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			gamerunning = false;
		}
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			gamerunning = false;
		}



	}

	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	//This portion handles keyboard inputs and gives a corresponding 
	player.vel.x = 0.f;
	if (keystates[SDL_SCANCODE_RIGHT]) {
		player.vel.x += 300.f;
	}
	if (keystates[SDL_SCANCODE_LEFT]) {
		player.vel.x -= player.acc.x;
	}
	if (keystates[SDL_SCANCODE_UP] && !player.inAir) {
		player.vel.y = -player.acc.y;
		player.inAir = true;
	}
}




void Update(float dt) {
	player.vel.y += GRAVITY;
	// Update player position based on velocity
	player.pos.x += player.vel.x * dt;
	player.pos.y += player.vel.y * dt;


	// Checks the boottom of the window 
	if (player.pos.y + player.rect.h >= 600) {
		player.pos.y = 600 - player.rect.h;
		player.vel.y = 0.f;
		player.inAir = false;
	}

	//obstacle detection 
	for (int i = 0; i < obstacles.size(); i++) {
		if (player.pos.y + player.rect.h >= obstacles[i].y &&
			player.pos.y <= obstacles[i].y + obstacles[i].h &&
			player.pos.x + player.rect.w >= obstacles[i].x &&
			player.pos.x <= obstacles[i].x + obstacles[i].w && player.vel.y >= 0.f)
		{
			player.pos.y = obstacles[i].y - player.rect.h;
			player.inAir = false;
			player.vel.y = 0.f;
		}

	}



	player.rect.x = (int)player.pos.x;
	player.rect.y = (int)player.pos.y;



}


void Render() {
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	SDL_RenderClear(renderer);



	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//loop through the items in the vector and render them to the game 
	for (int i = 0; i < obstacles.size(); i++) {
		SDL_RenderFillRect(renderer, &obstacles[i]);

	}

	// This was used to draw the player green
	SDL_SetRenderDrawColor(renderer, 0, 225, 0, 225);
	SDL_RenderFillRect(renderer, &player.rect);

	SDL_RenderPresent(renderer);

}

void Clean() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

}