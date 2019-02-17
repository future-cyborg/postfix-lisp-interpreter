#include <string>
#include <vector>

#ifndef PART_H
#define PART_H

typedef long num_t;

#define whitespaces " \t\f\v\n\r"
#define whiteParen " \t\f\v\n\r()"
#define parens "()"


class Part {
public:
	virtual ~Part();
	virtual std::string getVal();
	virtual std::string getType();
	virtual Part* call(std::vector<Part*> args);
	virtual Part* evaluate();
	virtual Part* copy();

private:
	std::string value = "PART";
};


class List : public Part {
public:
	List(std::string  v);
	std::string getType();
	std::string getVal();
	Part* call(std::vector<Part*> args);
	Part* evaluate();
	Part* copy();

private:
	std::string value;
};


class Atom : public Part {
public:	
	Atom(std::string v);
	virtual std::string getType();
	virtual std::string getVal();
	virtual Part* call(std::vector<Part*> args);
	virtual Part* evaluate();
	Part* copy();

	friend bool operator== (const Atom &a1, const Atom &a2);
    friend bool operator!= (const Atom &a1, const Atom &a2);

private:
	std::string value;
};


class Number : public Part {
public:
	Number(std::string v);
	std::string getType();
	std::string getVal();
	Part* call(std::vector<Part*> args);
	Part* evaluate();
	Part* copy();

	friend bool operator== (const Number &n1, const Number &n2);
    friend bool operator!= (const Number &n1, const Number &n2);

private:
	std::string value;
};


class Lambda : public Part {
public:
	Lambda(Part* args, Part* statement);
	Lambda(const Lambda &lambda);
	~Lambda();
	std::string getType();
	std::string getVal();
	Part* call(std::vector<Part*> args);
	Part* evaluate();
	Part* copy();

private:
	std::string value;
	std::vector<Part*> argTemplates;
	int numArgs = 0;
	Part* expression;
};


#endif