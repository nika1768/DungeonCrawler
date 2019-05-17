#include "Hero.h"
#include "ResourceManager.h"
#include "Tilemap.h"

Hero::Hero(int life) {

	this->life = life;
	tex = ResourceManager::LoadTexture("Graphics/hero.png");
}

void Hero::Move(SDL_Point& dst) {

	if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
		tilepos = dst;
		ResourceManager::GetTilemap()->OnTurn();
	}


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

int Hero::getLife()
{
	return life;
}

GameObject* Hero::getPtr() {
	return this; 
}
