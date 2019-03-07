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


Part_pt listEvaluate(std::string str) {
	return listEvaluate(str, *commandmap);
}

// Must free memory
Part_pt listEvaluate(std::string str, CommandMap &cmap) {
	std::vector<Part_pt> parts = parse(str);

	Part_pt command;
	Part_pt result = nullptr;
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
				Part_pt newPart = parts[i]->evaluate();

				parts[i] = newPart;

			}

			// if it's saved as a variable, expand it
			if(parts[i]->getType().compare("Atom") == 0) {
				Part_pt newPart = cmap.getValue(parts[i]);

				if(newPart) {
					parts[i] = newPart;
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

		std::vector<Part_pt> args(parts.begin() + 1, parts.end());
		result = command->call(args);

	} else if(command->getType().compare("Atom") == 0) {

		std::vector<Part_pt> args(parts.begin() + 1, parts.end());
		result = cmap.callFunction(command, args);

	} else {
		throw Exception("Hmm, how'd you get here. Error in listEvaluate()");
	}

	while(result != nullptr && result->getType().compare("List") == 0) {
		List_pt l = std::dynamic_pointer_cast<List>(result);
		if(l->shouldEvaluate()) {
			result = l->evaluate();
		} else {
			break;
		}
	}

	return result;
}

void run(std::string &input) {
	vector<Part_pt> parts;
	try {
		parts = parse(input);
		for(Part_pt part : parts) {
			if(part != nullptr) {
				Part_pt value = part->evaluate();
				if(value != nullptr)
					std::cout << value->getVal() << std::endl;
			}
		}

	} catch (Exception e) {
		std::cout << "Error running input: ";
		std::cout << e.what() << std::endl;
	}
}
