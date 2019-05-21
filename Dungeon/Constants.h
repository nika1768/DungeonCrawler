#pragma once
#include "libs.h"

const int TileSize = 32;
static int ratio = 2;
const int DefaultRenderTileSize = 64;
static int RenderTileSize = 64;
const int ScreenWidth = 1600;
const int ScreenHeight = 800;
const int HeroLevel = 1;
const int AwarenessDistance = 4;
const int font_size = 13;
const int ENEMY_AWARENESS = 6;
const int ENEMY_INITIATIVE = 2;
const int PLAYER_SIGHT = 5;
const int ENEMY_CHANCE = 20;
const int POTION_CHANCE = 30;

const std::string font_path = "Resources/calibri.ttf";

enum Color {Black};

// MAP CONSTS
const int TILE_FLOOR = 0;
const int TILE_WALL = 1;
const int TILE_DOOR_CLOSED_LR = 2;
const int TILE_DOOR_OPENED_LR = 3;
const int TILE_DOOR_CLOSED_TB = 4;
const int TILE_DOOR_OPENED_TB = 5;
const int TILE_SMALL_POTION = 6;
const int TILE_BIG_POTION = 7;

const int TILE_FLOOR_A = 50;
const int TILE_FLOOR_B = 51;
const int TILE_FLOOR_C = 52;
const int TILE_FLOOR_D = 53;
const int TILE_FLOOR_E = 54;
const int TILE_FLOOR_F = 55;
const int TILE_FLOOR_G = 56;
const int TILE_FLOOR_H = 57;
const int TILE_FLOOR_I = 58;
const int TILE_FLOOR_J = 59;
const int TILE_FLOOR_K = 60;
const int TILE_FLOOR_L = 61;

static SDL_Rect GetTileRect(int code) {
	SDL_Rect rect;
	rect.h = 16;
	rect.w = 16;
	rect.x = 120;
	rect.y = 64;

	if (code == TILE_WALL) {
		//rect.x = 16 + 16 * std::rand() % 4;
		rect.x = 16;
		rect.y = 0;
	}
	else if (code == TILE_FLOOR) {
		rect.x = 96;
		rect.y = 0;
		//rect.x = 96 + 16 * std::rand() % 4;
		//rect.y = 0 + 16 * std::rand() % 3;
	}
	else if (code == TILE_DOOR_CLOSED_LR || code == TILE_DOOR_OPENED_TB) {
		// 7 5
		rect.x = 112;
		rect.y = 64;
	}
	else if (code == TILE_DOOR_CLOSED_TB || code == TILE_DOOR_OPENED_LR) {
		rect.x = 112;
		rect.y = 48;
	}
	else if (code == TILE_BIG_POTION) {
		rect.x = 144;
		rect.y = 128;
	}
	else if (code == TILE_SMALL_POTION) {
		rect.x = 128;
		rect.y = 144;
	}
	return rect;
}

static SDL_Rect GetFloorRect(int i, int j) {
	int code = (i * 7 + j * 11) % 12;
	SDL_Rect r;
	r.w = 16;
	r.h = 16;
	r.x = 16 * ( 6 + (code % 4));
	r.y = 16 * (code / 4);
	return r;
}

// how much XP one needs to levelup in current level
static int getLevelXPCap(int level) {
	return 15 * level; // TODO
}

// how much life one has in said level
static int getLevelLife(int level) {
	return 10 * level;
}

// how much attack one has in said level
static int getLevelAttack(int level) {
	return 2 * level;
}

// how much xp does enemy give upon kill with such level
static int getLevelXPGain(int level) {
	return 4 * level;
}