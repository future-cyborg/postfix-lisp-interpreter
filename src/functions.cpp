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


Part* add(vector<Part*> numbers) {
	if(numbers.size() == 0) throw MissingParamException();

	num_t sum = 0;
	for(Part* part : numbers) {
		if(part->getType().compare("Number") != 0) {
			throw NotANumber();
		}
		sum += std::stol(part->getVal());
	}
	return new Number(std::to_string(sum));
}

Part* subtract(vector<Part*> numbers) {
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
	return new Number(std::to_string(diff));
}

Part* multiply(vector<Part*> numbers) {
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
	return new Number(std::to_string(product));
}

Part* divide(vector<Part*> numbers) {
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
	return new Number(std::to_string(factor));
}

Part* equal(vector<Part*> parts) {
	if((int)parts.size() < 2) throw MissingParamException();

	for(int n = 1; n < (int)parts.size(); ++n) {
		if(parts.at(n)->getType().compare(parts.at(0)->getType()) != 0) {
			return new Atom("Flase");
		}
		if(parts.at(n)->getVal().compare(parts.at(0)->getVal()) != 0) {
			return new Atom("False");
		}
	}
	return new Atom("True");
}

Part* isAtom(vector<Part*> parts) {
	if((int)parts.size() == 0) throw MissingParamException();

	for(int n = 0; n < (int)parts.size(); ++n) {
		if(parts.at(n)->getType().compare("Atom") != 0) {
			return new Atom("False");
		}
	}
	return new Atom("True");
}

Part* quote(vector<Part*> parts) {
	if((int)parts.size() == 0) throw MissingParamException();

	for(Part* part : parts) {
		std::cout << part->getVal() << ' ';
	}
	std::cout << std::endl;
	return nullptr;
}

Part* cons(vector<Part*> parts) {
	if((int)parts.size() != 2) throw Exception("cons takes exactly 2 arguments");

	std::string newStr = parts[0]->getVal();
	newStr.append(" ");
	newStr.append(parts[1]->getVal());

	return new List(newStr);
}

Part* car(vector<Part*> parts) {
	if((int)parts.size() != 1) throw Exception("car takes exactly 1 argument");
	if(parts[0]->getType().compare("List") != 0)
		throw Exception("Function car only works on a List");

	std::string newStr = parseCar(parts[0]->getVal());

	if(newStr.size() == 0) throw Exception("car error");
	if(newStr[0] == '(') return new List(newStr);

	if(isNumber(newStr)) {
		return new Number(newStr);
	} else {
		return new Atom(newStr);
	}
}

Part* cdr(vector<Part*> parts) {
	if((int)parts.size() != 1) throw Exception("cdr takes exactly 1 argument");
	if(parts[0]->getType().compare("List") != 0)
		throw Exception("Function cdr only works on a List");

	return new List(parseCdr(parts[0]->getVal()));
}


// Function moved to CommandMap class
// Part* define(vector<Part*> parts) { }

Part* lambda(vector<Part*> parts) {
	if((int)parts.size() != 2) throw Exception("lambda takes exactly 2 arguments");
	if(parts[0]->getType().compare("List") != 0   ||
			parts[1]->getType().compare("List") != 0) {
		throw Exception("lambda arguments must be lists");
	}

	return new Lambda(parts[0], parts[1]);
}
