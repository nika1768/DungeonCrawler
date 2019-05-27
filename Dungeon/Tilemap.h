#pragma once
#include "libs.h"
#include "Generator.h"

class NPC;
class Hero;
class GameObject;

class Tilemap {
public:

	// constructor + setup methods
	Tilemap() {};
	Tilemap(int x, int y, int x_rooms, int y_rooms, int x_room_size, int y_room_size, int room_count);
	~Tilemap();
	void LoadTextures();
	void Populate();

	// standard game methods
	void ResolveInput(SDL_Event& e);
	void Centralize();
	void OnTurn();
	void OnRender();
	void OnLevelUp();
	void EndGame();

	// help functions
	GameObject* GetObjectOnTile(SDL_Point tilepos);
	bool CanMove(GameObject* object, SDL_Point from, SDL_Point to);
	bool CanAttack(GameObject* attacker, GameObject* defender);
	void ClearFog();
	void Debug();

	// cleanup methods
	void DestroyObject(GameObject* go);

	// render attributes
	SDL_Texture* hp_tex = nullptr;
	SDL_Rect pos;
	SDL_Texture* tiletex = nullptr;
	std::map<int, SDL_Rect> textureblocks;

	// tilemap attributes
	Map map;
	std::vector<std::vector<GameObject*>> object_map;
	std::vector<std::vector<bool>> fog_map;

	std::vector<NPC*> units;
};