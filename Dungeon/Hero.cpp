#include "Hero.h"
#include "ResourceManager.h"
#include "Tilemap.h"
#include "NPC.h"

Hero::Hero(int level) {

	this->level = level;
	xp = 0;
	max_life = 10 * level;
	cur_life = max_life;
	attack = 2 * level;
	tex = ResourceManager::LoadTexture("Graphics/hero.png");
}

void Hero::Move(SDL_Point& dst) {
	tilepos = dst;
	ResourceManager::GetTilemap()->ClearFog();
	ResourceManager::GetTilemap()->OnTurn();
}

void Hero::Attack(NPC* defender) {

	defender->Defend(attack);
	
	// killed enemy
	if (defender->getCurLife() <= 0) {
		gainXP(defender->getXP());
	}

	ResourceManager::GetTilemap()->OnTurn();
}

void Hero::Defend(int amount) {

	cur_life -= amount;
	if (cur_life <= 0) {
		cur_life = 0;
		ResourceManager::GetTilemap()->EndGame();
	}
}

void Hero::Heal(int percentage) {

	cur_life = std::min(max_life, cur_life + max_life * percentage / 100);
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
	// debug
	else if (e.key.keysym.sym == SDLK_SPACE) { 
		ResourceManager::GetTilemap()->OnTurn();
	}

	GameObject* object = ResourceManager::GetTilemap()->GetObjectOnTile(dst);

	if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
		Move(dst);
	}
	else if (object != nullptr) {
		if (object->CanAttack()) {
			Attack((NPC*)object);
		}
	}
}

int Hero::getLife() {

	return cur_life;
}

void Hero::setLife(int value) {

	cur_life = value;
}

int Hero::getLevel() {

	return level;
}

void Hero::setLevel(int amount) {

	level = amount - 1;
	levelUp();
}

int Hero::getAttack() {

	return attack;
}

void Hero::setAttack(int amount) {

	attack = amount;
}

int Hero::getXP() {

	return xp;
}

void Hero::gainXP(int amount) {

	xp += amount;
	int cap = getLevelXPCap(level);
	if (xp >= cap) {
		levelUp();
	}
}

void Hero::levelUp() {

	level++;
	max_life = getLevelLife(level);
	attack = getLevelAttack(level);
	ResourceManager::GetTilemap()->OnLevelUp();
}

void Hero::gainItem(int item_code) {

	switch (item_code) {
	case TILE_GOLD_KEY:
		has_gold_key = true;
		break;
	}
}

bool Hero::checkItem(int item_code) {

	switch (item_code) {
	case TILE_GOLD_KEY:
		return has_gold_key;
	}
}

void Hero::deleteItems() {
	// in this version, hero can have only gold key
	has_gold_key = false;
}
