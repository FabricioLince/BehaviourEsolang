#ifndef EVAL_MATCH_H
#define EVAL_MATCH_H

#include "Evaluator.h"


Variable Evaluator::match(Tree* tree, Datatable* data) {
  Variable value = evaluate(tree->children.at(0), data);
  Tree* cases = tree->subTree(1)->subTree(0)->subTree(0);
  for (unsigned int i = 0; i < cases->children.size(); ++i) {
    Tree* cas = cases->subTree(i);
    Variable test = evaluate(cas->children.at(0), data);
    if (test.type == Variable::NODE) {
      Variable ok = executeAny(test, data, value);
      if (ok.toBool()) {
        return evaluate(cas->children.at(1), data);
      }
    } else {
      if (value.equals(test)) {
        return evaluate(cas->children.at(1), data);
      }
    }
  }
  return Variable();
}

#endif