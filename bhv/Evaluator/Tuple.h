#ifndef TUPLE_H
#define TUPLE_H

#include "Evaluator.h"

Variable Evaluator::tuple(Tree* tree, Datatable* data) {
  //std::cout << tree << std::endl;
  Tree* entries = tree->subTree(1);
  
  Datatable* table;
  if (tree->getToken(0)->string == "${") {
    table = data->makeOrphan();
    table->context = data;
  }
  else {
    table = data->makeChild();
  }
  
  for (unsigned int i = 0; i < entries->children.size(); ++i) {
    //std::cout << entries->subTree(i) << std::endl;
    evaluate(entries->children.at(i), table);
  }
  table->context = NULL;
  return Variable(table);
}

#endif