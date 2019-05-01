#pragma once
#include "libs.h"
#include "Generator.h"
#include "ResourceManager.h"
#include "Constants.h"
#include "GameObject.h"
#include "Hero.h"

class Tilemap {
public:

	Tilemap() {};
	Tilemap(int x, int y, int x_rooms, int y_rooms, int x_room_size, int y_room_size, int room_count) {

		map = Generator::getFullMap2(x_rooms, y_rooms, x_room_size, y_room_size, room_count);

		pos.x = x;
		pos.y = y;
		pos.w = map[0].size() * TileSize;
		pos.h = map.size() * TileSize;
		tiletex = ResourceManager::LoadTexture("Graphics/Dungeon_Tileset.png");
	}

	Tilemap* getPtr() { return this; }

	void SetHero(Hero* h) {
		hero = h;
	}

	bool Move(GameObject* object, SDL_Point from, SDL_Point to) {
		if (gameobjects[std::make_pair(from.x, from.y)] != object) {
			return false;
		}
		if (map[to.y][to.x] == 1) {
			return false;
		}
		if (map[to.y][to.x] == 2) {
			map[to.y][to.x] = 3;
		}
		gameobjects.erase(std::make_pair(from.x, from.y));
		gameobjects[std::make_pair(to.x, to.y)] = object;
		return true;
	}

	void ResolveInput(SDL_Event& e) {

		if (e.type == SDL_KEYDOWN) {

			if (e.key.keysym.sym == SDLK_p) {
				if(RenderTileSize < 256)
					RenderTileSize *= 2;
			}
			else if (e.key.keysym.sym == SDLK_m) {
				if(RenderTileSize > 32)
					RenderTileSize /= 2;
			}

		}

		hero->ResolveInput(e);
		Centralize();

	}

	void Centralize() {
		pos.x = ScreenWidth / 2 - RenderTileSize / 2 - hero->getPosition().x * RenderTileSize;
		pos.y = ScreenHeight / 2 - RenderTileSize / 2 - hero->getPosition().y * RenderTileSize;
	}

	void LoadTextures() { 
	}

	GameObject* GetObjectOnTile(SDL_Point tilepos) {
		return (gameobjects.find(std::make_pair(tilepos.x,tilepos.y)) == gameobjects.end() ? nullptr : gameobjects[std::make_pair(tilepos.x, tilepos.y)]);
	}

	void OnRender() {

		SDL_Rect r;
		r.x = pos.x;
		r.y = pos.y;
		r.w = RenderTileSize;
		r.h = RenderTileSize;

		for (int i = 0; i < map.size(); i++) {
			for (int j = 0; j < map[0].size(); j++) {
				SDL_RenderCopy(ResourceManager::ren, tiletex, &GetTileRect(map[i][j]), &r);
				r.x += RenderTileSize;
			}
			r.x = pos.x;
			r.y += RenderTileSize;
		}

		for (auto& pair : gameobjects) {
			SDL_Rect r;
			r.x = pair.first.first * RenderTileSize + pos.x;
			r.y = pair.first.second * RenderTileSize + pos.y;
			r.w = RenderTileSize;
			r.h = RenderTileSize;
			SDL_RenderCopy(ResourceManager::ren, pair.second->tex, nullptr, &r);
			//std::cout << "Rendering hero at " << r.x << " " << r.y << " " << r.w << " " << r.h << std::endl;
		}

	}

	SDL_Texture* tiletex = nullptr;
	SDL_Rect pos;
	Map map;
	int room_count = 0;

	Hero* hero = nullptr;

	std::map<int, SDL_Rect> textureblocks;
	std::map<std::pair<int,int>, GameObject*> gameobjects;

};