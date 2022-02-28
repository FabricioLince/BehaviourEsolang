
#include "bhv/Header.h"
#include "Parser/Stream.h"

#include <time.h>
#include <iostream>

#include "def_lib.h"
#include "environment.h"

#define RUN_FILE true

Environment env; 

bool anyarg(int argc, char** argv, const char* arg);
Variable loadfile_cfunc(Datatable* data);
Variable exec_cfunc(Datatable* data);
Variable parse_cfunc(Datatable* data);

int main(int argc, char** argv) {
  
  env.showResult = anyarg(argc, argv, "-result");
  env.showDatatable = anyarg(argc, argv, "-data");
  env.showParseTree = anyarg(argc, argv, "-tree");
  env.setPrintLineNumber(anyarg(argc, argv, "-ln"));
  
  addToDatatable(env.getDatatable());
  
  env.getDatatable()->setOrphanCFunc("load", &loadfile_cfunc);
  env.getDatatable()->setOrphanCFunc("exec", &exec_cfunc);
  env.getDatatable()->setOrphanCFunc("parse", &parse_cfunc);
  
  if (RUN_FILE) {
    std::string fileName = "test.bhv";
    if (argc > 1) {
      fileName = argv[1];
    }
    
    env.runfile(fileName);
  }
  else {
    env.console();
  }
  
	return 0;
}


bool anyarg(int argc, char** argv, const char* arg) {
  for (int i = 0; i < argc; ++i) {
    if (strcmp(arg, argv[i]) == 0) {
      return true;
    }
  }
  return false;
}

Variable loadfile_cfunc(Datatable* data) {
  Variable filename = data->getLocal("a");
  if (filename.type == Variable::STRING) {
    return env.loadfile(filename.string, data);
  }
  else {
    std::cerr << "no file " << "\n";
  }
  return Variable();
}
Variable exec_cfunc(Datatable* data) {
  Variable expression = data->get("a");
  if (expression.type == Variable::STRING) {
    return env.execute(expression.string, data);
  }
  return Variable();
}
Variable parse_cfunc(Datatable* data) {
  Variable expression = data->get("a");
  if (expression.type == Variable::STRING) {
    Node* node = env.parse(expression.string);
    if (node) {
      return Variable(node, data);
    }
  }
  return Variable();
}
  