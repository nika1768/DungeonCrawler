#pragma once
#include "libs.h"
#include "GameObject.h"

class Tilemap;

class NPC : public GameObject {
public:

	NPC();
	~NPC() {};

	GameObject* getPtr() override;
	void OnTurn() override;


private:
	int life;

};