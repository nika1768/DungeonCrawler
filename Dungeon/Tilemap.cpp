#include "Tilemap.h"
#include "ResourceManager.h"
#include "Constants.h"
#include "GameObject.h"
#include "Hero.h"
#include "NPC.h"

Tilemap::Tilemap(int x, int y, int x_rooms, int y_rooms, int x_room_size, int y_room_size, int room_count) {

	map = Generator::getFullMap(x_rooms, y_rooms, x_room_size, y_room_size, room_count);
	// fog_map[i][j] == true => uncovered tiles
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

Tilemap::~Tilemap() {

	for (auto& unit : units) {
		ResourceManager::DestroyGameObject(unit);
	}
}

void Tilemap::Populate() {

	// Add unique tiles to map
	bool floor_door = false;
	bool gold_key = false;
	while (true) {
		int i = std::rand() % (map.size() - 1);
		int j = std::rand() % (map[0].size() - 1);
		if (map[i][j] == 0) {
			if (!floor_door) {
				map[i][j] = TILE_DOOR_FLOOR;
				floor_door = true;
			}
			else if (!gold_key) {
				map[i][j] = TILE_GOLD_KEY;
				gold_key = true;
			}
			else {
				break;
			}
		}
	}

	// Add other tiles
	for (int i = 1; i < map.size() - 1; i++) {
		for (int j = 1; j < map[0].size() - 1; j++) {
			if (map[i][j] == 0) {
				if (std::rand() % ENEMY_CHANCE == 0) {
					NPC* npc = ResourceManager::CreateNPC();
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
		return false;
	}

	// There's a unit on tile
	else if (object_map[to.y][to.x] != nullptr) {
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

	// Door shaft, opens to ladder with gold key in inventory, passable
	else if (map[to.y][to.x] == TILE_DOOR_FLOOR) {
		if (object == ResourceManager::GetHero() && ResourceManager::GetHero()->checkItem(TILE_GOLD_KEY)) {
			map[to.y][to.x] = TILE_LADDER_FLOOR;
			return false;
		}
	}
	
	// Ladder, start next level
	else if (map[to.y][to.x] == TILE_LADDER_FLOOR) {
		if (object == ResourceManager::GetHero()) {
			SDL_Event e;
			e.type = SDL_USEREVENT;
			e.user.code = USER_NEXT_LEVEL;
			SDL_PushEvent(&e);
		}
		else {
			return false;
		}
	}

	// Gold key, passable, pickable by hero
	else if (map[to.y][to.x] == TILE_GOLD_KEY) {
		if (object == ResourceManager::GetHero()) {
			ResourceManager::GetHero()->gainItem(TILE_GOLD_KEY);
			map[to.y][to.x] = TILE_FLOOR;
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

	object_map[from.y][from.x] = nullptr;
	object_map[to.y][to.x] = object;
	return true;
}

bool Tilemap::CanAttack(GameObject* attacker, GameObject* defender) {

	if (std::abs(attacker->tilepos.x - defender->tilepos.x) + std::abs(attacker->tilepos.y - defender->tilepos.y) == 1) {
		return true;
	}
	return false;
}

void Tilemap::ClearFog() {

	auto tile = ResourceManager::GetHero()->tilepos;
	for (int i = std::max(tile.y - PLAYER_SIGHT, 0); i < std::min(tile.y + PLAYER_SIGHT, (int)fog_map.size()); i++) {
		for (int j = std::max(tile.x - PLAYER_SIGHT, 0); j < std::min(tile.x + PLAYER_SIGHT, (int)fog_map[0].size()); j++)
		{
			fog_map[i][j] = true;
		}
	}
}

void Tilemap::OnTurn() {

	for (int i = 0; i < units.size(); i++) {
		units[i]->OnTurn();
	}
}

void Tilemap::ResolveInput(SDL_Event& e) {

	if (e.type == SDL_KEYDOWN) {

		// zoom map
		if (e.key.keysym.sym == SDLK_p || e.key.keysym.sym == SDLK_KP_PLUS) {
			if (RenderTileSize < 256)
				RenderTileSize *= 2;
		}
		else if (e.key.keysym.sym == SDLK_m || e.key.keysym.sym == SDLK_KP_MINUS) {
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

	bool enable_fog = true;

	SDL_Rect r = ResourceManager::GetRectangle(pos.x, pos.y, RenderTileSize, RenderTileSize);

	// render tiles on tilemap
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[0].size(); j++) {
			if (!enable_fog || fog_map[i][j]) {
				SDL_RenderCopy(ResourceManager::ren, tiletex, &GetFloorRect(i, j), &r);
				if (map[i][j] != 0) {
					SDL_RenderCopy(ResourceManager::ren, tiletex, &GetTileRect(map[i][j]), &r);
				}
			}
			r.x += RenderTileSize;
		}
		r.x = pos.x;
		r.y += RenderTileSize;
	}

	// render objects
	for (int i = 0; i < object_map.size(); i++) {
		for (int j = 0; j < object_map[0].size(); j++) {
			if (object_map[i][j] != nullptr && (!enable_fog || fog_map[i][j])) {
				
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

void Tilemap::OnLevelUp() {

	for (auto& unit : units) {
		unit->levelUp();
	}
}

void Tilemap::Debug() {

	std::cout << "DEBUG" << std::endl;
	for (auto& unit : units) {
		std::cout << unit->toString() << std::endl;
	}
}

void Tilemap::EndGame() {

	SDL_Event ev;
	ev.type = SDL_QUIT;
	SDL_PushEvent(&ev);
}

void Tilemap::DestroyObject(GameObject* go) {

	for (int i = 0; i < units.size(); i++) {
		if (units[i] == go) {
			units.erase(units.begin() + i);
			break;
		}
	}
	object_map[go->tilepos.y][go->tilepos.x] = nullptr;
	ResourceManager::DestroyGameObject(go);
}
