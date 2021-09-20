#ifndef EVAL_SELECT_H
#define EVAL_SELECT_H

#include "Evaluator.h"


Variable Evaluator::sequence(Node* node, Datatable* data) {
  NodeList* children = &node->asTree()->children;
  Datatable* childData = data->makeChild();
  if (children->size() == 1) {
    return evaluate(children->at(0), childData);
  }
  Variable r;
  for (Node* child : *children) {
    r = evaluate(child, childData);
    if (!r.toBool()) {
      return r;
    }
  }
  return r;
}

Variable Evaluator::select(Node* node, Datatable* data) {
  Tree* tree = node->asTree();
  for (Node* child : tree->subTree(0)->children) {
    Variable r = evaluate(child, data);
    if (r.toBool()) {
      return r;
    }
  }
  return Variable();
}


Variable Evaluator::repeat(Node* node, Datatable* data) {
  Tree* tree = node->asTree();
  Variable r = evaluate(tree->children.at(0), data);
  while (!r.toBool()) {
    r = evaluate(tree->children.at(0), data);
  }
  return r;
}

Variable Evaluator::negate(Node* node, Datatable* data) {
  Variable r = evaluate(node->asTree()->children.at(0), data);
  return Variable(!r.toBool());
}

Variable Evaluator::optional(Node* node, Datatable* data) {
  evaluate(node->asTree()->children.at(0), data);
  return true;
}

#endif