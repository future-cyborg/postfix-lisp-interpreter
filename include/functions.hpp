#include <string>
#include <iostream>
#include <unordered_map>
#include <cstdarg>
#include <vector>
#include "Part.hpp"
#include "CommandMap.hpp"
#include "exceptions.hpp"
#include "parse.hpp"

using std::string;
using std::unordered_map;
using std::vector;

/*
 * 	Math fsymbols currently only support number types. No function overloade=ing.
 */

Part* add(vector<Part*> numbers);

Part* subtract(vector<Part*> numbers);

Part* multiply(vector<Part*> numbers);

Part* divide(vector<Part*> numbers);



Part* equal(vector<Part*> parts);

Part* isAtom(vector<Part*> parts);

Part* quote(vector<Part*> parts);

Part* cons(vector<Part*> parts);

Part* car(vector<Part*> parts);

Part* cdr(vector<Part*> parts);

Part* define(vector<Part*> parts);

Part* lambda(vector<Part*> parts);

Part* condition(vector<Part*> parts);
