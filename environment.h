
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
  
  void showHelp() {
    std::cout << "!quit \n\tExits the Runtime Environment\n\n";
    std::cout << "!doc \n\tOpen the documentation at 'https://github.com/FabricioLince/BehaviourExolang/blob/main/README.md'\n\n";
    std::cout << "!load:filepath \n\tLoad and Evaluate the file denoted by filepath\n\n";
    std::cout << "!type:var \n\tReturns a string representation of the data type for var\n\n";
    std::cout << "!data \n\tPrints the Datatable at the current context\n\n";
  }
  
  public:
    
    Environment() {
      
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
      if (showDatatable) {
        std::cout << "Data:\n" << mainDatatable << "\n";
      }
    }
  
    void console() {
      std::cout << "Behaviour Runtime environment v0.1.10\n" ;
      std::cout << "type !help for help\n\n";
      
      while (true) {
  
        std::string userString;
        
        std::cout << ">>";
        
        getline(std::cin, userString);
        
        if (userString == "!quit") {
          break;
        }
        else if (userString == "!help") {
          showHelp();
        }
        else if (userString == "!doc") {
          system("start https://github.com/FabricioLince/BehaviourExolang/blob/main/README.md");
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
    
    Variable loadfile(std::string filename, Datatable* data) {
    std::ifstream file;
    file.open(filename.c_str());
    if (file.is_open()) {
      file.close();
      Parser::Stream* stream = new Parser::FileStream(filename);
      SafeNode safeNode = extractNodeSafe(stream);
      delete stream;
      if (safeNode.node) {
        if (showParseTree)
          std::cout << safeNode.node << "\n";
        nodesLoaded.push_back(safeNode.node);
        return evaluator.evaluate(safeNode.node, data);
      }
      else {
        std::cerr << "Parsing failed\n";
        std::cerr << safeNode.error << "\n";
      }
    }
    else {
      std::cerr << "couldn't open '" << filename << "'\n";
    }
    return Variable();
  }
};


