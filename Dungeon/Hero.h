#pragma once
#include "libs.h"
#include "GameObject.h"

class Tilemap;

class Hero : public GameObject {

public:

	Hero(int life);
	~Hero() {};

	void Move(SDL_Point& dst);

	void ResolveInput(SDL_Event& e);

	int getLife();

	GameObject* getPtr() override;



private:
	int life;

};