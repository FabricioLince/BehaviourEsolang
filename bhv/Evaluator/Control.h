#ifndef EVAL_SELECT_H
#define EVAL_SELECT_H

#include "Evaluator.h"


Variable Evaluator::sequence(Tree* tree, Datatable* data) {
  NodeList* children = &tree->children;
  Datatable* childData = data;
  if (children->size() == 1) {
    return evaluate(children->at(0), childData);
  }
  Variable r = Variable::error(std::string("Empty Sequencer")+tree->pos);
  for (Node* child : *children) {
    const Variable& temp = evaluate(child, childData);
    if (child->id == print_id || child->id == optional_id) {
       if (r.type == Variable::NIL) {
         r = temp;
       }
    }
    else {
      r = temp;
    }
    if (!r.toBool()) {
      return Variable::error(
        std::string("Sequencer Failed")+tree->pos, 
        r
      );
    }
  }
  return r;
}

Variable Evaluator::select(Tree* tree, Datatable* data) {
  Variable::VarList errors;
  for (Node* child : tree->subTree(0)->children) {
    const Variable& r = evaluate(child, data);
    if (r.toBool()) {
      return r;
    }
    if (r.isError())
      errors.push_back(r);
  }
  return Variable::error(
    std::string("No children succeeded") + tree->pos, 
    errors
  );
}

Variable Evaluator::repeat(Tree* tree, Datatable* data) {
  if (tree->subTree(0)->children.size() > 0) {
    const Variable& max = evaluate(tree->subTree(0)->subTree(0)->children.at(0), data);
    if (max.type == Variable::NUMBER) {
      Node* child = tree->children.at(1);
      Variable r;
      for (int i = 0; i < max.number; ++i) {
        r = evaluate(child, data);
        if (r.toBool()) {
          return r;
        }
      }
      return Variable::error(
        std::string("Repeater exceeded max tries")+tree->pos, 
        r
      );
    }
  }
  Node* child = tree->children.at(1);
  Variable r = evaluate(child, data);
  while (!r.toBool()) {
    r = evaluate(child, data);
  }
  return r;
}

Variable Evaluator::negate(Tree* tree, Datatable* data) {
  const Variable& r = evaluate(tree->children.at(0), data);
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
      
  if (other.type == Variable::NODE || other.type == Variable::CFUNC) {
    const Variable& value = evaluate(tree->children.at(0), data);
    other = executeAny(other, data, value);
  }
  
  if (other.toBool()) {
    return evaluate(tree->children.at(0), data);
  }
  if (other.isError())
    return Variable::error(
      std::string("Failed condition") + cond->pos,
      other);
  return Variable::error(
    std::string("Failed condition") + cond->pos);
}

#endif