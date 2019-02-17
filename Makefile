CXX = g++ -std=c++11
CXXFLAGS= -Wall -fmessage-length=0 -g
OPTIMIZE= -O3 -fomit-frame-pointer

SRCDIR  = src
OBJDIR  = obj
INCDIR  = include
TSTDIR  = test

CMD     = $(CXX) $(CXXFLAGS) $(OPTIMIZE) -I$(INCDIR)
CMDTEST = $(CMD) -I$(TSTDIR)

BASE    = parse Part exceptions functions

# Point this to location of cxxtest library
TESTGEN = ~/cplusplus/cxxtest-4.3/bin/cxxtestgen
TESTDEP	= $(BASE)
TEST    = PartTypes





all: postlisp

postlisp: $(OBJDIR)/postlisp.o $(patsubst %, $(OBJDIR)/%.o, $(BASE))
	$(CMD) $^ -o $@

$(OBJDIR)/postlisp.o: $(SRCDIR)/postlisp.cpp $(patsubst %, $(INCDIR)/%.hpp, $(BASE))
	$(CMD) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.hpp #$(patsubst %, $(INCDIR)/%.hpp, $(BASE))
	$(CMD) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o  postlisp
	rm -f $(TSTDIR)/RunTest $(TSTDIR)/RunTest.cpp


.PHONY: test
test: $(patsubst %, $(TSTDIR)/Test%.hpp, $(TEST)) $(patsubst %, $(OBJDIR)/%.o, $(TESTDEP))
	$(TESTGEN) --error-printer -o $(TSTDIR)/RunTest.cpp $(patsubst %, $(TSTDIR)/Test%.hpp, $(TEST))
	$(CMDTEST) -o runTest $(patsubst %, $(OBJDIR)/%.o, $(TESTDEP)) $(TSTDIR)/RunTest.cpp