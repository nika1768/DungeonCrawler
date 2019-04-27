#include "Generator.h"
#include <iostream>
#include <string>

int main() {
	auto mapa = Generator::getMap(50,50);
	Generator::createRoom(mapa, 10, 10, 5, 5);
	while (Generator::findRoom(mapa, 5, 10)) {
		Generator::printMap(mapa);
	}

	return 0;
}