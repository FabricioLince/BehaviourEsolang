#ifndef EVAL_EXPRESSION_H
#define EVAL_EXPRESSION_H

#include "Evaluator.h"
#include <iostream>
#include <string>
#include <cmath>

Variable Evaluator::integer(Node* node, Datatable* data) {
  return  Variable(std::stoi(node->asToken()->string));
}

Variable Evaluator::decimal(Tree* tree, Datatable* data) {
  Variable i = integer(tree->children.at(0), data);
  Variable d = integer(tree->children.at(1), data);
  double e = 1;
  for (unsigned int i = 0; i < tree->getToken(1)->string.size(); ++i) {
    e *= 10;
  }
  return i.number + d.number/e;
}

Variable Evaluator::unary(Tree* tree, Datatable* data) {
  Variable value = evaluate(tree->children.at(1), data);
  std::string symbol = tree->getToken(0)->string;
  if (symbol == "~") {
    if (value.type == Variable::NIL) {
      value = true;
    }
    else if (value.type == Variable::NODE or value.type == Variable::CFUNC) {
      value.invert = !value.invert;
    }
    else {
      value = !value.toBool();
    }
  }
  else if (symbol == "-") {
    value = value * -1;
  }
  else if (symbol == "#") {
    value = value.length();
  }

  return value;
}

Variable Evaluator::var(Node* node, Datatable* data) {
  Variable value = data->get(node->asToken()->string);
  return value;
}

Variable Evaluator::addition(Node* node, Datatable* data) {
  Tree* tree = node->asTree();
  Variable value = evaluate(tree->children.at(0), data);
  Node* ops = tree->get("ops");
  if (ops) {
    for (Node* op : ops->asTree()->children) {
      Tree* top = op->asTree();
      std::string symbol = top->getToken("symbol")->string;
      Variable other = evaluate(top->children.at(1), data);
      if (symbol == "+") {
        value = value + other;
      }
      else if (symbol == "-") {
        value = value - other;
      }
    }
  }
  return value;
}

Variable Evaluator::multiplication(Node* node, Datatable* data) {
  Tree* tree = dynamic_cast<Tree*>(node);
  Variable value = evaluate(tree->children.at(0), data);
  Tree* ops = tree->subTree("ops");
  if (ops) {
    for (Node* op : ops->children) {
      Tree* top = op->asTree();
      std::string symbol = top->getToken("symbol")->string;
      Variable other = evaluate(top->children.at(1), data);
      if (symbol == "*") {
        if (value.type == Variable::NUMBER) {
          switch (other.type) {
            case Variable::NODE:
            case Variable::CFUNC:
            {
              Variable::VarList l;
              for (int i = 0; i < value.number; ++i) {
                l.push_back(executeAny(other, data, i));
              }
              value = l;
              break;
            }
            default:
              value = value * other;
              break;
          }
        }
        else if (value.type == Variable::LIST && other.type == Variable::NODE) {
          value = applyTreeOnList(value, other, data);
        }
        else if (value.type == Variable::STRING && other.type == Variable::NODE) {
          value = applyTreeOnString(value, other, data);
        }
        else if (value.type == Variable::LIST && other.type == Variable::CFUNC) {
          value = applyCFuncOnList(value, other, data);
        }
        else if (value.type == Variable::STRING && other.type == Variable::CFUNC) {
          value = applyTreeOnString(value, other, data);
        }
        else {
          value = value * other;
        }
      }
      else if (symbol == "/") {
        if (value.type == Variable::STRING) {
          switch (other.type) {
            case Variable::NODE:
            case Variable::CFUNC:
            {
              Variable::VarList list = Variable::VarList();
              std::string item = "";
              for (unsigned int i = 0; i < value.string.size(); ++i) {
                std::string c = std::string(1, value.string.at(i));
                Variable r = executeAny(other, data, c);
                if (r.toBool()) {
                  if (item.size() > 0) {
                    list.push_back(item);
                    item = "";
                  }
                }
                else {
                  item += c;
                }
              }
              if (item.size() > 0) {
                list.push_back(item);
              }
              value = list;
              break;
            }
            case Variable::STRING:
            {
              Variable::VarList list = Variable::VarList();
              std::string item = "";
              for (unsigned int i = 0; i < value.string.size(); ++i) {
                std::string c = std::string(1, value.string.at(i));
                if (c == other.string) {
                  if (item.size() > 0) {
                    list.push_back(item);
                    item = "";
                  }
                }
                else {
                  item += c;
                }
              }
              if (item.size() > 0) {
                list.push_back(item);
              }
              value = list;
              break;
            }
            default:
              value = value / other;
          }
        }
        else {
          value = value / other;
        }
      }
      else if (symbol == "%") {
        value = value % other;
      }
      else if (symbol == "^") {
        if (value.type == Variable::NUMBER && other.type == Variable::NUMBER) {
          value = static_cast<long double>(pow(value.number, other.number));
        }
      }
    }
  }
  return value;
}


Variable Evaluator::comparation(Node* node, Datatable* data) {
  Tree* tree = node->asTree();
  Variable value = evaluate(tree->children.at(0), data);
  Tree* comp = tree->subTree("comp");
  if (comp) {
    std::string symbol = comp->getToken("symbol")->string;
    Variable other = evaluate(comp->children.at(1), data);
    if (symbol == ">") {
      if (value.type == Variable::LIST && (other.type == Variable::NODE or other.type == Variable::CFUNC)) {
        if (value.list.size() == 0) {
          return Variable();
        }
        Variable r = value.list.at(0);
        for (unsigned int i = 1; i < value.list.size(); ++i) {
          r = executeAny(other, data, r, value.list.at(i));
        }
        return r;
      }
      return value > other;
    }
    else if (symbol == "<") {
      if (value.type == Variable::LIST && (other.type == Variable::NODE or other.type == Variable::CFUNC)) {
        int index = 0;
        for (Variable v : value.list) {
          if (executeAny(other, data, v).toBool()) {
            return index;
          }
          index++;
        }
        return Variable();
      }
      return value < other;
    }
    else if (symbol == ">=") {
      return value >= other;
    }
    else if (symbol == "<=") {
      return value <= other;
    }
    else if (symbol == "==") {
      return value == other;
    }
    else if (symbol == "~=") {
      return value != other;
    }
    else if (symbol == "|") {
      int count = 999999;
      while ((other != value).toBool()) {
        if (count-- < -999999) {
          return false;
        }
        if (value.type == Variable::NIL || other.type == Variable::NIL) {
          // if any expr returns NIL the comparation should stop
          return Variable();
        }
        
        value = evaluate(tree->children.at(0), data);
        other = evaluate(comp->children.at(1), data);
        //std::cout << value << " != " << other << "\n";
        //std::cout << comp->children.at(1) << "\n";
        
      }
      return true;
    }
  }
  return value;
}

Variable Evaluator::applyTreeOnList(Variable list, Variable tree, Datatable* data) {
  //std::cout << "applying " << tree << " on " << list << "\n";
  Variable rlist = (Variable::VarList());
  //std::cout << r.type << "\n";
  for (Variable v : list.list) {
    Variable r = executeAny(tree, data, v);
    if (r.type != Variable::NIL) 
      rlist.list.push_back(r);
  }
  return rlist;
}

Variable Evaluator::applyTreeOnString(Variable string, Variable tree, Datatable* data) {
  //std::cout << "applying " << tree << " on " << list << "\n";
  Variable::VarList list;
  //std::cout << r.type << "\n";
  for (unsigned int i = 0; i < string.string.size(); ++i) {
    list.push_back(executeAny(tree, data, std::string(1, string.string.at(i))));
  }
  return list;
}

Variable Evaluator::applyCFuncOnList(Variable list, Variable cfunc, Datatable* data) {
  Variable rlist = (Variable::VarList());
  //std::cout << r.type << "\n";
  
  for (Variable v : list.list) {
    Variable r = executeAny(cfunc, data, v);
    if (r.type != Variable::NIL) 
      rlist.list.push_back(r);
  }
  
  return rlist;
}

#endif