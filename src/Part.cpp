#include <string>
#include <vector>
#include <algorithm>
#include "exceptions.hpp"
#include "Part.hpp"
#include "parse.hpp"


Part::~Part () {};

std::string Part::getVal() {
    return value;
}
std::string Part::getType() {
    return "Part";
}
Part* Part::call(std::vector<Part*> args) {
    throw CannotCall("Part cannot be called");
}
Part* Part::evaluate(){
    throw CannotEvaluate("Part cannot be evaluated");
}
Part* Part::copy() {
    throw Exception("Part cannot be copied");
}


List::List(std::string v) {
	value = v;
}
std::string List::getVal() {
    return value;
}
std::string List::getType() {
	return "List";
}
Part* List::call(std::vector<Part*> args) {
    throw CannotCall("List cannot be called");
}
Part* List::evaluate() {
    return listEvaluate(value);
}
Part* List::copy() {
    return new List(value);
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
Part* Atom::call(std::vector<Part*> args) {
    throw CannotCall("Atom cannot be called");
}
Part* Atom::evaluate() {
    throw CannotEvaluate("Atom cannot be evaluated");
}
Part* Atom::copy() {
    return new Atom(value);
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
Part* Number::call(std::vector<Part*> args) {
    throw CannotCall("Number cannot be called");
}
Part* Number::evaluate() {
    throw CannotEvaluate("Number cannot be evaluated");
}
Part* Number::copy() {
    return new Number(value);
}


bool operator== (const Number &n1, const Number &n2)
{
    return (n1.value.compare(n2.value) == 0);
}
 
bool operator!= (const Number &n1, const Number &n2)
{
    return !(n1.value.compare(n2.value) == 0);
}




Lambda::Lambda(Part args, Part statement) : expression {statement} {
	std::vector<Part*> parts = parse(args.getVal());
	for(Part* part : parts) {
		if(part->getType().compare("Atom") != 0) {
			throw Exception("Lambda parameters can only contain Atoms");
		}
		++numArgs;
		argTemplates.push_back(part);
	}
}

Lambda::Lambda(const Lambda &lambda) :
    argTemplates(lambda.argTemplates),
    numArgs(lambda.numArgs),
    expression(lambda.expression) {}

Lambda::~Lambda() {
    for(int i = 0; i < (int)argTemplates.size(); i++) {
        delete argTemplates.at(i);
    }
}

Part* Lambda::call(std::vector<Part*> args) {
	std::string exp(expression.getVal());
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
	return new List(exp);
}

std::string Lambda::getVal() {
    throw Exception("Lambda has no value");
}
std::string Lambda::getType() {
    return "Lambda";
}

Part* Lambda::evaluate(){
    throw CannotEvaluate("Lambda cannot be evaluated");
}
Part* Lambda::copy(){
    return new Lambda(*this);
}