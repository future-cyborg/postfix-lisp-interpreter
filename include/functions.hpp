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

Part_pt add(vector<Part_pt> numbers);

Part_pt subtract(vector<Part_pt> numbers);

Part_pt multiply(vector<Part_pt> numbers);

Part_pt divide(vector<Part_pt> numbers);



Part_pt equal(vector<Part_pt> parts);

Part_pt isAtom(vector<Part_pt> parts);

Part_pt quote(vector<Part_pt> parts);

Part_pt cons(vector<Part_pt> parts);

Part_pt car(vector<Part_pt> parts);

Part_pt cdr(vector<Part_pt> parts);

Part_pt define(vector<Part_pt> parts);

Part_pt lambda(vector<Part_pt> parts);

Part_pt condition(vector<Part_pt> parts);
