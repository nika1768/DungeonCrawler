#include "Generator.h"
#include <iostream>
#include <string>
#include <map>

class Foo {
public:
	Foo(int value) { val = value; }
	int val;
};

int main() {
	std::map<int, Foo*> map;
	map[1] = new Foo(1);
	map[3] = new Foo(3);
	map.erase(3);
	for (auto pair : map) {
		std::cout << pair.first << pair.second->val << std::endl;
	}
	std::cout << map[3] << std::endl;
	std::cin;
	return 0;
}