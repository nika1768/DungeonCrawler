#pragma once
#include "libs.h"
#include "GameObject.h"

class Tilemap;

class NPC : public GameObject {
public:

	NPC();
	~NPC() {};

	int getMaxLife();
	int getCurLife();
	void setLife(int value);
	void setMaxLife(int value);
	void setCurLife(int value);

	int getAttack();
	void setAttack(int value);

	void Attack();
	void Defend(int amount);

	void OnTurn() override;

	bool CanAttack() override;

	SDL_Texture* getTexture() override;
private:

	int max_life = 0;
	int cur_life = 0;
	int attack;

	// is aware of player - if true, follows and tries to attack player; else moves randomly
	bool isAware = false;

	SDL_Texture* tex_aware = nullptr;
};