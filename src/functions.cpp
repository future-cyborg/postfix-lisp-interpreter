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

CommandMap* init() {
	CommandMap* cmap = new CommandMap();

	cmap->addPrimative("+", &add);
	cmap->addPrimative("-", &subtract);
	cmap->addPrimative("/", &divide);
	cmap->addPrimative("*", &multiply);

	cmap->addPrimative("eq?", &equal);
	cmap->addPrimative("atom?", &isAtom);
	cmap->addPrimative("quote", &quote);
	
	cmap->addPrimative("cons", &cons);
	cmap->addPrimative("car", &car);
	cmap->addPrimative("cdr", &cdr);

	// cmap->addPrimative("define", &define);
	cmap->addPrimative("lambda", &lambda);

	return cmap;
}

CommandMap *commandmap = init();


Part* listEvaluate(std::string str) {
	return listEvaluate(str, *commandmap);
}

// Must free memory
Part* listEvaluate(std::string str, CommandMap &cmap) {
	std::vector<Part*> parts = parse(str);

	Part* command;
	Part* result = nullptr;
	// if first arg is a list
	if(parts[0]->getType().compare("List") == 0) {
		command = parts[0]->evaluate();
	} else {
		command = parts[0];
	}


	// if not quote or lambda, evaluate all subLists
	bool skip = false;

	// if quoting or lambda, we don't want to evaluate
	if(command->getType().compare("Atom") == 0) {
		if(command->getVal().compare("quote") == 0 ||
			command->getVal().compare("lambda") == 0) {
			skip = true;
		}
	}
	
	if(!skip) {
		for(int i = 1; i < (int)parts.size(); i++) {
			// if it's a list, evaluate it
			if(parts[i]->getType().compare("List") == 0) {
				// get pointer to 
				Part* oldList = parts[i];
				Part* newPart = oldList->evaluate();
				delete oldList;

				parts[i] = newPart;

			}

			// if it's saved as a variable, expand it
			if(parts[i]->getType().compare("Atom") == 0) {
				Part* newPart = cmap.getValue(parts[i]);

				if(newPart) {
					delete parts[i];
					parts[i] = newPart->copy();
				}
			}
		}
	}


	//check type of command
	if(command->getType().compare("List") == 0) {
		throw Exception("List is not callable");
	} else if(command->getType().compare("Number") == 0) {
		throw Exception("Number is not callable");
	} else if(command->getType().compare("Lambda") == 0) {

		std::vector<Part*> args(parts.begin() + 1, parts.end());
		result = command->call(args);

	} else if(command->getType().compare("Atom") == 0) {

		std::vector<Part*> args(parts.begin() + 1, parts.end());
		result = cmap.callFunction(command, args);

	} else {
		throw Exception("Hmm, how'd you get here. Error in listEvaluate()");
	}


	// Free memory
	if(parts[0]->getType().compare("List") == 0) {
		delete command;
	}
	for(Part* part : parts) {
		delete part;
	}

	return result;
}

void run(std::string &input) {
	vector<Part*> parts;
	try {
		parts = parse(input);
		for(Part* part : parts) {
			if(part != nullptr) {
				Part* value = part->evaluate();
				if(value != nullptr)
					std::cout << value->getVal() << std::endl;
				delete value;
			}
		}

	} catch (Exception e) {
		std::cout << "Error running input: ";
		std::cout << e.what() << std::endl;
	}

	// cleanup
	for(Part* part : parts) {
		delete part;
	}
}
