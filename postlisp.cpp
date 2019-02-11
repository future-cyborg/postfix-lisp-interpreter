#include <string>
#include <iostream>
#include <unordered_map>
#include <cstdarg>
#include <vector>
#include "Part.hpp"
#include "exceptions.hpp"
#include "parse.hpp"
#include "functions.hpp"

using std::string;
using std::unordered_map;
using std::vector;

void run(std::string input) {
	vector<Part*> parts;
	try {
		parts = parse(input);
		for(Part* part : parts) {
			if(part != nullptr) {
				Part* value = part->evaluate();
				std::cout << value->getVal() << std::endl;
				delete value;
			}
		}

	} catch (Exception e) {
		std::cout << "Error running input\n";
		std::cout << e.what() << std::endl;
	}

	// cleanup
	for(Part* part : parts) {
		delete part;
	}
}



int main() {
	init();
	std::string str = "(2 2 +)";
	run(str);

	return 0;
}