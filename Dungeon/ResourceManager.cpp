#include "ResourceManager.h"
#include "GameObject.h"
#include "Hero.h"
#include "Tilemap.h"

bool ResourceManager::Initialized = false;
SDL_Renderer* ResourceManager::ren = nullptr;
std::unique_ptr<Hero> ResourceManager::hero = nullptr;
std::unique_ptr<Tilemap> ResourceManager::tilemap = nullptr;
std::map<std::string, SDL_Texture*> ResourceManager::textures;
std::vector<std::unique_ptr<GameObject>> ResourceManager::gameobjects;

void ResourceManager::Init(SDL_Renderer* renderer)
{
	ren = renderer;
	Initialized = true;
	srand(time(0));
}

SDL_Texture* ResourceManager::LoadTexture(std::string path)
{
	if (!Initialized) {
		std::cout << "ResourceManager not initialized, renderer is nullptr." << std::endl;
		return nullptr;
	}
	SDL_Texture* tex = IMG_LoadTexture(ren,path.c_str());
	if (tex == nullptr) {
		std::cout << "ResourceManager::LoadTexture, texture couldn't load " << SDL_GetError() << std::endl;
		return nullptr;
	}
	textures[path] = tex;
	return tex;
}

SDL_Rect ResourceManager::GetRectangle(int x, int y, int w, int h)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

Hero* ResourceManager::GetHero()
{
	if (hero != nullptr) {
	}
	else {
		hero = std::make_unique<Hero>();
	}
	return (Hero*)hero->getPtr();
}

Tilemap *ResourceManager::GetTilemap()
{
	if (tilemap != nullptr) {
	}
	else {
		tilemap = std::make_unique<Tilemap>();
	}
	return (Tilemap*)tilemap->getPtr();
}