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

	void Heal(int percentage);

	void ResolveInput(SDL_Event& e);

	bool isHero() override { return true; };

	int getLife();
	void setLife(int value);


private:
	int max_life;
	int cur_life;
	int level;
	int attack;

};