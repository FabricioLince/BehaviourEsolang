#ifndef EVAL_PRINT_H
#define EVAL_PRINT_H

#include "Evaluator.h"
#include <iostream>

Variable Evaluator::print(Tree* tree, Datatable* data) {
  
  Variable value = evaluate(tree->children.at(0), data);
  if (printLineNumber) {
    Token* t = tree->firstToken();
    if (t) {
      std::cout << t->line << ": " << value ;
    }
    else {
      std::cout << "?: " << value ;
    }
  }
  else {
    std::cout << value;
  }
  std::cout << std::endl;
  return Variable(true);
}

#endif