#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "exceptions.hpp"
#include "Part.hpp"
#include "parse.hpp"
#include "setup.hpp"


Part::~Part () {};

std::string Part::getVal() {
    return value;
}
std::string Part::getType() {
    return "Part";
}
Part_pt Part::call(std::vector<Part_pt> args) {
    throw CannotCall("Part cannot be called");
}
Part_pt Part::evaluate(){
    throw CannotEvaluate("Part cannot be evaluated");
}

// bool Part::isType(std::string str) {
//     size_t prev = 0, pos;
//     while( (pos = type.find_first_of(":", prev)) != std::string::npos) {
//         std::string t(type, prev, pos - prev);
//         if(t.compare(str) == 0) return true;
//         prev = pos + 1;
//     }
//     if(std::string(type, prev, pos - prev).compare(str) == 0) return true;

//     return false;
// }

List::List(std::string v) {
    if(v.back() == '~') {
        shouldEval = true;
        value.assign(v.begin(), v.end() - 1);
    } else {
        value = v;
    }
}
std::string List::getVal() {
    return value;
}
std::string List::getType() {
	return "List";
}
Part_pt List::call(std::vector<Part_pt> args) {
    throw CannotCall("List cannot be called");
}
Part_pt List::evaluate() {
    return listEvaluate(value);
}
bool List::shouldEvaluate() {
    return shouldEval;
}

Atom::Atom(std::string v) {
 	value = v;
 	if(value.size() == 0) throw NotAString();
}

std::string Atom::getVal() {
    return value;
}
std::string Atom::getType() {
    return "Atom";
}
Part_pt Atom::call(std::vector<Part_pt> args) {
    throw CannotCall("Atom cannot be called");
}
Part_pt Atom::evaluate() {
    throw CannotEvaluate("Atom cannot be evaluated");
}


bool operator== (const Atom &a1, const Atom &a2)
{
    return (a1.value.compare(a2.value) == 0);
}
 
bool operator!= (const Atom &a1, const Atom &a2)
{
    return !(a1.value.compare(a2.value) == 0);
}





Number::Number(std::string v) {
    value = v;
    if(value.size() == 0) throw NotAString();
}

std::string Number::getVal() {
    return value;
}
std::string Number::getType() {
    return "Number";
}
Part_pt Number::call(std::vector<Part_pt> args) {
    throw CannotCall("Number cannot be called");
}
Part_pt Number::evaluate() {
    throw CannotEvaluate("Number cannot be evaluated");
}


bool operator== (const Number &n1, const Number &n2)
{
    return (n1.value.compare(n2.value) == 0);
}
 
bool operator!= (const Number &n1, const Number &n2)
{
    return !(n1.value.compare(n2.value) == 0);
}




Lambda::Lambda(Part_pt args, Part_pt statement) : expression {statement} {
	std::vector<Part_pt> parts = parse(args->getVal());
	for(Part_pt part : parts) {
		if(part->getType().compare("Atom") != 0) {
			throw Exception("Lambda parameters can only contain Atoms");
		}
		++numArgs;
		argTemplates.push_back(part);
	}
}

Lambda::Lambda(const Lambda &lambda) :
    numArgs(lambda.numArgs),
    expression(lambda.expression) {
        // argTemplates(lambda.argTemplates),
        for(int argI = 0; argI < (int)lambda.argTemplates.size(); argI++) {
            argTemplates.push_back(Part_pt(lambda.argTemplates[argI]));
        }
    }

Lambda::~Lambda() {
}

Part_pt Lambda::call(std::vector<Part_pt> args) {
	std::string exp(expression->getVal());
	if((int)args.size() != numArgs) {
		throw InvalidLambdaExpression("Wrong number of arguments to lambda");
	}

	// find all occurances of argTemplates and replace with args
	// but only replace if occurance of argTemplate is a whole symbol
	for(int arg = 0; arg < (int)args.size(); arg++) {
		size_t index = 0;
		std::string from = argTemplates.at(arg)->getVal();
		while (true) {
		    index = exp.find(from, index);
			if (index == std::string::npos) break;
			
			size_t end = exp.find_first_of(whiteParen, index);
			if(index + from.size() == end) {
				exp.replace(index, from.size(), args.at(arg)->getVal());
				index += from.size();
			} else {
				index = end;
			}
			
		}
	}
    List* li = new List(exp);
    Part_pt result = li->evaluate();
    delete li;
    return result;
}

std::string Lambda::getVal() {
    throw Exception("Lambda has no value");
}
std::string Lambda::getType() {
    return "Lambda";
}

Part_pt Lambda::evaluate(){
    throw CannotEvaluate("Lambda cannot be evaluated");
}