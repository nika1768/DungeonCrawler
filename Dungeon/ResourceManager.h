#pragma once
#include "libs.h"

class ResourceManager {

public:
	static void Init(SDL_Renderer*);
	static SDL_Texture* LoadTexture(std::string name);

	static SDL_Rect GetRectangle(int x, int y, int w, int h);

	static SDL_Renderer* ren;
	static SDL_Window* win;

private:

	static bool Initialized;
	static std::map<std::string, SDL_Texture*> textures;


};