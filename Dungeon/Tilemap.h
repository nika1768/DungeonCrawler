#pragma once
#include "libs.h"
#include "Generator.h"
#include "ResourceManager.h"
#include "Bordel.h"

class Tilemap {
public:

	Tilemap(int x, int y, int w, int h, int min, int max) {
		map = Generator::getMap(w, h);
		while (Generator::findRoom(map, min, max));
		pos.x = x;
		pos.y = y;
		pos.w = map[0].size() * TileSize;
		pos.h = map.size() * TileSize;
	}

	void LoadTextures() {

		tiletex = ResourceManager::LoadTexture("Graphics/Tilemap.png");
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

	void OnRender() {

		SDL_Rect r;
		r.x = pos.x;
		r.y = pos.y;
		r.w = TileSize;
		r.h = TileSize;

		for (int i = 0; i < map.size(); i++) {
			for (int j = 0; j < map[0].size(); j++) {
				SDL_RenderCopy(ResourceManager::ren, tiletex, &textureblocks[map[i][j]], &r);
				r.x += TileSize;
			}
			r.x = pos.x;
			r.y += TileSize;
		}

	}

	SDL_Texture* tiletex;
	SDL_Rect pos;
	Map map;

	std::map<int, SDL_Rect> textureblocks;

};