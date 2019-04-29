#pragma once
#include "libs.h"
#include "GameObject.h"

class Tilemap;

class Hero : public GameObject {

public:

	Hero();
	~Hero() {};

	void Move(SDL_Point& dst);

	void ResolveInput(SDL_Event& e);

	GameObject* getPtr() override;

	SDL_Point getPosition();


private:

	SDL_Point tilepos;

};