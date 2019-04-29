#pragma once
#include "libs.h"

class Hero;
class GameObject;
class Tilemap;

class ResourceManager {

public:
	static void Init(SDL_Renderer*);
	static SDL_Texture* LoadTexture(std::string name);

	static SDL_Rect GetRectangle(int x, int y, int w, int h);

	static Hero* GetHero();
	static Tilemap* GetTilemap();

	static std::unique_ptr<Tilemap> tilemap; // TODO

	static SDL_Renderer* ren;
	static SDL_Window* win;

private:

	static bool Initialized;
	static std::map<std::string, SDL_Texture*> textures;
	
	static std::unique_ptr<Hero> hero;
	static std::vector<std::unique_ptr<GameObject>> gameobjects;

};