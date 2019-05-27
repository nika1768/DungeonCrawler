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
		key_src_rect = GetTileRect(TILE_GOLD_KEY);
		key_dst_rect.w = RenderTileSize;
		key_dst_rect.h = RenderTileSize;
		key_dst_rect.x = 0;
		key_dst_rect.y = 96;
		recTex = ResourceManager::LoadTexture("Graphics/rectangle.png");
		tiles = ResourceManager::LoadTexture("Graphics/Dungeon_Tileset.png");
		
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
		if (hero->checkItem(TILE_GOLD_KEY)) {
			SDL_RenderCopy(ResourceManager::ren, tiles, &key_src_rect, &key_dst_rect);
		}
	}



private:
	Hero* hero = nullptr;
	SDL_Texture* hp_tex = nullptr;
	SDL_Texture* lvl_tex = nullptr;
	SDL_Texture* att_tex = nullptr;
	SDL_Texture* recTex = nullptr;
	SDL_Texture* tiles = nullptr; // same tilemap used in Tilemap.h
	SDL_Rect key_src_rect;
	SDL_Rect key_dst_rect;
	SDL_Rect rect;
	SDL_Rect data_rect;
};