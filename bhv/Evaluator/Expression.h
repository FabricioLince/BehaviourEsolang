#ifndef EVAL_EXPRESSION_H
#define EVAL_EXPRESSION_H

#include "Evaluator.h"
#include <iostream>
#include <string>
#include <cmath>

Variable Evaluator::integer(Node* node, Datatable* data) {
  return Variable(std::stoi(node->asToken()->string));
}

Variable Evaluator::decimal(Node* node, Datatable* data) {
  return Variable(stold(node->asToken()->string));
}

Variable Evaluator::unary(Tree* tree, Datatable* data) {
  const Variable& value = evaluate(tree->children.at(1), data);
  const std::string& symbol = tree->getToken(0)->string;
  if (symbol == "~") {
    if (value.type == Variable::NIL) {
      return true;
    }
    else if (value.type == Variable::NODE or value.type == Variable::CFUNC) {
      Variable n = value;
      n.invert = !n.invert;
      return n;
    }
    else {
      return !value.toBool();
    }
  }
  else if (symbol == "-") {
    return value * -1;
  }
  else if (symbol == "#") {
    return value.length();
  }

  return value;
}

Variable Evaluator::var(Node* node, Datatable* data) {
  Variable r = data->get(node->asToken()->string);
  if (r.isError()) {
    r.string += node->pos;
  }
  return r;
}

Variable Evaluator::addition(Tree* tree, Datatable* data) {
  Variable value = evaluate(tree->children.at(0), data);
  
  if (tree->children.size() > 1) {
    for (Node* op : tree->subTree(1)->children) {
      
      if (value.type == Variable::NIL) {
        return value;
      }
      
      Tree* top = op->asTree();
      const std::string& symbol = top->getToken(0)->string;
      
      const Variable& other = evaluate(top->children.at(1), data);
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

Variable Evaluator::multiplication(Tree* tree, Datatable* data) {
  Variable value = evaluate(tree->children.at(0), data);
  if (tree->children.size() > 1) {
    for (Node* op : tree->subTree(1)->children) {
      
      if (value.type == Variable::NIL)
        return value;
      
      Tree* top = op->asTree();
      const std::string& symbol = top->getToken(0)->string;
      const Variable& other = evaluate(top->children.at(1), data);
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
        else if (value.type == Variable::LIST && 
          (other.type == Variable::NODE 
          || other.type == Variable::CFUNC)) {
          value = applyTreeOnList(value, other, data);
        }
        else if (value.type == Variable::STRING && 
          (other.type == Variable::NODE
          || other.type == Variable::CFUNC)) {
          value = applyTreeOnString(value, other, data);
        }
        else {
          value = value * other;
        }
      }
      else if (symbol == "/") {
        if (value.type == Variable::NUMBER) {
          switch (other.type) {
            case Variable::NODE:
            case Variable::CFUNC:
            {
              Variable::VarList l;
              for (int i = 0; i < value.number; ++i) {
                Variable r = executeAny(other, data, i);
                if (r.toBool())
                  l.push_back(i);
              }
              value = l;
              break;
            }
            default:
              value = value / other;
              break;
          }
        }
        else if (value.type == Variable::STRING) {
          switch (other.type) {
            case Variable::NODE:
            case Variable::CFUNC:
            {
              Variable::VarList list = Variable::VarList();
              std::string item = "";
              for (unsigned int i = 0; i < value.string.size(); ++i) {
                std::string c = std::string(1, value.string.at(i));
                const Variable& r = executeAny(other, data, c);
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
              int left = 0;
              int right = value.string.find(other.string);
              list.push_back(value.string.substr(left, right-left));
              while (right != std::string::npos) {
                left = right + other.string.size();
                right = value.string.find(other.string, left+1);
                std::string sub = value.string.substr(left, right-left);
                if (sub.size() > 0)
                  list.push_back(sub);
              }
              value = list;
              break;
            }
            default:
              value = value / other;
          }
        }
        else if (value.type == Variable::LIST) {
          switch (other.type) {
            case Variable::NODE:
            case Variable::CFUNC:
            {
              Variable rlist = Variable::VarList();
              //std::cout << r.type << "\n";
              for (const Variable& v : value.list) {
                Variable r = executeAny(other, data, v);
                if (r.toBool())
                  rlist.list.push_back(v);
              }
              value = rlist;
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
        else
          return Variable::errorInvalidOp("^", value, other);
      }
    }
  }
  return value;
}

Variable Evaluator::doCompare(Tree* tree, Datatable* data) {
  Variable value = evaluate(tree->children.at(0), data);
  Tree* comp = tree->subTree("comp");
  if (comp) {
    const std::string& symbol = comp->getToken("comparation")->string;
    const Variable& other = evaluate(comp->children.at(1), data);
    if (symbol == ">") {
      if (value.type == Variable::LIST && (other.type == Variable::NODE or other.type == Variable::CFUNC)) {
        if (value.list.size() == 0) {
          return Variable::error("Reduced empty list");
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
        for (const Variable& v : value.list) {
          if (executeAny(other, data, v).toBool()) {
            return index;
          }
          index++;
        }
        return Variable::error("Truthy value not found in list");
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
    else if (symbol == "..") {
      if (value.type == Variable::NUMBER && other.type == Variable::NUMBER) {
        Variable::VarList l;
        if (value.number < other.number) {
          for (int i = int(value.number); i <= int(other.number); ++i) {
            l.push_back(i);
          }
        }
        else {
          for (int i = int(value.number); i >= int(other.number); --i) {
            l.push_back(i);
          }
        }
        return l;
      }
      else {
        return Variable::errorInvalidOp("..", value, other);
      }
    }
    else if (symbol == "|>") {
      const Variable& comp = value > other;
      if (comp.isError()) return comp;
      if (comp.toBool())
        return value;
      return other;
    }
    else if (symbol == "|<") {
      const Variable& comp = value < other;
      if (comp.isError()) return comp;
      if (comp.toBool())
        return value;
      return other;
    }
    
  }
  return value;
}

Variable Evaluator::comparation(Tree* tree, Datatable* data) {
  const Variable& value = doCompare(tree, data);
  Tree* postop = tree->subTree("postop");
  if (postop) {
    const std::string& symbol = postop->getToken("reduce")->string;
    if (value.type == Variable::LIST) {
      if (value.list.size() == 0) {
        return Variable::error("Can not reduce empty list");
      }
      if (symbol == ">+") {
        Variable sum = value.list.at(0);
        for (unsigned int i = 1; i < value.list.size(); ++i) {
          sum = sum + value.list.at(i);
        }
        return sum;
      }
      
      if (symbol == ">*") {
        Variable sum = value.list.at(0);
        for (unsigned int i = 1; i < value.list.size(); ++i) {
          sum = sum * value.list.at(i);
        }
        return sum;
      }
      
      if (symbol == ">>") {
        Variable chosen = value.list.at(0);
        for (unsigned int i = 1; i < value.list.size(); ++i) {
          const Variable& comp = value.list.at(i) > chosen;
          if (comp.isError()) return comp;
          if (comp.toBool())
            chosen = value.list.at(i);
        }
        return chosen;
      }
      
      if (symbol == "><") {
        Variable chosen = value.list.at(0);
        for (unsigned int i = 1; i < value.list.size(); ++i) {
          const Variable& comp = value.list.at(i) < chosen;
          if (comp.isError()) return comp;
          if (comp.toBool())
            chosen = value.list.at(i);
        }
        return chosen;
      }
      
    }
    else {
      return Variable::error(
        std::string("Can not reduce ") + Variable::typeName(value), 
        value
      );
    }   
  }
  return value;
}

Variable Evaluator::applyTreeOnList(const Variable& list, const Variable& tree, Datatable* data) {
  //std::cout << "applying " << tree << " on " << list << "\n";
  Variable::VarList rlist;
  //std::cout << r.type << "\n";
  for (unsigned int i = 0; i < list.list.size(); ++i) {
    rlist.push_back(
      executeAny(tree, data, list.list.at(i), int(i))
    );
  }
  return rlist;
}

Variable Evaluator::applyTreeOnString(const Variable& string, const Variable& tree, Datatable* data) {
  //std::cout << "applying " << tree << " on " << list << "\n";
  Variable::VarList list;
  //std::cout << r.type << "\n";
  for (unsigned int i = 0; i < string.string.size(); ++i) {
    list.push_back(
      executeAny(tree, data, std::string(1, string.string.at(i)), int(i))
    );
  }
  return list;
}


#endif