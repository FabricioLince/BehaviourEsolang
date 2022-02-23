
#include "bhv/Header.h"
#include "Parser/Stream.h"

#include <time.h>
#include <iostream>

#include "def_lib.h"
#include "environment.h"

bool anyarg(int argc, char** argv, const char* arg) {
  for (int i = 0; i < argc; ++i) {
    if (strcmp(arg, argv[i]) == 0) {
      return true;
    }
  }
  return false;
}

int main(int argc, char** argv) {
	std::string fileName = "test.bhv";
	if (argc > 1) {
		fileName = argv[1];
	}
  
  Environment env;
  env.showResult = anyarg(argc, argv, "-result");
  env.showDatatable = anyarg(argc, argv, "-data");
  env.showParseTree = anyarg(argc, argv, "-tree");
  addToDatatable(env.getDatatable());
  
  //env.runfile(fileName);
  env.console();
  
	return 0;
}


