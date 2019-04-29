#pragma once

#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

using Vec = std::vector<int>;
using Map = std::vector<Vec>;

using VecB = std::vector<bool>;
using MapB = std::vector<VecB>;

class Generator {
public:

	static Map getFullMap(int w, int h, int min, int max, int room_count = -1) {
		
		// get empty map
		Map map = getMap(w, h);

		// fill it with rooms
		if (room_count <= 0) {
			room_count = 0;
			while (findRoom(map, room_count + 2, min, max)) {
				room_count++;
			}
		}
		else {
			for (int i = 0; i < room_count; i++)
				findRoom(map, i + 2, min, max);
		}

		// remove double walls
		RemoveDoubleWalls(map);

		// add doors
		//AddDoors(map, room_count + 2, room_count + 5);

		// remove ints
		FixMap(map, room_count + 2);

		return map;

	}

	static MapB getMapB(int w, int h) {
		MapB map;
		for (int i = 0; i < h; ++i) {
			map.push_back(VecB());
			for (int j = 0; j < w; ++j) {
				map[i].push_back(false);
			}
		}
		return map;
	}

	static Map getMap(int w, int h) {
		Map map;
		for (int i = 0; i < h; ++i) {
			map.push_back(Vec());
			for (int j = 0; j < w; ++j) {
				map[i].push_back(0);
			}
		}
		return map;
	}

	static bool findRoom(Map& map, int fill, int min = 3, int max = 10) {
		if (map.size() < min && map[0].size() < min) {
			return false;
		}

		Map tempMap = getMap(map[0].size(), map.size());

		for (int i = 0; i < map.size(); ++i) {
			tempMap[i][0] = 1;
			for (int j = 1; j < map[0].size(); ++j) {
				if (map[i][j] == 0 || map[i][j] == 1) {
					tempMap[i][j] = tempMap[i][j - 1] + 1;
				}
			}
		}

		Vec js;
		for (int i = 0; i < map[0].size(); i++) js.push_back(i);

		int x = 0, y = 0, w = 0, h = 0;
		bool end = false;
		while(!js.empty()) {

			int index = std::rand() % js.size();
			int j = js[index];
			js.erase(js.begin() + index);

			int sum = 0;
			for (int s = 0; s < map.size() - 1; ++s) {
				for (int t = s + 1; t < map.size(); ++t) {

					int sirkast = 1000;

					for (int u = s; u <= t; ++u) {
						sirkast = std::min(sirkast, tempMap[u][j]);
					}

					if (sirkast >= min && sirkast <= max && (t - s + 1) >= min && (t - s + 1) <= max && std::rand() % 5 == 1) {
						w = sirkast;
						h = t - s + 1;
						x = j - sirkast + 1;
						y = s;
						end = true;
					}
					if (end) break;
				}
				if (end) break;
			}
			if (end) break;
		}

		//printMap(tempMap);
		std::cout << x << " " << y << " " << w << " " << h << std::endl;
		createRoom(map, fill, w - 1, h - 1, x, y);
		if (!end)
			return false;
		return true;


	}

	static void createRoom(Map& map, int fill, int w, int h, int x, int y) {

		for (int i = 0; i <= h; ++i) {
			map[i + y][x] = 1;
			map[i + y][x + w] = 1;
		}

		for (int i = 0; i < w; ++i) {
			map[y][i + x] = 1;
			map[y + h][i + x] = 1;
		}

		for (int i = 1; i < h; ++i) {
			for (int j = 1; j < w; ++j) {
				map[i + y][j + x] = 2;
			}
		}
	}

	static void RemoveDoubleWalls(Map& map) {
		
		for (int i = 0; i < map.size()-3; ++i) {
			for (int j = 0; j < map[0].size(); ++j) {
				if (map[i][j] != 1 && map[i + 1][j] == 1 && map[i + 2][j] == 1 && map[i + 3][j] != 1)
					map[i + 1][j] = map[i][j];
			}
		}
		for (int i = 0; i < map.size(); ++i) {
			for (int j = 0; j < map[0].size() - 3; ++j) {
				if (map[i][j] != 1 && map[i][j + 1] == 1 && map[i][j + 2] == 1 && map[i][j + 3] != 1)
					map[i][j + 1] = map[i][j];
			}
		}
	}

	static void FixMap(Map& map, int max_int) {

		for(int i = 0; i < map.size(); i++)
			for (int j = 0; j < map[0].size(); j++) {

				if (map[i][j] > 1 && map[i][j] <= max_int)
					map[i][j] = 2;

				if (map[i][j] == max_int + 1)
					map[i][j] = 3;
			}

	}

	static void AddDoors(Map& map, int max_int, int door_count) {

		while (door_count > 0) {

			int x = std::rand() % map.size();
			int y = std::rand() % map[0].size();

			if (map[x][y] == 1 && x > 0 && y > 0 && x < map.size()-1 && y < map[0].size()-1) {
				if (map[x - 1][y] > 1 && map[x + 1][y] > 1 && map[x][y - 1] == 1 && map[x][y + 1] == 1) {
					map[x][y] = max_int + 1;
					door_count--;
				}
				else if (map[x - 1][y] == 1 && map[x + 1][y] == 1 && map[x][y - 1] > 1 && map[x][y + 1] > 1) {
					map[x][y] = max_int + 1;
					door_count--;
				}
			}

		}

	}

	static void printMap(Map& map) {
		for (auto& line : map) {
			for (auto& e : line) {
				if (e == 0)
					std::cout << "  ";
				else if (e == 1)
					std::cout << "# ";
				else if (e == 2)
					std::cout << "_ ";
			}
			std::cout << std::endl;
		}
	}

private:

};