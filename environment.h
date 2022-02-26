
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
      std::cout << "Behaviour Runtime environment v0.2.0\n" ;
      std::cout << "type !help for help\n\n";
      
      std::string expression;
      
      while (true) {
  
        std::string userString;
        
        if (expression.length() == 0) {
          std::cout << ">>";
        }
        else {
          std::cout << "..";
        }
        
        getline(std::cin, userString);
        expression += userString;
        
        if (expression == "!quit") {
          break;
        }
        else if (expression == "!help") {
          showHelp();
          expression = "";
        }
        else if (expression == "!doc") {
          system("start https://github.com/FabricioLince/BehaviourExolang/blob/main/README.md");
          expression = "";
        }
        else if (expression.length() > 0) {
          
          int parCount = 0;
          int brackCount = 0;
          int curlyCount = 0;
          for (char c : expression) {
            switch (c) {
              case '(':
                parCount += 1;
                break;
              case ')':
                parCount -= 1;
                break;
              case '[':
                brackCount += 1;
                break;
              case ']':
                brackCount -= 1;
                break;
              case '{':
                curlyCount += 1;
                break;
              case '}':
                curlyCount -= 1;
                break;
            }
          }
          
          if (parCount == 0 && brackCount == 0 && curlyCount == 0) {
            execute(expression, &mainDatatable);
            expression = "";
          }
          else {
            expression += "\n";
          }
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
  
  void execute(std::string expression, Datatable* data) {
    Stream *stream = new StringStream(expression);
    SafeNode sn = extractNodeSafe(stream);
    if (sn.node) {
      Node* node = sn.node;
      if (showParseTree)
        std::cout << node << "\n";
      
      Variable value = evaluator.evaluate(node, data);
      
      std::cout << "= " << value << "\n";
      
      nodesLoaded.push_back(node);
    } 
    else {
      std::cerr << "Parsing failed\n";
      std::cerr << sn.error << "\n";
    }
    delete stream;
  }
};


