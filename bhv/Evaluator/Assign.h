#ifndef EVAL_ASSIGN_H
#define EVAL_ASSIGN_H

#include "Evaluator.h"
//#include <iostream>

Variable Evaluator::assign(Node* node, Datatable* dataFrom, Datatable* dataTo) {
  Tree* tree = node->asTree();
  //std::cout << tree << "\n";
  const std::string& varName = tree->getToken(0)->capture;
  const std::string& symbol = tree->getToken(1)->capture;
  
  const Variable& value = evaluate(tree->children.at(2), dataFrom);
  
  if (symbol == "=") {
    if (value.isError()) {
      dataTo->set(varName, Variable::error(
        value.string + node->pos)
      );
    }
    else {
      dataTo->set(varName, value);
    }
  }
  else {
    const Variable& lhs = dataFrom->get(varName);

    if (lhs.isError()) {
      return Variable::error(lhs.string + node->pos);
    }
    
    if (symbol == "+=") {
      dataTo->set(varName, lhs + value);
    }
    else if (symbol == "-=") {
      dataTo->set(varName, lhs - value);
    }
    else if (symbol == "*=") {
      if (lhs.type == Variable::LIST &&
        (value.type == Variable::NODE 
        || value.type == Variable::CFUNC)) {
          dataTo->set(varName, 
            applyTreeOnList(lhs, value, dataFrom)
          );
      }
      else if (lhs.type == Variable::STRING && 
        (value.type == Variable::NODE 
        || value.type == Variable::CFUNC)) {
          dataTo->set(varName,
            applyTreeOnString(lhs, value, dataFrom)
          );
      }
      else {
        dataTo->set(varName, lhs * value);
      }
    }
    else if (symbol == "/=") {
      dataTo->set(varName, lhs / value);
    }
    else if (symbol == "%=") {
      dataTo->set(varName, lhs % value);
    }
  }

  return dataTo->get(varName);
}


#endif