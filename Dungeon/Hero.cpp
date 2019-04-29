#include "Hero.h"
#include "ResourceManager.h"
#include "Tilemap.h"

Hero::Hero() {
	tilepos.x = 5;
	tilepos.y = 5;
	ResourceManager::GetTilemap()->gameobjects[std::make_pair(tilepos.x, tilepos.y)] = this;
	tex = ResourceManager::LoadTexture("pics/ghost.png");
}

void Hero::Move(SDL_Point& dst) {

	ResourceManager::GetTilemap()->Move(this, tilepos, dst);
	tilepos = dst;

}

void Hero::ResolveInput(SDL_Event& e) {
	SDL_Point dst = tilepos;
	if (e.type == SDL_KEYUP)
		return;
	if (e.key.keysym.sym == SDLK_DOWN) {
		dst.y++;
	}
	else if (e.key.keysym.sym == SDLK_UP) {
		dst.y--;
	}
	else if (e.key.keysym.sym == SDLK_LEFT) {
		dst.x--;
	}
	else if (e.key.keysym.sym == SDLK_RIGHT) {
		dst.x++;
	}
	GameObject* object = ResourceManager::GetTilemap()->GetObjectOnTile(dst);
	if (object == nullptr) {
		Move(dst);
	}

}

GameObject* Hero::getPtr() {
	return this; 
}

SDL_Point Hero::getPosition() {
	return tilepos;
}