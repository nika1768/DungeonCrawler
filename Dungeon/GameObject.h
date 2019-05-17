#pragma once
#include "libs.h"
#include "ResourceManager.h"
#include "Tilemap.h"

class GameObject {
public:

	~GameObject() = default;
	
	virtual void setPosition(int x, int y);

	SDL_Point getPosition();

	virtual GameObject* getPtr() = 0;
	virtual void OnTurn() {};

	virtual bool CanAttack() { return false; };

	SDL_Texture* tex;
	SDL_Point tilepos;

};