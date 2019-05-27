#include "ResourceManager.h"
#include "GameObject.h"
#include "Hero.h"
#include "Tilemap.h"
#include "NPC.h"

bool ResourceManager::Initialized = false;

SDL_Renderer* ResourceManager::ren = nullptr;
std::unique_ptr<Hero> ResourceManager::hero = nullptr;
std::unique_ptr<Tilemap> ResourceManager::tilemap = nullptr;
TTF_Font* ResourceManager::font = nullptr;

std::map<std::string, SDL_Texture*> ResourceManager::textures;
std::vector<std::unique_ptr<GameObject>> ResourceManager::gameobjects;

void ResourceManager::Init(SDL_Renderer* renderer) {

	ren = renderer;
	font = TTF_OpenFont(font_path.c_str(), font_size);
	Initialized = true;
	srand(time(0));
}

SDL_Texture* ResourceManager::LoadTexture(std::string path) {

	if (!Initialized) {
		std::cout << "ResourceManager not initialized, renderer is nullptr." << std::endl;
		return nullptr;
	}
	if (textures.find(path) != textures.end()) {
		return textures[path];
	}
	SDL_Texture* tex = IMG_LoadTexture(ren,path.c_str());
	if (tex == nullptr) {
		std::cout << "ResourceManager::LoadTexture, texture couldn't load " << SDL_GetError() << std::endl;
		return nullptr;
	}
	textures[path] = tex;
	return tex;
}

SDL_Texture* ResourceManager::LoadTextTexture(std::string caption, Color color) {

	SDL_Color col;
	col.r = 0;
	col.g = 0;
	col.b = 0;
	col.a = 255;

	if (textures.find(caption) != textures.end()) {
		return textures[caption];
	}
	else {
		SDL_Surface* surf = TTF_RenderText_Blended(font, caption.c_str(), col);
		auto text = SDL_CreateTextureFromSurface(ren, surf);
		textures[caption] = text;
		return text;
	}
}

SDL_Rect ResourceManager::GetRectangle(int x, int y, int w, int h) {

	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

void ResourceManager::DestroyHero() {

	hero.reset();
}

Hero* ResourceManager::GetHero() {

	if (hero == nullptr) {
		hero = std::make_unique<Hero>(HeroLevel);
	}
	return (Hero*)hero.get();
}

Tilemap* ResourceManager::GetTilemap() {

	if (tilemap == nullptr) {
		tilemap = std::make_unique<Tilemap>();
	}
	return (Tilemap*)tilemap.get();
}

void ResourceManager::DestroyTilemap() {

	tilemap.reset();
}

NPC* ResourceManager::CreateNPC() {

	gameobjects.push_back(std::make_unique<NPC>());
	return (NPC*)gameobjects[gameobjects.size() - 1].get();
}

void ResourceManager::DestroyGameObject(GameObject* go) {

	for (int i = 0; i < gameobjects.size(); i++) {
		if (gameobjects[i].get() == go) {
			gameobjects[i].reset();
			gameobjects.erase(gameobjects.begin() + i);
			break;
		}
	}
}