#include "Part.hpp"
#include "CommandMap.hpp"
#include "functions.hpp"


CommandMap* init();

Part_pt listEvaluate(std::string str);

Part_pt listEvaluate(std::string str, CommandMap &cmap);

void run(std::string &input);
