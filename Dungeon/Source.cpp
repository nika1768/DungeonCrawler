#include "libs.h"
#include "Generator.h"
#include "ResourceManager.h"
#include "Tilemap.h"
#include "NPC.h"
#include "HeroInfo.h"

using namespace std;

SDL_Window* win = nullptr;
SDL_Renderer* ren = nullptr;
SDL_Texture* tex = nullptr;
SDL_Rect pos;
SDL_Event e;

int main() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL init error " << endl;
		return -1;
	}
	if (TTF_Init() != 0) {
		cout << "TTF init error " << SDL_GetError() << endl;
		SDL_Quit();
		return -1;
	}
	if ((IMG_Init(IMG_INIT_PNG)) & (IMG_INIT_PNG != IMG_INIT_PNG)) {
		cout << "IMG init errror " << SDL_GetError() << endl;
		SDL_Quit();
		TTF_Quit();
		return -1;
	}
	win = SDL_CreateWindow("MainWindow", 100, 100, ScreenWidth, ScreenHeight, SDL_WindowFlags::SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		cout << "Window error " << SDL_GetError() << endl;
		SDL_Quit();
		return -1;
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);
	if (ren == nullptr) {
		cout << "Renderer error " << SDL_GetError() << endl;
		SDL_Quit();
		SDL_DestroyWindow(win);
		return -1;
	}

	ResourceManager::Init(ren);
	ResourceManager::tilemap = std::make_unique<Tilemap>(20, 20, 10, 10, 5, 3, 30);
	Tilemap* tilemap = ResourceManager::GetTilemap();
	tilemap->LoadTextures();

	Hero* hero = ResourceManager::GetHero();
	hero->setPosition(5, 5);
	tilemap->SetHero(hero);
	HeroInfo heroInfo(hero);

	// game loop
	bool quit = false;
	while (!quit) {
		// get input
		while (SDL_PollEvent(&e)) {
			SDL_Point p;
			p.x = 0;
			p.y = 0;
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.key.keysym.sym == SDLK_ESCAPE) {
				// show menu
			}
			else {
				tilemap->ResolveInput(e);
			}
		}

		// update

		// render screen

		SDL_RenderClear(ren);
		tilemap->OnRender();
		heroInfo.OnUpdate();
		heroInfo.OnRender();

		SDL_RenderPresent(ren);
	}

	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();


	return 0;
}