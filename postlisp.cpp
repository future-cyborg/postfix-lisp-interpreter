#include <string>
#include <iostream>
#include <unordered_map>
#include <cstdarg>
#include <vector>
#include "Part.hpp"
#include "exceptions.hpp"
#include "parse.hpp"
#include "functions.hpp"
#include "optionparser.h"
#include <sys/ioctl.h>

using std::string;
using std::unordered_map;
using std::vector;

std::ifstream::pos_type fileSize(const std::string fileName) {
    std::ifstream in(fileName, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

struct Arg: public option::Arg {
    static void printError(const char* msg1, const option::Option& opt, const char* msg2) {
        fprintf(stderr, "%s", msg1);
        fwrite(opt.name, opt.namelen, 1, stderr);
        fprintf(stderr, "%s", msg2);
    }

    static option::ArgStatus Unknown(const option::Option& option, bool msg) {
        if (msg) printError("Unknown option '", option, "'\n");
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus NonEmpty(const option::Option& option, bool msg) {
        if (option.arg != 0 && option.arg[0] != 0)
          return option::ARG_OK;

        if (msg) printError("Option '", option, "' requires a non-empty argument\n");
        return option::ARG_ILLEGAL;
    }
};

enum  optionIndex { UNKNOWN, HELP, INTER, INFILE };
const option::Descriptor usage[] =
{
 {UNKNOWN, 0,"" , ""    ,Arg::Unknown,  "USAGE: postlisp [-i] [-f FILE]\n"
                                        "Options:" },
 {HELP,	   0,"h", "help",Arg::None,     "  -h, --help \tPrint usage and exit"},
 {INTER,   0,"i" , "",   Arg::None,     "  -i  \tInteractive mode" },
 {INFILE,  0,"f", "file",Arg::NonEmpty, "  -f <FILE>, --file  \tFile to execute" },
 {UNKNOWN, 0,"" ,  ""   ,Arg::None,     "\nExamples:\n"
                                        "  postlisp -i\n"
                                        "  postlisp -i -f lisp.txt\n"
                                        "  postlisp -f lisp.txt\n" },
 {0,0,0,0,0,0}
};

int main(int argc, char* argv[]) {
	init();
	argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
	option::Stats  stats(usage, argc, argv);
	std::vector<option::Option> options(stats.options_max), buffer(stats.buffer_max);
	option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);
	if (parse.error())
		return 1;
	if (options[HELP] || argc == 0) {
		option::printUsage(std::cout, usage);
		return 0;
	}

	if(options[INFILE]) {
		std::string text;
		const char *fileName = options[INFILE].last()->arg;
		text.reserve(fileSize(fileName));
		std::ifstream fileReader(fileName);
		if(!fileReader) {
		    fprintf(stderr, "%s can not be opened.\n", fileName);
		    return 2;
		}

	    std::stringstream sstr;
	    sstr << fileReader.rdbuf();
	    text = sstr.str();
	    run(text);
	    //run(sstr.str());
	}

	std::string input;
	if(options[INTER]) {
		std::cout << "postlisp: ";
		while(getline(std::cin, input)) {

			if(input.compare("exit") == 0) {
				break;
			}
			std::cout << "|" << input << "|\n";
			run(input);
			std::cout << "postlisp: ";
		}
	}



	return 0;
}