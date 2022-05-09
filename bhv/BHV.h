#ifndef BHV_MAIN_H
#define BHV_MAIN_H

#include "../Parser/RuleCollector.h"

using namespace Parser;

typedef Parser::BaseRule* Rule;

class Bhv {
  Rule main;
  RuleCollection collection;
  void constructRules();

  public:
    Bhv() {
      constructRules();
    }
    
    Parser::Node* extractTree(Parser::TokenStream* stream) {
      Node* node = main->execute(stream);
      
      if (node) {
        //std::cout <<node << std::endl;
        node->asTree()->removeEmpty({"ops", "comp", "cond", "cases", "postop"});
        node->asTree()->subsOnlyChild({
          "expression", "comparation", "addition", "multiplication", 
          "comp", "parentheses", "arg", "sequence", "ifcond", "cond",
          "match"});
        //std::cout <<node << std::endl;
      }
      return node;
    }

    ~Bhv() {
      for (RuleCollection::iterator it = collection.begin(); it != collection.end(); ++it) {
        //std::cout << (*it)->name << "\n";
        delete (*it);
      }
    }
};


#endif