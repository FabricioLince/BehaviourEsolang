#ifndef EVAL_MATCH_H
#define EVAL_MATCH_H

#include "Evaluator.h"


Variable Evaluator::match(Tree* tree, Datatable* data) {
  const Variable& value = evaluate(tree->children.at(0), data);
  Tree* cases = tree->subTree(1)->subTree(0)->subTree(0);
  Variable::VarList errors;
  for (unsigned int i = 0; i < cases->children.size(); ++i) {
    Tree* cas = cases->subTree(i);
    const Variable& test = evaluate(cas->children.at(0), data);
    if (test.type == Variable::NODE) {
      Variable ok = executeAny(test, data, value);
      if (ok.toBool()) {
        return evaluate(cas->children.at(1), data);
      }
      else if (ok.isError())
        errors.push_back(ok);
    } else {
      if (value.equals(test)) {
        return evaluate(cas->children.at(1), data);
      }
    }
  }
  return Variable::error(
    std::string("Did not match any option")+tree->pos,
    errors);
}

#endif