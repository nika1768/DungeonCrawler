#include "ResourceManager.h"

bool ResourceManager::Initialized = false;
SDL_Renderer* ResourceManager::ren = nullptr;
std::map<std::string, SDL_Texture*> ResourceManager::textures;

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
