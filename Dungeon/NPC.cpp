#include "NPC.h"
#include "ResourceManager.h"
#include "Tilemap.h"
#include "Hero.h"
#include "Constants.h"

NPC::NPC() {

	max_life = getLevelLife(ResourceManager::GetHero()->getLevel());
	cur_life = max_life;
	attack = getLevelAttack(ResourceManager::GetHero()->getLevel()) - 1;
	tex = ResourceManager::LoadTexture("Graphics/ghost.png");
	tex_aware = ResourceManager::LoadTexture("Graphics/ghost_aware.png");
}

int NPC::getMaxLife() {

	return max_life;
}

int NPC::getCurLife() {

	return cur_life;
}

void NPC::setLife(int value) {

	setMaxLife(value);
	setCurLife(value);
}

void NPC::setMaxLife(int value) {

	max_life = value;
}

void NPC::setCurLife(int value) {

	cur_life = value;
}

int NPC::getXP() {

	int lvl = level;
	std::cout << "Dying" << std::endl; // dying was moved here because of deallocated class variables
	ResourceManager::GetTilemap()->DestroyObject(this);
	ResourceManager::DestroyGameObject(this);
	return getLevelXPGain(lvl);
}

int NPC::getAttack() {

	return attack;
}

void NPC::setAttack(int value) {

	attack = value;
}

void NPC::Attack() {

	ResourceManager::GetHero()->Defend(attack);
}

void NPC::Defend(int amount) {

	// std::cout << "Defending" << std::endl;
	cur_life = std::max(0, cur_life - amount);
	isAware = true;
}

void NPC::OnTurn() {

	SDL_Point dst = tilepos;
	int rnd = std::rand() % 4;

	// if player is too close and enemy happens to be looking
	if(!isAware){
		int distance = std::abs(ResourceManager::GetHero()->tilepos.x - tilepos.x) + std::abs(ResourceManager::GetHero()->tilepos.y - tilepos.y);
		if (distance <= AwarenessDistance && std::rand() % ENEMY_AWARENESS == 0)
			isAware = true;
	}

	// if player is too far and enemy doesn't really care anymore
	else {
		int distance = std::abs(ResourceManager::GetHero()->tilepos.x - tilepos.x) + std::abs(ResourceManager::GetHero()->tilepos.y - tilepos.y);
		if (distance > AwarenessDistance && std::rand() % ENEMY_AWARENESS != 0)
			isAware = false;
	}

	// skip turn if too lazy
	if (std::rand() % ENEMY_INITIATIVE != 0) {
		return;
	}

	// just wander around
	if (!isAware) {

		if (rnd == 0) {
			dst.x++;
		}
		else if (rnd == 1) {
			dst.x--;
		}
		else if (rnd == 2) {
			dst.y++;
		}
		else {
			dst.y--;
		}

		if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
			tilepos = dst;
		}
	}

	// go get that hero
	else {

		if (ResourceManager::GetTilemap()->CanAttack(this, ResourceManager::GetHero())) {
			Attack();
			return;
		}

		if (ResourceManager::GetHero()->tilepos.x < tilepos.x) {
			dst.x--;
			if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
				tilepos = dst;
				return;
			}
		}
		if (ResourceManager::GetHero()->tilepos.x > tilepos.x) {
			dst.x++;
			if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
				tilepos = dst;
				return;
			}
		}
		if (ResourceManager::GetHero()->tilepos.y < tilepos.y) {
			dst.y--;
			if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
				tilepos = dst;
				return;
			}
		}
		if (ResourceManager::GetHero()->tilepos.y > tilepos.y) {
			dst.y++;
			if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
				tilepos = dst;
				return;
			}
		}
	}
}

void NPC::levelUp() {

	if (!isAware) {
		level++;
		max_life = getLevelLife(level);
		cur_life = max_life;
		attack = getLevelAttack(level);
	}
}

bool NPC::CanAttack() { 

	return true; 
}

SDL_Texture* NPC::getTexture() {

	return isAware ? tex_aware : tex;
}

std::string NPC::toString() {

	return "HP:" + std::to_string(cur_life) + "/" + std::to_string(max_life) + ", ATT:" + std::to_string(attack) + ", LVL:" + std::to_string(level);
}
