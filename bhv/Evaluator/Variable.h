#ifndef EVALRESULT_H
#define EVALRESULT_H

#include <ostream>
#include <vector>

#include "../../Parser/Tree.h"

class Variable {
  public:
    static const int NIL = 0;
    static const int NUMBER = 1;
    static const int STRING = 2;
    static const int BOOL = 3;
    static const int NODE = 4;
    static const int LIST = 5;
    static const int CFUNC  = 6;
    
    typedef std::vector<Variable> VarList;
    Variable() {
      this->type = NIL;
    }
    Variable(int integer){
      this->number = integer;
      this->type = NUMBER;
    }
    Variable(long double db){
      this->number = db;
      this->type = NUMBER;
    }
    Variable(std::string string) {
      this->string = string;
      this->type = STRING;
    }
    Variable(const char* str) {
      this->string = str;
      this->type = STRING;
    }
    Variable(bool b) {
      this->number = b;
      this->type = BOOL;
    }
    Variable(Parser::Node* node, Datatable* context) {
      if (node) {
        this->node = node;
        this->type = NODE;
        this->context = context;
      }
      else {
        this->type = BOOL;
        this->number = false;
      }
    }
    Variable(VarList list) {
      this->type = LIST;
      this->list = list;
    }
    Variable(Variable (*func)(Datatable*), Datatable* context = NULL) {
      this->cfunc = func;
      this->type = CFUNC;
      this->context = context;
    }
    bool toBool() const {
      if (this->type == BOOL) {
        return number != 0;
      }
      if (this->type == NIL) {
        return false;
      }
      return true;
    }
    std::string toString() const {
      std::string s;
      switch (type) {
        case Variable::NIL:
          return "nil";
        case Variable::NUMBER:
          s = (std::to_string(number));
          if (s.find('.')) {
            while (s.back() == '0') {
              s.replace(s.size()-1, 1, "");
            }
            if (s.back() == '.') {
              s.replace(s.size()-1, 1, "");
            }
          }
          return s;
        case Variable::STRING:
          return string;
        case Variable::BOOL:
          return (number?"true":"false");
        case Variable::NODE:
          return std::string("Node:") + node->name;
        case Variable::LIST:
          s = "{";
          for (int i = 0; i < list.size(); ++i) {
            if (list.at(i).type == STRING)
              s += "\"";
            s += list.at(i).toString();
            if (list.at(i).type == STRING)
              s += "\"";
            if (i < list.size()-1) 
              s += " ";
          }
          s += "}";
          return s;
        case Variable::CFUNC:
          return std::string("CFUNC");
      }
      return ("");
    }

    Variable operator+(const Variable& other) {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number + other.number);
      if (type == LIST) {
        list.push_back(other);
        return *this;
      }
      if (type == STRING || other.type == STRING)
        return Variable(toString() + other.toString());
      return Variable();
    }

    Variable operator-(const Variable& other) {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number - other.number);
      if (type == STRING) {
        int index;
        switch (other.type) {
          case NUMBER:
            index = int(other.number);
            if (index >= 0 && index < string.size()) {
              string.erase(index, 1);
            }
            return *this;
            break;
          case STRING:
            index = string.find(other.string);
            if (index >= 0 && index < string.size()) {
              string.erase(index, other.string.size());
            }
            return *this;
            break;
        }
      }
      if (type == LIST) {
        if (other.type == NUMBER) {
          int index = int(other.number);
          if (index < 0) {
            index = list.size() + index;
          }
          if (index >= 0 && index < list.size()) {
            list.erase(list.begin()+index);
          }
          return *this;
          
        }
      }
      return Variable();
    }

    Variable operator*(const Variable& other) {
      if (type == NUMBER) {
        if (other.type == NUMBER) {
          return Variable(number * other.number);
        }
      }
      if (type == STRING && other.type == NUMBER) {
        std::string s = "";
        for (int i = 0; i < other.number; ++i) {
          s += string;
        }
        return Variable(s);
      }
      if (type == BOOL && other.type == NUMBER && other.number == -1) {
        return !toBool();
      }
      if (type == LIST) {
        if (other.type == LIST) {
          for (Variable v : other.list) {
            list.push_back(v);
          }
        }
        else if (other.type == NUMBER) {
          VarList l;
          for (int i = 0; i < other.number; ++i) {
            l.insert(l.begin(), list.begin(), list.end());
          }
          return l;
        }
        return *this;
      }
      return Variable();
    }

    Variable operator/(const Variable& other) {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number / other.number);
      if (type == LIST && other.type == NUMBER) {
        Variable r = VarList();
        if (other.number > 0) {
          for (int i = 0; i < other.number; ++i) {
            if (i < list.size())
              r.list.push_back(list.at(i));
          }
        }
        else if(other.number < 0) {
          for (int i = 0; i < -other.number; ++i) {
            int j = (list.size()-1-i);
            if (j >= 0)
              r.list.insert(r.list.begin(), list.at(j));
          }
        }
        return r;
      }
      if (type == STRING && other.type == NUMBER) {
        Variable r = std::string("");
        if (other.number > 0) {
          for (int i = 0; i < other.number; ++i) {
            if (i < string.size())
              r.string += string.at(i);
          }
        }
        else if(other.number < 0) {
          for (int i = 0; i < -other.number; ++i) {
            int j = (string.size()-1-i);
            if (j >= 0)
              r.string  = string.at(j) + r.string;
          }
        }
        return r;
      }
        
      return Variable();
    }

    Variable operator%(const Variable& other) {
      if (type == NUMBER && other.type == NUMBER) {
        int m = int(other.number);
        if (m != 0) {
          return Variable(int(number) % m);
        }
        return Variable();
      } 
      //printf("%s %% %s\n", toString().c_str(), other.toString().c_str());
      if (type == STRING) {
        if (other.type == NUMBER) {
          int index = int(other.number);
          if (index < 0) {
            index = string.size() + index;
          }
          if (index >= 0 && index < string.size()) {
            return std::string(1, string.at(index));
          }
        }
        else if (other.type == STRING) {
          int index = string.find(other.string);
          if (index == std::string::npos) {
            return Variable();
          }
          return index;
        }
      }
      if (type == LIST) {
        if (other.type == NUMBER) {
          int index = int(other.number);
          if (index < 0) {
            index = list.size() + index;
          }
          if (index >= 0 && index < list.size()) {
            return list.at(index);
          }
        }
      }
      return Variable();
    }

    Variable operator>(const Variable& other) {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number > other.number);
      if (type == STRING && other.type == STRING) {
        return string.compare(other.string) > 0;
      }
      return Variable();
    }
    Variable operator<(const Variable& other) {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number < other.number);
      if (type == STRING && other.type == STRING) {
        return string.compare(other.string) < 0;
      }
      return Variable();
    }
    Variable operator>=(const Variable& other) {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number >= other.number);
      if (type == STRING && other.type == STRING) {
        return string.compare(other.string) >= 0;
      }
      return Variable();
    }
    Variable operator<=(const Variable& other) {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number <= other.number);
      if (type == STRING && other.type == STRING) {
        return string.compare(other.string) <= 0;
      }
      return Variable();
    }
    Variable operator==(const Variable& other) {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number == other.number);
      if (type == STRING && other.type == STRING)
        return Variable(string == other.string);
      if (type == BOOL && other.type == BOOL)
        return Variable(number == other.number);
      if (type == LIST && other.type == LIST) {
        if (list.size() != other.list.size())
          return false;
        for (int i = 0; i < list.size(); ++i) {
          if (!(list.at(i) == other.list.at(i)).toBool()) {
            return false;
          }
        }
        return true;
      }
      if (type == NODE && other.type == NODE) {
        return node == other.node;
      }
      if (type == CFUNC && other.type == CFUNC) {
        return cfunc == other.cfunc;
      }
      return Variable(false);
    }
    Variable operator!=(const Variable& other) {
      return !(operator==(other).toBool());
    }

    int length() {
      switch (type) {
        case Variable::NIL:
          return 0;
        case Variable::NUMBER:
          return int(number);
        case Variable::STRING:
          return string.size();
        case Variable::BOOL:
          return 1;
        case Variable::NODE:
          return 0;
        case Variable::LIST:
          return list.size();
         case Variable::CFUNC:
          return 0;
      }
      return 0;
    }

    int type = 0;
    long double number = 0;
    std::string string;
    Parser::Node* node;
    Datatable* context; // creation context for the node/function
    VarList list;
    Variable (*cfunc)(Datatable*);
    

};

std::ostream& operator<<(std::ostream& out, Variable& r) {
  return out << r.toString();
}

#endif