#ifndef BHV_MAIN_H
#define BHV_MAIN_H

#include "../Parser/RuleCollector.h"

using namespace Parser;

typedef Parser::BaseRule* Rule;

class Bhv {
  Rule main;
  RuleCollection collection;
  Rule Symbol(std::string patt);
  void constructRules();

  public:
    Bhv() {
      constructRules();
    }
    
    Parser::Node* extractTree(Parser::Stream* stream) {
      Node* node = main->execute(stream);
       if (node) {
        node->asTree()->removeEmpty({"ops", "comp", "cond"});
        node->asTree()->subsOnlyChild({"expression", "comparation", "addition", "multiplication", "comp", "parentheses", "arg", "sequence", "ifcond", "cond"});
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