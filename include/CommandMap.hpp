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



class CommandMap {
public:
	CommandMap();

	void addPrimative(string command, sexps function);

	Part* getValue(Part* symbol) const;
	Part* callFunction(Part* command, vector<Part*> args);


private:
	command_map commands;
	primative_map primatives;

	Part* define(vector<Part*> parts);

};


#endif
