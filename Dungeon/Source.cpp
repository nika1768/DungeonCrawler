#include "libs.h"
#include "Generator.h"
#include "ResourceManager.h"
#include "Tilemap.h"
#include "NPC.h"
#include "MainMenu.h"
#include "HeroInfo.h"

SDL_Window* win = nullptr;
SDL_Renderer* ren = nullptr;
SDL_Texture* tex = nullptr;
SDL_Rect pos;
SDL_Event e;
std::string message = "     Welcome!     ";

// bool indicates if newgame has been clicked
bool MainMenu() {

	Menu menu(message);
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_USEREVENT && e.user.code == USER_NEW_GAME) {
				return true;
			}
			menu.ResolveInput(e);
		}

		SDL_RenderClear(ren);
		SDL_RenderCopy(ResourceManager::ren, ResourceManager::LoadTexture("Graphics/bg.png"), nullptr, nullptr);
		menu.OnRender();
		SDL_RenderPresent(ren);
	}
	return false;
}

// returns false if hero died, true if going to next level
bool NewGame() {

	ResourceManager::tilemap = std::make_unique<Tilemap>(20, 20, 10, 10, 5, 3, 30);
	Tilemap* tilemap = ResourceManager::GetTilemap();
	tilemap->Populate();
	tilemap->LoadTextures();

	Hero* hero = ResourceManager::GetHero();
	hero->setPosition(5, 5);
	HeroInfo heroInfo(hero);
	tilemap->ClearFog();
	tilemap->Centralize();

	// game loop
	bool quit = false;
	bool next_level = false;
	while (!quit) {
		// get input
		while (SDL_PollEvent(&e)) {
			SDL_Point p;
			p.x = 0;
			p.y = 0;
			if (e.type == SDL_QUIT) {
				quit = true;
				message = "Your total xp this game was: " + std::to_string(ResourceManager::GetHero()->getXP());
			}
			else if (e.type == SDL_USEREVENT && e.user.code == USER_NEXT_LEVEL) {
				quit = true;
				next_level = true;
			}
			else {
				tilemap->ResolveInput(e);
			}
		}

		// render screen
		SDL_RenderClear(ren);
		tilemap->OnRender();
		heroInfo.OnUpdate();
		heroInfo.OnRender();

		SDL_RenderPresent(ren);
	}

	ResourceManager::DestroyTilemap();
	if (!next_level) {
		ResourceManager::DestroyHero();
		return false;
	}

	ResourceManager::GetHero()->deleteItems();
	return true;
}

int main() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL init error " << std::endl;
		return -1;
	}
	if (TTF_Init() != 0) {
		std::cout << "TTF init error " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}
	if ((IMG_Init(IMG_INIT_PNG)) & (IMG_INIT_PNG != IMG_INIT_PNG)) {
		std::cout << "IMG init errror " << SDL_GetError() << std::endl;
		SDL_Quit();
		TTF_Quit();
		return -1;
	}
	win = SDL_CreateWindow("MainWindow", 100, 100, ScreenWidth, ScreenHeight, SDL_WindowFlags::SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		std::cout << "Window error " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);
	if (ren == nullptr) {
		std::cout << "Renderer error " << SDL_GetError() << std::endl;
		SDL_Quit();
		SDL_DestroyWindow(win);
		return -1;
	}

	ResourceManager::Init(ren);
	
	while (MainMenu()) {
		while(NewGame());
	}

	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();

	return 0;
}