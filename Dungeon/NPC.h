#pragma once
#include "libs.h"
#include "GameObject.h"

class Tilemap;

class NPC : public GameObject {
public:

	NPC(int,int);
	~NPC() {};

	void Move(SDL_Point& dst);

	GameObject* getPtr() override;

	SDL_Point getPosition();


private:

	SDL_Point tilepos;

};