#pragma once
#include "libs.h"

class GameObject {
public:

	~GameObject() = default;

	virtual GameObject* getPtr() = 0;

	SDL_Texture* tex;
	SDL_Point tilepos;

};