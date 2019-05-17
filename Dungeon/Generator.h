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
			while (findRoom(map, min, max));
		}
		else {
			for (int i = 0; i < room_count; i++)
				findRoom(map, min, max);
		}

		// remove double walls
		RemoveDoubleWalls(map);

		return map;

	}

	static Map getFullMap2(int x_rooms, int y_rooms, int x_room_size, int y_room_size, int room_count) {
		Map map = getMap(x_rooms * x_room_size + x_rooms + 1, y_rooms * y_room_size + y_rooms + 1);

		Map room_map = getMap(x_rooms, y_rooms);

		bool found;

		// fill initial rooms
		for (int r = 0; r < std::min(room_count,x_rooms*y_rooms); r++) {

			found = false;
			
			for(int trial = 0; trial < 10; trial++){

				int i = std::rand() % y_rooms;
				int j = std::rand() % x_rooms;

				if (room_map[i][j] == 0) {
					room_map[i][j] = r;
					found = true;
				}

				if (found)
					break;
			}

			if (!found) {
				for(int i = 0; i < y_rooms; i++){
					for (int j = 0; j < x_rooms; j++) {
						if (room_map[i][j] == 0) {
							room_map[i][j] = r;
							found = true;
							break;
						}
					}

					if (found)
						break;
				}
			}

		}

		// enlarge existing rooms to fill the map
		int actual_room_count = x_rooms * y_rooms;

		while (actual_room_count > room_count) {
			found = false;

			for(int trial = 0; trial < 5; trial++){
				int i = std::rand() % y_rooms;
				int j = std::rand() % x_rooms;

				if (room_map[i][j] > 0) {

					if (i > 0) {
						if (room_map[i - 1][j] == 0) {
							room_map[i - 1][j] = room_map[i][j];
							found = true;
							actual_room_count--;
						}
					}
					if (j > 0) {
						if (room_map[i][j - 1] == 0) {
							room_map[i][j - 1] = room_map[i][j];
							found = true;
							actual_room_count--;
						}
					}
					if (i < y_rooms-1) {
						if (room_map[i + 1][j] == 0) {
							room_map[i + 1][j] = room_map[i][j];
							found = true;
							actual_room_count--;
						}
					}
					if (j < x_rooms-1) {
						if (room_map[i][j + 1] == 0) {
							room_map[i][j + 1] = room_map[i][j];
							found = true;
							actual_room_count--;
						}
					}


				}

				if (found)
					break;

			}

			if (!found) {
				

				for (int i = 0; i < y_rooms; i++) {
					for (int j = 0; j < x_rooms; j++) {
						if (room_map[i][j] > 0) {

							if (i > 0) {
								if (room_map[i - 1][j] == 0) {
									room_map[i - 1][j] = room_map[i][j];
									found = true;
									actual_room_count--;
								}
							}
							if (j > 0) {
								if (room_map[i][j - 1] == 0) {
									room_map[i][j - 1] = room_map[i][j];
									found = true;
									actual_room_count--;
								}
							}
							if (i < y_rooms - 1) {
								if (room_map[i + 1][j] == 0) {
									room_map[i + 1][j] = room_map[i][j];
									found = true;
									actual_room_count--;
								}
							}
							if (j < x_rooms - 1) {
								if (room_map[i][j + 1] == 0) {
									room_map[i][j + 1] = room_map[i][j];
									found = true;
									actual_room_count--;
								}
							}
						}

						if (found)
							break;

					}

					if (found)
						break;

				}

			}

		}

		// fill map according to room_map

		// top-bottom walls
		for (int j = 0; j < x_rooms * x_room_size + x_rooms + 1; j++) {
			map[0][j] = 1;
			map[map.size() - 1][j] = 1;
		}

		// left-right walls
		for (int i = 0; i < y_rooms * y_room_size + y_rooms + 1; i++) {
			map[i][0] = 1;
			map[i][map[0].size() - 1] = 1;
		}

		// walls between rooms
		for (int i = 0; i < y_rooms; i++) {
			for (int j = 0; j < x_rooms; j++) {

				if (i > 0) {
					if (room_map[i - 1][j] != room_map[i][j]) {
						int door = std::rand() % x_room_size;
						for (int w = -1; w <= x_room_size; w++) {
							if (w == door)
								map[i * y_room_size + i][j * x_room_size + j + 1 + w] = 2;
							else
								map[i * y_room_size + i][j * x_room_size + j + 1 + w] = 1;
						}
					}
				}
				if (j > 0) {
					if (room_map[i][j - 1] != room_map[i][j]) {
						int door = std::rand() % y_room_size;
						for (int h = -1; h <= y_room_size; h++) {
							if(h == door)
								map[i * y_room_size + i + 1 + h][j * x_room_size + j] = 2;
							else
								map[i * y_room_size + i + 1 + h][j * x_room_size + j] = 1;
						}
					}
				}

			}
		}

		/*
		std::cout << "Room map: " << std::endl;
		printMap(room_map);
		std::cout << "Map:" << std::endl;
		printGraphicMap(map);
		*/
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

	static bool findRoom(Map& map, int min = 3, int max = 10) {
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
		createRoom(map, w - 1, h - 1, x, y);
		if (!end)
			return false;
		return true;


	}

	static void createRoom(Map& map, int w, int h, int x, int y) {

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

	static void AddDoors(Map& map, int room_count) {

		Map tempMap = getMap(map[0].size(), map.size());
		for (int i = 0; i < map.size(); i++)
			for (int j = 0; j < map[0].size(); j++)
				tempMap[i][j] = map[i][j];

		while (room_count > 0) {

			// find unexplored room
			int lti, ltj;
			bool found = false;
			for(int i = 0; i < map.size(); i++){
				for (int j = 0; j < map[0].size(); j++) {
					if (tempMap[i][j] == 2) {
						lti = i;
						ltj = j;
						found = true;
						break;
					}
				}
				if (found)
					break;
			}

			// unexplored room around map[ri][rj]


		}

	}

	static void printMap(Map& map) {
		for (auto& line : map) {
			for (auto& e : line) {
				std::cout << e << " ";
			}
			std::cout << std::endl;
		}
	}

	static void printGraphicMap(Map& map) {
		for (auto& line : map) {
			for (auto& e : line) {
				if (e == 0)
					std::cout << "  ";
				else if (e == 1)
					std::cout << "# ";
				else if (e == 2)
					std::cout << "D ";
			}
			std::cout << std::endl;
		}
	}

private:

};