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

	for (int i = 0; i < map.size(); i++) {
		std::vector<GameObject*> line;
		for (int j = 0; j < map[0].size(); j++) {
			line.push_back(nullptr);
		}
		gameobjects.push_back(line);
	}
}

void Tilemap::Populate(int chance_modifier = 20)
{
	for (int i = 1; i < map.size() - 1; i++) {
		for (int j = 1; j < map[0].size() - 1; j++) {

			if (map[i][j] == 0) {
				if (std::rand() % chance_modifier == 0) {
					NPC* npc = ResourceManager::CreateNPC();
					npc->setLife(20);
					gameobjects[i][j] = npc;
					npc->setPosition(i, j);
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
	if (gameobjects[from.y][from.x] != object) {
		std::cout << "ERROR, invalid movement." << std::endl;
		return false;
	}
	if (map[to.y][to.x] == 1) {
		return false;
	}
	if (map[to.y][to.x] == 2) {
		// DOOR LOGIC TO MOVE
		map[to.y][to.x] = 3;
	}
	if (gameobjects[to.y][to.x] != nullptr) {
		return false;
	}
	gameobjects[from.y][from.x] = nullptr;
	gameobjects[to.y][to.x] = object;

	return true;
}

void Tilemap::OnTurn() {
	
	std::cout << "On Turn" << std::endl;
	int count = 0;

	for (int i = 0; i < gameobjects.size(); i++) {
		for (int j = 0; j < gameobjects[0].size(); j++) {
			if (gameobjects[i][j] != nullptr) {
				gameobjects[i][j]->OnTurn();
				count++;
			}
		}
	}

	std::cout << "...called " << count << " times." << std::endl;

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
	hp_tex = ResourceManager::LoadTexture("Graphics/healthbar.png");
}

GameObject* Tilemap::GetObjectOnTile(SDL_Point tilepos) {
	return gameobjects[tilepos.y][tilepos.x];
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

	for (int i = 0; i < gameobjects.size(); i++)
	{
		for (int j = 0; j < gameobjects[0].size(); j++)
		{
			if (gameobjects[i][j] != nullptr) {

				if (gameobjects[i][j]->tilepos.x != j || gameobjects[i][j]->tilepos.y != i) {
					std::cout << "Position invalid: " << gameobjects[i][j]->tilepos.x << " " << gameobjects[i][j]->tilepos.y << " - " << j << " " << i << std::endl;
				}

				SDL_Rect r;
				r.x = j * RenderTileSize + pos.x;
				r.y = i * RenderTileSize + pos.y;
				r.w = RenderTileSize;
				r.h = RenderTileSize;
				SDL_RenderCopy(ResourceManager::ren, gameobjects[i][j]->tex, nullptr, &r);

				if (gameobjects[i][j]->CanAttack()) {
					r.w = RenderTileSize * ((NPC*)gameobjects[i][j])->getCurLife() / ((NPC*)gameobjects[i][j])->getMaxLife();
					r.h = RenderTileSize / 8;
					r.y += (RenderTileSize * 7 / 8);
					SDL_RenderCopy(ResourceManager::ren, hp_tex, nullptr, &r);
				}

			}
		}
	}

}
