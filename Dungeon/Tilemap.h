#pragma once
#include "libs.h"
#include "Generator.h"


class NPC;
class Hero;
class GameObject;

class Tilemap {
public:

	Tilemap() {};
	Tilemap(int x, int y, int x_rooms, int y_rooms, int x_room_size, int y_room_size, int room_count);

	void SetHero(Hero* h);
	bool CanMove(GameObject* object, SDL_Point from, SDL_Point to);
	void OnTurn();
	void ResolveInput(SDL_Event& e);
	void Centralize();
	void LoadTextures();
	void OnRender();

	GameObject* GetObjectOnTile(SDL_Point tilepos);
	Tilemap* getPtr();

	SDL_Texture* tiletex = nullptr;
	Hero* hero = nullptr;
	int room_count = 0;
	SDL_Rect pos;
	Map map;
	

	std::map<int, SDL_Rect> textureblocks;
	std::map<std::pair<int,int>, GameObject*> gameobjects;

};