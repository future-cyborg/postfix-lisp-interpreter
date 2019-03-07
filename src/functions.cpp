#include <string>
#include <iostream>
#include <unordered_map>
#include <cstdarg>
#include <vector>
#include "Part.hpp"
#include "CommandMap.hpp"
#include "exceptions.hpp"
#include "parse.hpp"
#include "functions.hpp"


using std::string;
using std::unordered_map;
using std::vector;

typedef long num_t;


Part_pt add(vector<Part_pt> numbers) {
	if(numbers.size() == 0) throw MissingParamException();

	num_t sum = 0;
	for(Part_pt part : numbers) {
		if(part->getType().compare("Number") != 0) {
			throw NotANumber();
		}
		sum += std::stol(part->getVal());
	}
	return std::make_shared<Number>(std::to_string(sum));
}

Part_pt subtract(vector<Part_pt> numbers) {
	if(numbers.size() == 0) throw MissingParamException();

	if(numbers[0]->getType().compare("Number") != 0)
		throw NotANumber();
	num_t diff = std::stol(numbers[0]->getVal());
	for(int i = 1; i < (int)numbers.size(); i++) {
		if(numbers[i]->getType().compare("Number") != 0) {
			throw NotANumber();
		}
		diff -= std::stol(numbers[i]->getVal());
	}
	return std::make_shared<Number>(std::to_string(diff));
}

Part_pt multiply(vector<Part_pt> numbers) {
	if(numbers.size() == 0) throw MissingParamException();

	if(numbers[0]->getType().compare("Number") != 0)
		throw NotANumber();
	num_t product = std::stol(numbers[0]->getVal());
	for(int i = 1; i < (int)numbers.size(); i++) {
		if(numbers[i]->getType().compare("Number") != 0) {
			throw NotANumber();
		}
		product *= std::stol(numbers[i]->getVal());
	}
	return std::make_shared<Number>(std::to_string(product));
}

Part_pt divide(vector<Part_pt> numbers) {
	if(numbers.size() == 0) throw MissingParamException();

	if(numbers[0]->getType().compare("Number") != 0)
		throw NotANumber();
	num_t factor = std::stol(numbers[0]->getVal());
	for(int i = 1; i < (int)numbers.size(); i++) {
		if(numbers[i]->getType().compare("Number") != 0) {
			throw NotANumber();
		}
		factor /= std::stol(numbers[i]->getVal());
	}
	return std::make_shared<Number>(std::to_string(factor));
}

Part_pt equal(vector<Part_pt> parts) {
	if((int)parts.size() < 2) throw MissingParamException();

	for(int n = 1; n < (int)parts.size(); ++n) {
		if(parts.at(n)->getType().compare(parts.at(0)->getType()) != 0) {
			return std::make_shared<Atom>("Flase");
		}
		if(parts.at(n)->getVal().compare(parts.at(0)->getVal()) != 0) {
			return std::make_shared<Atom>("False");
		}
	}
	return std::make_shared<Atom>("True");
}

Part_pt isAtom(vector<Part_pt> parts) {
	if((int)parts.size() == 0) throw MissingParamException();

	for(int n = 0; n < (int)parts.size(); ++n) {
		if(parts.at(n)->getType().compare("Atom") != 0) {
			return std::make_shared<Atom>("False");
		}
	}
	return std::make_shared<Atom>("True");
}

Part_pt quote(vector<Part_pt> parts) {
	if((int)parts.size() == 0) throw MissingParamException();

	for(Part_pt part : parts) {
		std::cout << part->getVal() << ' ';
	}
	std::cout << std::endl;
	return nullptr;
}

Part_pt cons(vector<Part_pt> parts) {
	if((int)parts.size() != 2) throw Exception("cons takes exactly 2 arguments");

	std::string newStr = parts[0]->getVal();
	newStr.append(" ");
	newStr.append(parts[1]->getVal());

	return std::make_shared<List>(newStr);
}

Part_pt car(vector<Part_pt> parts) {
	if((int)parts.size() != 1) throw Exception("car takes exactly 1 argument");
	if(parts[0]->getType().compare("List") != 0)
		throw Exception("Function car only works on a List");

	std::string newStr = parseCar(parts[0]->getVal());

	if(newStr.size() == 0) throw Exception("car error");
	if(newStr[0] == '(') return std::make_shared<List>(newStr);

	if(isNumber(newStr)) {
		return std::make_shared<Number>(newStr);
	} else {
		return std::make_shared<Atom>(newStr);
	}
}

Part_pt cdr(vector<Part_pt> parts) {
	if((int)parts.size() != 1) throw Exception("cdr takes exactly 1 argument");
	if(parts[0]->getType().compare("List") != 0)
		throw Exception("Function cdr only works on a List");

	return std::make_shared<List>(parseCdr(parts[0]->getVal()));
}


// Function moved to CommandMap class
// Part_pt define(vector<Part_pt> parts) { }

Part_pt lambda(vector<Part_pt> parts) {
	if((int)parts.size() != 2) throw Exception("lambda takes exactly 2 arguments");
	if(parts[0]->getType().compare("List") != 0   ||
			parts[1]->getType().compare("List") != 0) {
		throw Exception("lambda arguments must be lists");
	}

	return std::make_shared<Lambda>(parts[0], parts[1]);
}


Part_pt condition(vector<Part_pt> parts) {
	// Check if each part is a list
	Part_pt result = nullptr;
	for(int i = 0; i < (int)parts.size() - 1; i++) {
		if(parts[i]->getType().compare("List") != 0) {
			throw Exception("Condition cases must be lists");
		}


		std::vector<Part_pt> subParts = parse(parts[i]->getVal());
		if(subParts.size() != 2) {
			throw Exception("Condition cases must only have 2 parts");
		}
		if(subParts[0]->getType().compare("List") != 0) {
			throw Exception("First arguement of condition cases must be a list");
		}

		Part_pt boolean = subParts[0]->evaluate();
		if(boolean->getType().compare("Atom") == 0
				&& boolean->getVal().compare("True") == 0) {
			result = subParts[1];
			break;
		}

	}
	if(result == nullptr) {
		std::vector<Part_pt> subParts = parse(parts[parts.size() - 1]->getVal());
		result = subParts[1];
	}
	return result;
}
