#include "cxxtest/TestSuite.h"
#include "exceptions.hpp"
#include "Part.hpp"
#include <iostream>

using std::vector;
using std::string;

class TestPartTypes : public CxxTest::TestSuite {
public:

	vector<Part*> getParts() {
		vector<Part*> allParts;

		allParts.push_back(new Part());
		allParts.push_back(new List("list"));
		allParts.push_back(new Atom("atom"));
		allParts.push_back(new Number("num"));
		Part* arg = new Atom("x");
		Part* exp = new Atom("x x *");
		allParts.push_back(new Lambda(arg, exp));

		return allParts;
	}

	vector<Part*> allParts = getParts();


	void testCall(Part *part) {
		if(part->getType().compare("Lambda") != 0) {
			TS_ASSERT_THROWS(part->call(vector<Part*>()), CannotCall);
		}
	}

	void testCallAll(void) {
		vector<Part *> allParts = getParts();
		for(Part *part : allParts) {
			testCall(part);
		}
	}

	void testEvaluate(Part *part) {
		if(part->getType().compare("List") != 0) {

			TS_ASSERT_THROWS(part->evaluate(), CannotEvaluate);
		}
	}

	void testEvaluateAll(void) {
		vector<Part *> allParts = getParts();
		for(Part *part : allParts) {
			testCall(part);
		}
	}
};