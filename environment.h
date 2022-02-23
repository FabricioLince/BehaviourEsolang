
#include "bhv/Header.h"
#include "Parser/Stream.h"

#include <time.h>
#include <iostream>

class Environment {
  
  Bhv bhv;
  Evaluator evaluator;
  std::vector<Node*> nodesLoaded; 
  Datatable mainDatatable;
  
  
  struct SafeNode {
    Node* node = NULL;
    std::string error;
  };

  SafeNode extractNodeSafe(Stream* stream) {
    SafeNode sn;
    try {
      Parser::Node *node = bhv.extractTree(stream);
      sn.node = node;
    }
    catch (Parser::ParsingError e) {
      sn.error = e.what();
    }
    return sn;
  }
  
  
  Variable loadfile(std::string filename, Datatable* data) {
    std::ifstream file;
    file.open(filename.c_str());
    if (file.is_open()) {
      file.close();
      Parser::Stream* stream = new Parser::FileStream(filename);
      Node* node = bhv.extractTree(stream);
      delete stream;
      if (node) {
        nodesLoaded.push_back(node);
        return evaluator.evaluate(node, data);
      }
    }
    else {
      std::cerr << "couldn't open '" << filename << "'\n";
    }
    return Variable();
  }

  Variable loadfile_cfunc(Datatable* data) {
    Variable filename = data->getLocal("a");
    if (filename.type == Variable::STRING) {
      return loadfile(filename.string, data);
    }
    else {
      std::cerr << "no file " << "\n";
    }
    return Variable();
  }
  
  public:
    
    Environment() {
      mainDatatable.setOrphanCFunc("load", &loadfile_cfunc);
    }
    ~Environment() {
      for (Node* node : nodesLoaded) {
        delete node;
      }
    }
    
    Datatable* getDatatable() {
      return &mainDatatable;
    }
    
    bool showParseTree = false;
    bool showDatatable = false;
    bool showResult = false;
  
    void runfile(std::string filename) {
      Variable result = loadfile(filename, &mainDatatable);
      if (showResult) {
        std::cout << "result = " << result << "\n";
      }
    }
  
    void console() {
      std::cout << "Behaviour Runtime environment v0.1.10\n\n" ;
      
      while (true) {
  
        std::string userString;
        
        std::cout << ">>";
        
        getline(std::cin, userString);
        
        if (userString == "!quit") {
          break;
        }
        else {
          Stream *stream = new StringStream(userString);
          SafeNode sn = extractNodeSafe(stream);
          if (sn.node) {
            Node* node = sn.node;
            if (showParseTree)
              std::cout << node << "\n";
            
            Variable value = evaluator.evaluate(node, &mainDatatable);
            
            std::cout << "= " << value << "\n";
            
            nodesLoaded.push_back(node);
            
          } else {
            std::cerr << "Parsing failed\n";
            std::cerr << sn.error << "\n";
            
          }
          delete stream;
        }
      }
      
      if (showDatatable) {
        std::cout << "Data:\n" << mainDatatable << "\n";
      }
      
      
    }
};


