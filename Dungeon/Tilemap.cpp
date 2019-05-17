#include "Tilemap.h"
#include "ResourceManager.h"
#include "Constants.h"
#include "GameObject.h"
#include "Hero.h"
#include "NPC.h"

Tilemap::Tilemap(int x, int y, int x_rooms, int y_rooms, int x_room_size, int y_room_size, int room_count) {

	map = Generator::getFullMap2(x_rooms, y_rooms, x_room_size, y_room_size, room_count);

	pos.x = x;
	pos.y = y;
	pos.w = map[0].size() * TileSize;
	pos.h = map.size() * TileSize;
	tiletex = ResourceManager::LoadTexture("Graphics/Dungeon_Tileset.png");

	for (int i = 1; i < map.size(); i++) {
		for (int j = 1; j < map[0].size(); j++) {

			if (map[i][j] == 0) {
				if (std::rand() % 20 == 19) {
					NPC* npc = ResourceManager::CreateNPC();
					gameobjects[std::make_pair(j, i)] = npc;
					npc->setPosition(j, i);
				}
			}
		}
	}
}

Tilemap* Tilemap::getPtr() { return this; }

void Tilemap::SetHero(Hero* h) {
	hero = h;
}

bool Tilemap::CanMove(GameObject* object, SDL_Point from, SDL_Point to) {
	if (gameobjects[std::make_pair(from.x, from.y)] != object) {
		return false;
	}
	if (map[to.y][to.x] == 1) {
		return false;
	}
	if (map[to.y][to.x] == 2) {
		map[to.y][to.x] = 3;
	}
	gameobjects[std::make_pair(from.x, from.y)] = nullptr;
	gameobjects[std::make_pair(to.x, to.y)] = object;
	return true;
}

void Tilemap::OnTurn() {
	
	for (auto& pair : gameobjects) {
		if (pair.second != nullptr)
			pair.second->OnTurn();
	}
}

void Tilemap::ResolveInput(SDL_Event& e) {

	if (e.type == SDL_KEYDOWN) {

		if (e.key.keysym.sym == SDLK_p) {
			if (RenderTileSize < 256)
				RenderTileSize *= 2;
		}
		else if (e.key.keysym.sym == SDLK_m) {
			if (RenderTileSize > 32)
				RenderTileSize /= 2;
		}

	}

	hero->ResolveInput(e);
	Centralize();

}

void Tilemap::Centralize() {
	pos.x = ScreenWidth / 2 - RenderTileSize / 2 - hero->getPosition().x * RenderTileSize;
	pos.y = ScreenHeight / 2 - RenderTileSize / 2 - hero->getPosition().y * RenderTileSize;
}

void Tilemap::LoadTextures() {
}

GameObject* Tilemap::GetObjectOnTile(SDL_Point tilepos) {
	return (gameobjects.find(std::make_pair(tilepos.x, tilepos.y)) == gameobjects.end() ? nullptr : gameobjects[std::make_pair(tilepos.x, tilepos.y)]);
}

void Tilemap::OnRender() {

	SDL_Rect r = ResourceManager::GetRectangle(pos.x, pos.y, RenderTileSize, RenderTileSize);

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
		if (pair.second != nullptr)
			SDL_RenderCopy(ResourceManager::ren, pair.second->tex, nullptr, &r);
		//std::cout << "Rendering hero at " << r.x << " " << r.y << " " << r.w << " " << r.h << std::endl;
	}

}
