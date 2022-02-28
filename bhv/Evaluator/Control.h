#ifndef EVAL_SELECT_H
#define EVAL_SELECT_H

#include "Evaluator.h"


Variable Evaluator::sequence(Tree* tree, Datatable* data) {
  NodeList* children = &tree->children;
  Datatable* childData = data;//->makeChild();
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

Variable Evaluator::select(Tree* tree, Datatable* data) {
  for (Node* child : tree->subTree(0)->children) {
    Variable r = evaluate(child, data);
    if (r.toBool()) {
      return r;
    }
  }
  return Variable();
}


Variable Evaluator::repeat(Tree* tree, Datatable* data) {
  Variable r = evaluate(tree->children.at(0), data);
  while (!r.toBool()) {
    r = evaluate(tree->children.at(0), data);
  }
  return r;
}

Variable Evaluator::negate(Tree* tree, Datatable* data) {
  Variable r = evaluate(tree->children.at(0), data);
  return Variable(!r.toBool());
}

Variable Evaluator::optional(Tree* tree, Datatable* data) {
  evaluate(tree->children.at(0), data);
  return true;
}


Variable Evaluator::ifcond(Tree* tree, Datatable* data) {
  
  Node* cond = tree->subTree(1)->children.at(1);
  //std::cout << "cond = " << cond << "\n";
  
  Variable other = evaluate(cond, data);
      
  if (other.type == Variable::NODE or other.type == Variable::CFUNC) {
    Variable value = evaluate(tree->children.at(0), data);
    other = executeAny(other, data, value);
  }
  
  if (other.toBool()) {
    return evaluate(tree->children.at(0), data);
  }
  
  return Variable();
}

#endif