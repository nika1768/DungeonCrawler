#include "Hero.h"
#include "ResourceManager.h"
#include "Tilemap.h"
#include "NPC.h"

Hero::Hero(int level) {

	this->level = level;
	this->life = 10 * level;
	this->attack = 2 * level;
	tex = ResourceManager::LoadTexture("Graphics/hero.png");
}

void Hero::Move(SDL_Point& dst) {
	tilepos = dst;
	ResourceManager::GetTilemap()->OnTurn();
	
}

void Hero::Attack(NPC* defender) {

	std::cout << "Attacking" << std::endl;
	defender->Defend(attack);
	ResourceManager::GetTilemap()->OnTurn();

}

void Hero::Defend(int amount) {

}

void Hero::ResolveInput(SDL_Event& e) {
	SDL_Point dst = tilepos;
	if (e.type == SDL_KEYUP)
		return;
	if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
		dst.y++;
	}
	else if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
		dst.y--;
	}
	else if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
		dst.x--;
	}
	else if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
		dst.x++;
	}
	else if (e.key.keysym.sym == SDLK_SPACE) {
		ResourceManager::GetTilemap()->OnTurn();
	}
	GameObject* object = ResourceManager::GetTilemap()->GetObjectOnTile(dst);

	if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
		Move(dst);
	}
	else if (object->CanAttack()) {
		Attack((NPC*)object);
	}

}

int Hero::getLife()
{
	return life;
}

void Hero::setLife(int value)
{
	life = value;
}

GameObject* Hero::getPtr() {
	return this; 
}
