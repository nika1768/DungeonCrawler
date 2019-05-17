#pragma once
#include "libs.h"
#include "GameObject.h"

class Tilemap;
class NPC;

class Hero : public GameObject {

public:

	Hero(int life);
	~Hero() {};

	void Move(SDL_Point& dst);

	void Attack(NPC* defender);

	void Defend(int amount);

	void ResolveInput(SDL_Event& e);

	int getLife();
	void setLife(int value);

	GameObject* getPtr() override;



private:
	int life;
	int level;
	int attack;

};