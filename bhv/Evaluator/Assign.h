#ifndef EVAL_ASSIGN_H
#define EVAL_ASSIGN_H

#include "Evaluator.h"
//#include <iostream>

Variable Evaluator::assign(Node* node, Datatable* dataFrom, Datatable* dataTo) {
  Tree* tree = node->asTree();
  //std::cout << tree << "\n";
  std::string varName = tree->getToken("word")->capture;
  std::string symbol = tree->getToken(1)->capture;
  
  Variable value = evaluate(tree->children.at(2), dataFrom);
  //std::cout << "assigning " << varName << " " << symbol << " " << value << "\n";
  //std::cout << std::hex << static_cast<void*>(dataFrom) << " -> " << static_cast<void*>(dataTo) << std::dec << "\n";
  //std::cout << "From: "; dataFrom->printFamily();
  //std::cout << "To: "; dataTo->printFamily();
  
  
  if (symbol == "+=") {
    value = dataFrom->get(varName) + value;
  }
  else if (symbol == "-=") {
    value = dataFrom->get(varName) - value;
  }
  else if (symbol == "*=") {
    Variable lhs = dataFrom->get(varName);
    if (lhs.type == Variable::LIST && value.type == Variable::NODE) {
      value = applyTreeOnList(lhs, value, dataFrom);
    }
    else if (lhs.type == Variable::STRING && value.type == Variable::NODE) {
      value = applyTreeOnString(lhs, value, dataFrom);
    }
    else if (lhs.type == Variable::LIST && value.type == Variable::CFUNC) {
      value = applyCFuncOnList(lhs, value, dataFrom);
    }
    else {
      value = dataFrom->get(varName) * value;
    }
  }
  else if (symbol == "/=") {
    value = dataFrom->get(varName) / value;
  }
  else if (symbol == "%=") {
    value = dataFrom->get(varName) % value;
  }

  dataTo->set(varName, value);

  return value;
}

#endif