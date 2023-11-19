#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "source.hpp"
#undef main
using namespace std;


int main() {
	//Our Init functions as defined in source.pp is being called below
	Init();

	float currentTime = 0.f;
	float previousTime = 0.f;
	float dt = 0.f;

	while (gamerunning) {
		currentTime = SDL_GetTicks();
		dt = (currentTime - previousTime) / 1000;
		previousTime = currentTime;

		HandleEvents(dt);
		Update(dt);
		Render();
	}


	Clean();

	return 0;



}