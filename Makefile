CXX = g++ -std=c++11
CXXFLAGS= -Wall -fmessage-length=0
OPTIMIZE= -O3 -fomit-frame-pointer

SRCDIR  = src
OBJDIR  = obj
INCDIR  = include
TSTDIR  = test

CMD     = $(CXX) $(CXXFLAGS) $(OPTIMIZE) -I$(INCDIR)
CMDTEST = $(CMD) -I$(TSTDIR)

# Point this to location of cxxtest library
TESTGEN = ~/cplusplus/cxxtest-4.3/bin/cxxtestgen
TEST 	= Part



BASE    = parse Part exceptions functions

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
test: $(patsubst %, $(TSTDIR)/Test%.hpp, $(TEST)) $(patsubst %, $(OBJDIR)/%.o, $(TEST))
	$(TESTGEN) --error-printer -o $(TSTDIR)/RunTest.cpp $(patsubst %, $(TSTDIR)/Test%.hpp, $(TEST))
	$(CMDTEST) -o runTest $(patsubst %, $(OBJDIR)/%.o, $(TEST)) $(TSTDIR)/RunTest.cpp