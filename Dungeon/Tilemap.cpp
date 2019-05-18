#include "Tilemap.h"
#include "ResourceManager.h"
#include "Constants.h"
#include "GameObject.h"
#include "Hero.h"
#include "NPC.h"

Tilemap::Tilemap(int x, int y, int x_rooms, int y_rooms, int x_room_size, int y_room_size, int room_count) {

	map = Generator::getFullMap2(x_rooms, y_rooms, x_room_size, y_room_size, room_count);
	fog_map = Generator::getMapB(x_rooms * x_room_size + x_rooms + 1, y_rooms * y_room_size + y_rooms + 1);

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
		object_map.push_back(line);
	}
}

void Tilemap::Populate()
{
	for (int i = 1; i < map.size() - 1; i++) {
		for (int j = 1; j < map[0].size() - 1; j++) {
			if (map[i][j] == 0) {
				if (std::rand() % ENEMY_CHANCE == 0) {
					NPC* npc = ResourceManager::CreateNPC();
					npc->setLife(20);
					units.push_back(npc);
					object_map[i][j] = npc;
					npc->setPosition(i, j);
				}
				else if (std::rand() % POTION_CHANCE == 0) {
					if (std::rand() % 3 == 0) {
						map[i][j] = TILE_BIG_POTION;
					}
					else {
						map[i][j] = TILE_SMALL_POTION;
					}
				}
			}
		}
	}
}

bool Tilemap::CanMove(GameObject* object, SDL_Point from, SDL_Point to) {

	// Exception
	if (object_map[from.y][from.x] != object) {
		std::cout << "ERROR, invalid movement." << std::endl;
		return false;
	}

	// Wall
	else if (map[to.y][to.x] == TILE_WALL) {
		return false;
	}

	// Door, can be opened by player, when opened passed by anyone
	else if (map[to.y][to.x] == TILE_DOOR_CLOSED_LR) {
		if (object == ResourceManager::GetHero()) {
			map[to.y][to.x] = TILE_DOOR_OPENED_LR;
		}
		else {
			return false;
		}
	}

	// Door, same
	else if (map[to.y][to.x] == TILE_DOOR_CLOSED_TB) {
		if (object == ResourceManager::GetHero()) {
			map[to.y][to.x] = TILE_DOOR_OPENED_TB;
		}
		else {
			return false;
		}
	}

	// Small potion, passable, consumed only by player
	else if (map[to.y][to.x] == TILE_SMALL_POTION) {
		if (object == ResourceManager::GetHero()) {
			map[to.y][to.x] = TILE_FLOOR;
			ResourceManager::GetHero()->Heal(25);
		}
	}

	// Big potion, same
	else if (map[to.y][to.x] == TILE_BIG_POTION) {
		if (object == ResourceManager::GetHero()) {
			map[to.y][to.x] = TILE_FLOOR;
			ResourceManager::GetHero()->Heal(50);
		}
	}

	// There's a unit on tile
	else if (object_map[to.y][to.x] != nullptr) {
		return false;
	}

	object_map[from.y][from.x] = nullptr;
	object_map[to.y][to.x] = object;
	return true;
}

bool Tilemap::CanAttack(GameObject* attacker, GameObject* defender)
{
	if (std::abs(attacker->tilepos.x - defender->tilepos.x) + std::abs(attacker->tilepos.y - defender->tilepos.y) == 1) {
		return true;
	}
	return false;
}

SDL_Point Tilemap::FindPath(SDL_Point from, SDL_Point to)
{
	// TODO
	return SDL_Point();
}

void Tilemap::ClearFog()
{
	auto tile = ResourceManager::GetHero()->tilepos;
	for (int i = std::max(tile.y - PLAYER_SIGHT, 0); i < std::min(tile.y + PLAYER_SIGHT, (int)fog_map.size()); i++) {
		for (int j = std::max(tile.x - PLAYER_SIGHT, 0); j < std::min(tile.x + PLAYER_SIGHT, (int)fog_map[0].size()); j++)
		{
			fog_map[i][j] = true;
		}
	}
}

void Tilemap::OnTurn() {
	for (int i = 0; i < units.size(); i++)
	{
		units[i]->OnTurn();
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

	ResourceManager::GetHero()->ResolveInput(e);
	Centralize();

}

void Tilemap::Centralize() {
	pos.x = ScreenWidth / 2 - RenderTileSize / 2 - ResourceManager::GetHero()->getPosition().x * RenderTileSize;
	pos.y = ScreenHeight / 2 - RenderTileSize / 2 - ResourceManager::GetHero()->getPosition().y * RenderTileSize;
}

void Tilemap::LoadTextures() {
	hp_tex = ResourceManager::LoadTexture("Graphics/healthbar.png");
}

GameObject* Tilemap::GetObjectOnTile(SDL_Point tilepos) {
	return object_map[tilepos.y][tilepos.x];
}

void Tilemap::OnRender() {

	SDL_Rect r = ResourceManager::GetRectangle(pos.x, pos.y, RenderTileSize, RenderTileSize);

	// render tiles on tilemap
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[0].size(); j++) {
			if (fog_map[i][j]) {
				if (map[i][j] != 0)
					SDL_RenderCopy(ResourceManager::ren, tiletex, &GetTileRect(0), &r);
				SDL_RenderCopy(ResourceManager::ren, tiletex, &GetTileRect(map[i][j]), &r);
			}
			r.x += RenderTileSize;
		}
		r.x = pos.x;
		r.y += RenderTileSize;
	}

	// render objects
	for (int i = 0; i < object_map.size(); i++)
	{
		for (int j = 0; j < object_map[0].size(); j++)
		{
			if (object_map[i][j] != nullptr && fog_map[i][j]) {
				
				/*
				// debug
				if (object_map[i][j]->tilepos.x != j || object_map[i][j]->tilepos.y != i) {
					std::cout << "Position invalid: " << object_map[i][j]->tilepos.x << " " << object_map[i][j]->tilepos.y << " - " << j << " " << i << std::endl;
				}
				*/

				SDL_Rect r;
				r.x = j * RenderTileSize + pos.x;
				r.y = i * RenderTileSize + pos.y;
				r.w = RenderTileSize;
				r.h = RenderTileSize;
				SDL_RenderCopy(ResourceManager::ren, object_map[i][j]->getTexture(), nullptr, &r);

				if (object_map[i][j]->CanAttack()) {
					r.w = (RenderTileSize - 2) * ((NPC*)object_map[i][j])->getCurLife() / ((NPC*)object_map[i][j])->getMaxLife();
					r.x += 1;
					r.h = RenderTileSize / 8;
					r.y += (RenderTileSize * 7 / 8);
					SDL_RenderCopy(ResourceManager::ren, hp_tex, nullptr, &r);
				}

			}
		}
	}

}

void Tilemap::DestroyObject(GameObject* go)
{
	for (int i = 0; i < units.size(); i++)
	{
		if (units[i] == go) {
			units.erase(units.begin() + i);
			break;
		}
	}
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i] == go) {
			items.erase(items.begin() + i);
			break;
		}
	}
	object_map[go->tilepos.y][go->tilepos.x] = nullptr;
}
