#include "CommandMap.hpp"
#include "exceptions.hpp"

CommandMap::CommandMap() {}

void CommandMap::addPrimative(string command, sexps function) {
	if(primatives.find(command) != primatives.end()) {
		throw Exception("Cannot overwrite primative function");
	}
	primatives.insert(std::make_pair(command, function));

}


Part* CommandMap::getValue(Part* symbol) const {
	if(commands.find(symbol->getVal()) != commands.end()) {
		return ( commands.at(symbol->getVal())  );
	}

	return nullptr;
}


Part* CommandMap::callFunction(Part* command, vector<Part*> args) {
	std::string symbol = command->getVal();

	if(symbol.compare("define") == 0) {

		return define(args);
	// if exists as a primative
	} else if(primatives.find(symbol) != primatives.end()) {

		return (primatives[symbol])(args);
	// if not primative, might be a command
	} else if(commands.find(symbol) != commands.end()) {

		return commands[symbol]->call(args);
	// if not a command or primative
	} else {
		std::cout << symbol << '\n';
		throw Exception(symbol.c_str());
	}
}



Part* CommandMap::define(vector<Part*> parts) {
	if((int)parts.size() != 2) throw Exception("define takes exactly 2 arguments");
	if(parts[0]->getType().compare("Atom") != 0) {
		throw Exception("define first argument must be an atom");
	}
	if(parts[1]->getType().compare("Atom") == 0) {
		throw Exception("define second argument cannot be an atom");
	}

	Part* symbol = parts[0];
	Part* value = parts[1];
	
	// define is now a special 'primative'
	if(symbol->getVal().compare("define") == 0) {
		throw Exception("Cannot overwrite primative function");
	}
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
