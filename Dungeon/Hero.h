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
	int getLevel();
	void setLevel(int amount);
	int getAttack();
	void setAttack(int amount);
	void gainXP(int amount);
	void levelUp();


private:
	int max_life;
	int cur_life;
	int level;
	int attack;
	int xp;

};