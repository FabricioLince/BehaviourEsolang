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
  
  Variable main(Tree* node, Datatable* data);
  
  Variable sequence(Tree* tree, Datatable* data);
  Variable select(Tree* tree, Datatable* data);
  Variable repeat(Tree* tree, Datatable* data);
  Variable ifcond(Tree* tree, Datatable* data);
  Variable match(Tree* tree, Datatable* data);
  
  Variable negate(Tree* tree, Datatable* data);
  Variable optional(Tree* tree, Datatable* data);
  Variable print(Tree* tree, Datatable* data);
  
  Variable assign(Node*node, Datatable* dataFrom, Datatable* dataTo);
  
  Variable comparation(Tree* tree, Datatable* data);
  Variable doCompare(Tree* tree, Datatable* data);
  Variable addition(Tree* tree, Datatable* data);
  Variable multiplication(Tree* tree, Datatable* data);
  Variable unary(Tree* tree, Datatable* data);
  Variable integer(Node* node, Datatable* data);
  Variable decimal(Node* node, Datatable* data);
  Variable var(Node* node, Datatable* data);
  Variable list(Tree* tree, Datatable* data);
  Variable string(Node* node, Datatable* data);
  Variable tuple(Tree* tree, Datatable* data);
  Variable var_acc(Tree* tree, Datatable* data);
  
  Variable execute(Tree* tree, Datatable* data);
  Variable getTree(Tree* tree, Datatable* data);
  
  void evaluateArgs(Tree* tree, Datatable* data, Datatable* childData);
  void evaluateArg(Node* arg, Datatable* data, Datatable* childData);
  
  
  Variable applyTreeOnList(const Variable& list, const Variable& tree, Datatable* data);
  Variable applyTreeOnString(const Variable& string, const Variable& tree, Datatable* data);
  
  Variable executeChildTree(Node* tree, Datatable* data);
  Variable executeCFunc(const Variable& cfunc, Datatable* data);
  Variable executeAny(const Variable& nodeOrFunc, Datatable* data, Variable arg = Variable(), Variable b = Variable());
  Variable justExecute(const Variable& nodeOrFunc, Datatable* data);
  
  
  std::hash<std::string> hasher;
  const int assign_id = hasher("assign");
  const int print_id = hasher("print");
  const int optional_id = hasher("optional");
  
  std::map<int, Variable(Evaluator::*)(Node*, Datatable*)> evaluatorsNode;
  std::map<int, Variable(Evaluator::*)(Tree*, Datatable*)> evaluatorsTree;
  
  public:
    
    bool printLineNumber = true;
  
    Evaluator() {
      evaluatorsTree[hasher("main")] = &Evaluator::main;
      
      evaluatorsTree[hasher("sequence")] = &Evaluator::sequence;
      evaluatorsTree[hasher("select")] = &Evaluator::select;
      evaluatorsTree[hasher("repeat")] = &Evaluator::repeat;
      evaluatorsTree[hasher("ifcond")] = &Evaluator::ifcond;
      evaluatorsTree[hasher("match")] = &Evaluator::match;
      
      evaluatorsTree[hasher("print")] = &Evaluator::print;
      evaluatorsTree[hasher("negate")] = &Evaluator::negate;
      evaluatorsTree[hasher("optional")] = &Evaluator::optional;
      evaluatorsTree[hasher("unary")] = &Evaluator::unary;
      
      evaluatorsTree[hasher("comparation")] = &Evaluator::comparation;
      evaluatorsTree[hasher("addition")] = &Evaluator::addition;
      evaluatorsTree[hasher("multiplication")] = &Evaluator::multiplication;
      evaluatorsNode[hasher("integer")] = &Evaluator::integer;
      evaluatorsNode[hasher("decimal")] = &Evaluator::decimal;
      
      evaluatorsNode[hasher("string")] = &Evaluator::string;
      evaluatorsNode[hasher("word")] = &Evaluator::var;
      evaluatorsTree[hasher("list")] = &Evaluator::list;
      evaluatorsTree[hasher("tuple")] = &Evaluator::tuple;
      evaluatorsTree[hasher("var_acc")] = &Evaluator::var_acc;
      
      evaluatorsTree[hasher("execute")] = &Evaluator::execute;
      evaluatorsTree[hasher("getTree")] = &Evaluator::getTree;
    }
    
    Variable evaluate(Node* node, Datatable* data) {
      //std::cout << "evaluating:" << node << std::endl;
      
      if (evaluatorsNode.count(node->id)) {
        return (this->*evaluatorsNode[node->id])(node, data);
      }
      else if (evaluatorsTree.count(node->id)) {
        return (this->*evaluatorsTree[node->id])(node->asTree(), data);
      }
      
      else if (node->id == assign_id) {
        return assign(node, data, data);
      }
      
      //printf("no evaluator for <%s>\n", node->name.c_str());
      return Variable::error(
        std::string("Runtime Error: No evaluator for ")
        + node->name + node->pos);
    }
};

Variable Evaluator::main(Tree* tree, Datatable* data) {
  //std::cout << tree->children.at(0)->name << "\n";
  
  Tree* exprs = tree->subTree(0);
  Variable r;
  for (unsigned int i = 0; i < exprs->children.size(); ++i) {
    //std::cout << cmds->children.at(i)->name << "\n";
    r = evaluate(exprs->children.at(i), data);
  }
  return r;
}

Variable Evaluator::list(Tree* tree, Datatable* data) {
  std::vector<Variable> v;
  Tree* values = tree->subTree(0);
  for (unsigned int i = 0; i < values->children.size(); ++i) {
    v.push_back(evaluate(values->children.at(i), data));
  }
  return Variable(v);
}

Variable Evaluator::string(Node* node, Datatable* data) {
  std::string str = node->asToken()->capture;
  int pos = str.find("\\n");
  while (pos != std::string::npos) {
    str.replace(pos, 2, "\n");
    pos = str.find("\\n");
  }
  pos = str.find("\\t");
  while (pos != std::string::npos) {
    str.replace(pos, 2, "\t");
    pos = str.find("\\t");
  }
  return Variable(str.substr(1, str.length()-2));
}

#endif