#pragma once
#include "libs.h"
#include "GameObject.h"
#include "Constants.h"
; // C2236
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
	void levelUp();

	void ResolveInput(SDL_Event& e);

	int getLife();
	void setLife(int value);
	int getLevel();
	void setLevel(int amount);
	int getAttack();
	void setAttack(int amount);
	int getXP();
	void gainXP(int amount);
	

	void gainItem(int item_code);
	bool checkItem(int item_code);

private:
	int max_life;
	int cur_life;
	int level;
	int attack;
	int xp;

	bool has_gold_key = false;
};