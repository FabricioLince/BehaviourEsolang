#ifndef EVAL_PRINT_H
#define EVAL_PRINT_H

#include "Evaluator.h"
#include <iostream>

Variable Evaluator::print(Node* node, Datatable* data) {
  Tree* tree = node->asTree();
  
  Variable value = evaluate(tree->children.at(1), data);
  std::cout << value ;
  if (tree->children.at(0)->asTree()->children.size() > 0)
    std::cout << "\n";
  //std::cout << "Data:" << data->depth() << "\n" << data << "\n";
  return Variable(true);
}

#endif