#pragma once

#include "libs.h"
#include "Hero.h"
#include "ResourceManager.h"

class HeroInfo {
public:

	HeroInfo(Hero* hero) {
		this->hero = hero;
		rect = ResourceManager::GetRectangle(0, 0, 128, 32);
		recTex = ResourceManager::LoadTexture("Graphics/rectangle.png");
	}

	void OnUpdate() {
		int life = hero->getLife();
		tex = ResourceManager::LoadTextTexture("life: " + std::to_string(life), Black);
	}

	void OnRender() {
		SDL_RenderCopy(ResourceManager::ren, recTex, &rect, &rect);
		SDL_RenderCopy(ResourceManager::ren, tex, &rect, &rect);
	}



private:
	Hero* hero = nullptr;
	SDL_Texture* tex = nullptr;
	SDL_Texture* recTex = nullptr;
	SDL_Rect rect;
};