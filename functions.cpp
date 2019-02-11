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

typedef long num_t;
typedef Part* (*sexps)(vector<Part*>);
typedef unordered_map<string, sexps> primative_map;
typedef unordered_map<string, Part*> command_map;

command_map commands;
primative_map primatives;

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

	num_t sum = 0;
	for(Part* part : numbers) {
		if(part->getType().compare("Number") != 0) {
			throw NotANumber();
		}
		sum -= std::stol(part->getVal());
	}
	return new Number(std::to_string(sum));
}

Part* multiply(vector<Part*> numbers) {
	if(numbers.size() == 0) throw MissingParamException();

	num_t sum = 0;
	for(Part* part : numbers) {
		if(part->getType().compare("Number") != 0) {
			throw NotANumber();
		}
		sum *= std::stol(part->getVal());
	}
	return new Number(std::to_string(sum));
}

Part* divide(vector<Part*> numbers) {
	if(numbers.size() == 0) throw MissingParamException();

	num_t sum = 0;
	for(Part* part : numbers) {
		if(part->getType().compare("Number") != 0) {
			throw NotANumber();
		}
		sum /= std::stol(part->getVal());
	}
	return new Number(std::to_string(sum));
}

Part* equal(vector<Part*> parts) {
	if((int)parts.size() < 2) throw MissingParamException();

	for(int n = 1; n < (int)parts.size(); ++n) {
		if(parts.at(n)->getType().compare(parts.at(0)->getType()) != 0) {
			return new Atom("Flase");
		}
		if(parts.at(n) != parts.at(0)) {
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
		std::cout << part->getVal();
	}
	std::cout << std::flush;
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

Part* define(vector<Part*> parts) {
	if((int)parts.size() != 2) throw Exception("define takes exactly 2 arguments");
	if(parts[0]->getType().compare("Atom") != 0) {
		throw Exception("define first argument must be an atom");
	}
	if(parts[1]->getType().compare("Atom") == 0) {
		throw Exception("define second argument cannot be an atom");
	}

	Part* symbol = parts[0];
	Part* value = parts[1];
	// if symbol is a primative
	if(primatives.find(symbol->getVal()) != primatives.end()) {
		throw Exception("Cannot overwrite primative function");
	}

	// if overwriting symbol, free memory
	if(commands.find(symbol->getVal()) != commands.end()) {
		delete commands[symbol->getVal()];
		commands[symbol->getVal()] = value->copy();
	} else {
		commands.insert(std::make_pair(symbol->getVal(), value->copy()));
	}

	return nullptr;
}

Part* lambda(vector<Part*> parts) {
	if((int)parts.size() != 2) throw Exception("lambda takes exactly 2 arguments");
	if(parts[0]->getType().compare("List") != 0   ||
			parts[1]->getType().compare("List") != 0) {
		throw Exception("lambda arguments must be lists");
	}

	return new Lambda(*parts[0], *parts[1]);
}

void init() {
	primatives.emplace("+", &add);
	primatives.emplace("-", &subtract);
	primatives.emplace("/", &divide);
	primatives.emplace("*", &multiply);

	primatives.emplace("eq?", &equal);
	primatives.emplace("atom?", &isAtom);
	primatives.emplace("quote", &quote);
	
	primatives.emplace("cons", &cons);
	primatives.emplace("car", &car);
	primatives.emplace("cdr", &cdr);

	primatives.emplace("define", &define);
	primatives.emplace("lambda", &lambda);	
}


Part* callFunction(Part* command, std::vector<Part*> args) {
	std::string symbol = command->getVal();
	
	// if exists as a primative
	if(primatives.find(symbol) != primatives.end()) {
		return (primatives[symbol])(args);
	// if not primative, might be a command
	} else if(commands.find(symbol) != commands.end()) {

		return commands[symbol]->call(args);
	// if not a command or primative
	} else {
		throw Exception("Command not defined");
	}
}

// Must free memory
Part* listEvaluate(std::string str) {
	std::vector<Part*> parts = parse(str);

	Part* command;
	Part* result = nullptr;
	// if first arg is a list
	if(parts[0]->getType().compare("List") == 0) {
		command = parts[0]->evaluate();
	} else {
		command = parts[0];
	}

	//check type of command
	if(command->getType().compare("List") == 0) {
		throw Exception("List is not callable");
	} else if(command->getType().compare("Number") == 0) {
		throw Exception("Number is not callable");
	} else if(command->getType().compare("Lambda") == 0) {

		std::vector<Part*> args(parts.begin() + 1, parts.end());
		Part* lambdaList = command->call(args);
		result = lambdaList->evaluate();
		delete lambdaList;

	} else if(command->getType().compare("Atom") == 0) {

		std::vector<Part*> args(parts.begin() + 1, parts.end());
		result = callFunction(command, args);

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
