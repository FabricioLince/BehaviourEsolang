
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
    
    void setPrintLineNumber(bool print) {
      evaluator.printLineNumber = print;
    }
  
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
      std::cout << "Behaviour Runtime environment v0.2.1\n" ;
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
          if (countPairs(expression)) {
            Variable value = execute(expression, &mainDatatable);
            std::cout << "= " << value << "\n";
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
        Node* node = parse(stream);
        delete stream;
        if (node) {
          return evaluator.evaluate(node, data);
        }
      }
      else {
        std::cerr << "couldn't open '" << filename << "'\n";
      }
      return Variable();
    }
    
    Variable execute(std::string expression, Datatable* data) {
      Node* node = parse(expression);
      if (node) {
        return evaluator.evaluate(node, data);
      }
      return Variable();
    }
    
    Node* parse(Stream* stream) {
      SafeNode sn = extractNodeSafe(stream);
      if (sn.node) {        
        if (showParseTree)
          std::cout << sn.node << "\n";
        nodesLoaded.push_back(sn.node);
        return sn.node;
      }
      else {
        std::cerr << "Parsing failed\n";
        std::cerr << sn.error << "\n";
      }
      return NULL;
    }
    Node* parse(std::string string) {
      Stream *stream = new StringStream(string);
      Node* node = parse(stream);
      delete stream;
      return node;
    }
  
  private:
    bool countPairs(std::string expression) {
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
      return parCount == 0 && brackCount == 0 && curlyCount == 0;
    }
};


