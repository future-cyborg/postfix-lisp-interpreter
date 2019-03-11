#include "setup.hpp"


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
	cmap->addPrimative("cond", &condition);

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
			command->getVal().compare("lambda") == 0 ||
			command->getVal().compare("cond") == 0) {
			skip = true;
		}
	}
	
	// Check if atom, check if 'cond'
	// call cond function

	// replace part with condEvaluate() results
	// 	similar to oldList, newPart below
	// 

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

		// memory leak?
		std::vector<Part*> args(parts.begin() + 1, parts.end());
		result = command->call(args);

	} else if(command->getType().compare("Atom") == 0) {

		std::vector<Part*> args(parts.begin() + 1, parts.end());
		result = cmap.callFunction(command, args);

	} else {
		throw Exception("Hmm, how'd you get here. Error in listEvaluate()");
	}

	while(result != nullptr && result->getType().compare("List") == 0) {
		List* l = dynamic_cast<List*>(result);
		if(l->shouldEvaluate()) {
			Part* newResult = l->evaluate();
			delete result;
			result = newResult;
		} else {
			break;
		}
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
