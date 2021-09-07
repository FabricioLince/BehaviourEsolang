
#include "bhv/Header.h"
#include "Parser/Stream.h"

#include <time.h>
#include <iostream>

bool anyarg(int argc, char** argv, const char* arg) {
  for (int i = 0; i < argc; ++i) {
    if (strcmp(arg, argv[i]) == 0) {
      return true;
    }
  }
  return false;
}

Variable type(Datatable* data) {
  Variable arg = data->get("a");
  switch (arg.type) {
    case Variable::NIL:
          return "nil";
        case Variable::NUMBER:
          return "number";
        case Variable::STRING:
          return "string";
        case Variable::BOOL:
          return "bool";
        case Variable::NODE:
          return "node";
        case Variable::LIST:
          return "list";
        case Variable::CFUNC:
          return "cfunc";
  }
  return Variable();
}
Variable rand(Datatable* data) {
  if (!data->has("seed")) {
    srand(time(NULL));
    data->setLocal("seed", (int)time(NULL));
  }
  return rand();
}
Variable input(Datatable* data) {
  if (!data->has("number")) {
    data->setLocal("number", 0);
    data->setLocal("string", 1);
  }
  Variable arg = data->getLocal("a");
  if (arg.type == Variable::NUMBER) {
    switch ((int)arg.number) {
      case 0:
      {
        int n = 0;
        std::cin >> n;
        return n;
      }
      case 1:
      {
        std::string s;
        getline(std::cin, s);
        return s;
      }
    }
  }
  return Variable();
}

int main(int argc, char** argv) {
	std::string fileName = "test.bhv";
	if (argc > 1) {
		fileName = argv[1];
	}
  
	Bhv bhv;
	Stream *stream = new FileStream(fileName);
	Parser::Node *node = bhv.extractTree(stream);
	if (node) {
    if (anyarg(argc, argv, "-tree"))
      std::cout << node << "\n";
		
    Datatable data;
    data.set("type", &type);
    data.set("rand", Variable(&rand, data.makeChild())); 
    data.set("read", Variable(&input, data.makeChild()));
		
    Evaluator evaluator;
    clock_t start = clock();
		Variable value = evaluator.evaluate(node, &data);
    //std::cout << "Time taken: " << ((double)(clock() - start)/CLOCKS_PER_SEC) << "s\n";
    
    if (anyarg(argc, argv, "-result"))
      std::cout << "\nresult = " << value << "\n";
    
    if (anyarg(argc, argv, "-data"))
      std::cout << "Data:\n" << data << "\n";
		
    delete node;
    
	} else {
		std::cout << "Parsing failed\n";
	}

	delete stream;
	return 0;
}
