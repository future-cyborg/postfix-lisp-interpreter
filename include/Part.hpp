#include <string>
#include <vector>
#include <memory>

#ifndef PART_H
#define PART_H

typedef long num_t;
// typedef std::shared_ptr<Part> Part_pt;
// typedef std::shared_ptr<List> List_pt;
// typedef std::shared_ptr<Atom> Atom_pt;
// typedef std::shared_ptr<Lambda> Lambda_pt;

#define whitespaces " \t\f\v\n\r"
#define whiteParen " \t\f\v\n\r()"
#define parens "()"


class Part {
public:
	virtual ~Part();
	virtual std::string getVal();
	virtual std::string getType();
	virtual std::shared_ptr<Part> call(std::vector<std::shared_ptr<Part>> args);
	virtual std::shared_ptr<Part> evaluate();
	// bool isType(std::string str);

protected:
	// std::string type;
	std::string value = "PART";
};

typedef std::shared_ptr<Part> Part_pt;

class List : public Part {
public:
	List(std::string  v);
	std::string getType();
	std::string getVal();
	Part_pt call(std::vector<Part_pt> args);
	Part_pt evaluate();
	bool shouldEvaluate();

private:
	// std::string value;
	bool shouldEval = false;
};

typedef std::shared_ptr<List> List_pt;

class Atom : public Part {
public:	
	Atom(std::string v);
	virtual std::string getType();
	virtual std::string getVal();
	virtual Part_pt call(std::vector<Part_pt> args);
	virtual Part_pt evaluate();

	friend bool operator== (const Atom &a1, const Atom &a2);
    friend bool operator!= (const Atom &a1, const Atom &a2);

private:
	// std::string value;
};


class Number : public Part {
public:
	Number(std::string v);
	std::string getType();
	std::string getVal();
	Part_pt call(std::vector<Part_pt> args);
	Part_pt evaluate();

	friend bool operator== (const Number &n1, const Number &n2);
    friend bool operator!= (const Number &n1, const Number &n2);

private:
	// std::string value;
};


class Lambda : public Part {
public:
	Lambda(Part_pt args, Part_pt statement);
	Lambda(const Lambda &lambda);
	~Lambda();
	std::string getType();
	std::string getVal();
	Part_pt call(std::vector<Part_pt> args);
	Part_pt evaluate();

private:
	// std::string value;
	std::vector<Part_pt> argTemplates;
	int numArgs = 0; 
	Part_pt const expression;
};


#endif