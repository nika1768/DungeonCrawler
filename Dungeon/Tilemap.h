#pragma once
#include "libs.h"
#include "Generator.h"
#include "ResourceManager.h"
#include "Bordel.h"
#include "GameObject.h"
#include "Hero.h"

class Tilemap {
public:

	Tilemap() {};
	Tilemap(int x, int y, int w, int h, int min, int max, int room_count = -1) {
		/*
		map = Generator::getMap(w, h);
		if (room_count <= 0) {
			int counter = 2;
			while (Generator::findRoom(map, counter, min, max)) {
				counter++;
			}
			room_count = counter;
		}
		else {
			for (int i = 0; i < room_count; i++)
				Generator::findRoom(map, i+2, min, max);
			this->room_count = room_count;
		}
		*/

		map = Generator::getFullMap(w, h, min, max, room_count);

		pos.x = x;
		pos.y = y;
		pos.w = map[0].size() * TileSize;
		pos.h = map.size() * TileSize;
		tiletex = ResourceManager::LoadTexture("Graphics/Tilemap.png");
	}

	Tilemap* getPtr() { return this; }

	void SetHero(Hero* h) {
		hero = h;
	}

	void Move(GameObject* object, SDL_Point from, SDL_Point to) {
		if (gameobjects[std::make_pair(from.x, from.y)] != object) {
			std::cout << "A horrible event has occured, someone has teleported!" << std::endl;
			return;
		}
		gameobjects.erase(std::make_pair(from.x, from.y));
		gameobjects[std::make_pair(to.x, to.y)] = object;
	}

	void Centralize() {
		pos.x = ScreenWidth / 2 - TileSize / 2 - hero->getPosition().x * TileSize*2;
		pos.y = ScreenHeight / 2 - TileSize / 2 - hero->getPosition().y * TileSize*2;
	}

	void MoveMap(SDL_Point& p) {
		pos.x += p.x;
		pos.y += p.y;
	}

	void LoadTextures() {

		SDL_Rect r;
		r.x = 64;
		r.y = 288;
		r.w = TileSize;
		r.h = TileSize;
		textureblocks[1] = r;
		r.x = 160;
		r.y = 256;
		textureblocks[2] = r;

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
				SDL_RenderCopy(ResourceManager::ren, tiletex, &textureblocks[map[i][j]], &r);
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
			std::cout << "Rendering hero at " << r.x << " " << r.y << " " << r.w << " " << r.h << std::endl;
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