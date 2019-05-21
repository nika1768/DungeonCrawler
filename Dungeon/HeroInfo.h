#pragma once

#include "libs.h"
#include "Hero.h"
#include "ResourceManager.h"

class HeroInfo {
public:

	HeroInfo(Hero* hero) {
		this->hero = hero;
		rect = ResourceManager::GetRectangle(0, 0, 128, 96);
		data_rect = ResourceManager::GetRectangle(0, 0, 128, 32);
		recTex = ResourceManager::LoadTexture("Graphics/rectangle.png");
	}

	void OnUpdate() {
		hp_tex = ResourceManager::LoadTextTexture("Life: " + std::to_string(hero->getLife()), Black);
		lvl_tex = ResourceManager::LoadTextTexture("Level: " + std::to_string(hero->getLevel()), Black);
		att_tex = ResourceManager::LoadTextTexture("Attack: " + std::to_string(hero->getAttack()), Black);
	}

	void OnRender() {
		SDL_RenderCopy(ResourceManager::ren, recTex, nullptr, &rect);
		data_rect.y = 0;
		SDL_RenderCopy(ResourceManager::ren, hp_tex, nullptr, &data_rect);
		data_rect.y += 32;
		SDL_RenderCopy(ResourceManager::ren, lvl_tex, nullptr, &data_rect);
		data_rect.y += 32;
		SDL_RenderCopy(ResourceManager::ren, att_tex, nullptr, &data_rect);
	}



private:
	Hero* hero = nullptr;
	SDL_Texture* hp_tex = nullptr;
	SDL_Texture* lvl_tex = nullptr;
	SDL_Texture* att_tex = nullptr;
	SDL_Texture* recTex = nullptr;
	SDL_Rect rect;
	SDL_Rect data_rect;
};