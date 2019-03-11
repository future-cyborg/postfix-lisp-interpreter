#ifndef COMMANDMAP_H
#define COMMANDMAP_H

#include "Part.hpp"
#include <vector>
#include <unordered_map>
#include <string>


using std::vector;
using std::unordered_map;
using std::string;



typedef Part* (*sexps)(vector<Part*>);
typedef unordered_map<string, sexps> primative_map;
typedef unordered_map<string, Part*> command_map;

/**
 * 	CommandMap contains and manages all symbols that can be resolved.
 * 
 * 	Primitive functions are defined programmatically before compilation
 * 		and stored in primative_map.
 * 	User defined symbols and functions are stored as Parts in command_map.
 *
 */

class CommandMap {
public:
	CommandMap();

	/* 
	 * 	addPrimative() should be called only during initialization to
	 * 		add programmatically defined functions.
	 */
	void addPrimative(string command, sexps function);

	/*
	 * 	getValue() returns the value of a user-defined symbol 
	 */
	Part* getValue(Part* symbol) const;

	/*
	 * 	callFunction() is used both to define symbols and execute functions.
	 * 
	 * 	If being used to define a symbol, command must be an atom with a
	 * 		value "define".
	 *
	 * 	Command must either be equal to a primitive function or resolve to
	 * 		a lambda. User-defined symbols that resolve to non-lambdas 
	 *		will throw an Exception.
	 */
	Part* callFunction(Part* command, vector<Part*> args);


private:
	command_map commands;
	primative_map primatives;

	Part* define(vector<Part*> parts);

};


#endif
