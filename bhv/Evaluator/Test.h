#ifndef EVAL_TEST_H
#define EVAL_TEST_H

#include "Evaluator.h"
#include <iostream>

Variable Evaluator::test(Node* node, Datatable* data) {
  Tree* tree = node->asTree();
  Variable value = evaluate(tree->children.at(0), data);
  return Variable(value.toBool());
}

#endif