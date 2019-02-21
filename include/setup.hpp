#include "Part.hpp"
#include "CommandMap.hpp"
#include "functions.hpp"


CommandMap* init();

Part* listEvaluate(std::string str);

// Must free memory
Part* listEvaluate(std::string str, CommandMap &cmap);

void run(std::string &input);
