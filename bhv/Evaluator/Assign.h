#ifndef EVAL_ASSIGN_H
#define EVAL_ASSIGN_H

#include "Evaluator.h"
//#include <iostream>

Variable Evaluator::assign(Node* node, Datatable* dataFrom, Datatable* dataTo) {
  Tree* tree = node->asTree();
  //std::cout << tree << "\n";
  std::string varName = tree->getToken(0)->capture;
  std::string symbol = tree->getToken(1)->capture;
  
  Variable value = evaluate(tree->children.at(2), dataFrom);

  if (symbol == "=") {
    if (value.isError()) {
      value.string += node->pos;
    }
  }
  else {
    Variable lhs = dataFrom->get(varName);

    if (lhs.isError()) {
      lhs.string += node->pos;
      return lhs;
    }
    
    if (symbol == "+=") {
      value = lhs + value;
    }
    else if (symbol == "-=") {
      value = lhs - value;
    }
    else if (symbol == "*=") {
      if (lhs.type == Variable::LIST && value.type == Variable::NODE) {
        value = applyTreeOnList(lhs, value, dataFrom);
      }
      else if (lhs.type == Variable::STRING && value.type == Variable::NODE) {
        value = applyTreeOnString(lhs, value, dataFrom);
      }
      else if (lhs.type == Variable::LIST && value.type == Variable::CFUNC) {
        value = applyTreeOnList(lhs, value, dataFrom);
      }
      else if (lhs.type == Variable::STRING && value.type == Variable::CFUNC) {
        value = applyTreeOnString(lhs, value, dataFrom);
      }
      else {
        value = lhs * value;
      }
    }
    else if (symbol == "/=") {
      value = lhs / value;
    }
    else if (symbol == "%=") {
      value = lhs % value;
    }
  }

  dataTo->set(varName, value);

  return value;
}

#endif