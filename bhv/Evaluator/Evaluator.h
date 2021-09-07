#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "../../Parser/Tree.h"

#include "Variable.h"
#include "Datatable.h"
#include <map>
#include <fstream>
#include <iostream>

typedef Parser::Node Node;
typedef Parser::Tree Tree;

class Evaluator {
  public:
    Variable sequence(Node*node, Datatable* data);
    Variable select(Node*node, Datatable* data);
    Variable repeat(Node*node, Datatable* data);
    Variable negate(Node*node, Datatable* data);
    Variable optional(Node*node, Datatable* data);
    Variable assign(Node*node, Datatable* dataFrom, Datatable* dataTo);
    Variable expression(Node* node, Datatable* data);
    Variable comparation(Node* node, Datatable* data);
    Variable addition(Node* node, Datatable* data);
    Variable multiplication(Node* node, Datatable* data);
    Variable integer(Node* node, Datatable* data);
    Variable decimal(Tree* tree, Datatable* data);
    Variable var(Node* node, Datatable* data);
    Variable print(Node* node, Datatable* data);
    Variable test(Node* node, Datatable* data);
    Variable main(Tree* node, Datatable* data);
    Variable list(Tree* tree, Datatable* data);
    Variable unary(Tree* tree, Datatable* data);
    Variable execute(Tree* tree, Datatable* data);
    Variable executeChildTree(Node* tree, Datatable* data, Variable arg = Variable());
    void evaluateArgs(Tree* tree, Datatable* data, Datatable* childData);
    void evaluateArg(Node* arg, Datatable* data, Datatable* childData);
    
    Variable loadstring(Variable str, Datatable* data);
    Variable getTree(Tree* tree, Datatable* data);
    Variable applyTreeOnList(Variable list, Variable tree, Datatable* data);
    Variable applyTreeOnString(Variable string, Variable tree, Datatable* data);
    Variable applyCFuncOnList(Variable list, Variable cfunc, Datatable* data);

    Variable string(Node* node, Datatable* data) {
      std::string str = node->asToken()->string;
      int pos = str.find("\\n");
      while (pos != std::string::npos) {
        str.replace(pos, 2, "\n");
        pos = str.find("\\n");
      }
      return Variable(str);
    }
    Variable evaluate(Node* node, Datatable* data) {
      if (node->name == "sequence") {
        return sequence(node, data);
      }
      if (node->name == "select") {
        return select(node, data);
      }
      if (node->name == "assign") {
        return assign(node, data, data);
      }
      if (node->name == "integer") {
        return integer(node, data);
      }
      if (node->name == "decimal") {
        return decimal(node->asTree(), data);
      }
      if (node->name == "addition") {
        return addition(node, data);
      }
      if (node->name == "multiplication") {
        return multiplication(node, data);
      }
      if (node->name == "print") {
        return print(node, data);
      }
      if (node->name == "string") {
        return string(node, data);
      }
      if (node->name == "var") {
        return var(node, data);
      }
      if (node->name == "test") {
        return test(node, data);
      }
      if (node->name == "comparation") {
        return comparation(node, data);
      }
      if (node->name == "repeat") {
        return repeat(node, data);
      }
      if (node->name == "negate") {
        return negate(node, data);
      }
      if (node->name == "main") {
        return main(node->asTree(), data);
      }
      if (node->name == "list") {
        return list(node->asTree(), data);
      }
      if (node->name == "unary") {
        return unary(node->asTree(), data);
      }
      if (node->name == "execute") {
        return execute(node->asTree(), data);
      }
      if (node->name == "optional") {
        return optional(node, data);
      }
      if (node->name == "getTree") {
        return getTree(node->asTree(), data);
      }

      printf("no evaluator for <%s>\n", node->name.c_str());
      return Variable();
    }
};

Variable Evaluator::main(Tree* tree, Datatable* data) {
  //std::cout << tree->children.at(0)->name << "\n";
  
  Tree* exprs = tree->subTree(0);
  Variable r;
  for (int i=0; i < exprs->children.size(); ++i) {
    //std::cout << cmds->children.at(i)->name << "\n";
    r = evaluate(exprs->children.at(i), data);
  }
  return r;
}

Variable Evaluator::list(Tree* tree, Datatable* data) {
  std::vector<Variable> v;
  Tree* values = tree->children.at(0)->asTree();
  for (int i = 0; i < values->children.size(); ++i) {
    v.push_back(evaluate(values->children.at(i), data));
  }
  return Variable(v);
}


#endif