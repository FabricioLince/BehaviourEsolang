  #ifndef EVALRESULT_H
#define EVALRESULT_H

#include <ostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>

#include "../../Parser/Tree.h"

class Variable {
  public:
    static const int NIL = 0;
    static const int NUMBER = 1;
    static const int STRING = 2;
    static const int BOOL = 3;
    static const int NODE = 4;
    static const int LIST = 5;
    static const int CFUNC = 6;
    static const int TUPLE = 7;
    
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
    Variable(const std::string& string) {
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
    Variable(const VarList& list) {
      this->type = LIST;
      this->list = list;
    }
    Variable(Variable (*func)(Datatable*), Datatable* context = NULL) {
      this->cfunc = func;
      this->type = CFUNC;
      this->context = context;
    }

    Variable(Datatable* table) {
      this->type = TUPLE;
      this->context = table;
    }
    
    /// copy constructor
    Variable (const Variable& other) {
      type = other.type;
      switch (type) {
        case NIL:
          string = other.string;
          break;
        case NUMBER:
          number = other.number;
          break;
        case BOOL:
          number = other.number;
          break;
        case STRING:
          string = other.string;
          break;
        case LIST:
          list = other.list;
          break;
        case NODE:
          node = other.node;
          context = other.context;
          invert = other.invert;
          break;
        case CFUNC:
          cfunc = other.cfunc;
          context = other.context;
          invert = other.invert;
          break;
        case TUPLE:
          context = other.context;
          break;
      }
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
      if (isError()) {
        return std::string("error:") + string;
      }
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
        {
          std::stringstream ss;
          ss << ("Node:");
          if (context == NULL) {
            ss << "orphan";
          }
          else {
            ss << "full";
          }
          ss << "@0x" << std::hex << reinterpret_cast<uintptr_t>(node);
          return ss.str();
        }
        case Variable::LIST:
          s = "{";
          for (unsigned int i = 0; i < list.size(); ++i) {
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
        case Variable::TUPLE:
          {
            s = "{";
            std::stringstream ss;
            ss << context;
            std::string str;
            ss >> str;
            s += str + "}";
            return s;
          }
      }
      return ("");
    }
 
    Variable errorInvalidOp(const char* opSymbol, const Variable& other) const {
     return errorInvalidOp(opSymbol, *this, other);
    }

    Variable operator+(const Variable& other) const {
      switch (type) {
        case NUMBER:
          if (other.type == NUMBER)
            return Variable(number + other.number);
          break;
        case LIST:
        {
          Variable r = VarList(list);
          r.list.push_back(other);
          return r;
        }
        case STRING:
          return Variable(toString() + other.toString());
      }
      if (other.type == STRING)
        return Variable(toString() + other.toString());
      return errorInvalidOp("+", other);
    }

    Variable operator-(const Variable& other) const {
      switch (type) {
        case NUMBER: {
          if (other.type == NUMBER)
            return Variable(number - other.number);
          break;
        }
        case STRING: {
          int index;
          switch (other.type) {
            case NUMBER:
              index = int(other.number);
              if (index >= 0 && index < int(string.size())) {
                Variable r = string;
                r.string.erase(index, 1);
                return r;
              }
              return *this;
              break;
            case STRING:
              index = string.find(other.string);
              if (index >= 0 && index < int(string.size())) {
                Variable r = string;
                r.string.erase(index, other.string.size());
                return r;
              }
              return *this;
              break;
          }
          break;
        }
        case LIST: {
          if (other.type == NUMBER) {
            int index = int(other.number);
            if (index < 0) {
              index = list.size() + index;
            }
            if (index >= 0 && index < int(list.size())) {
              Variable r = list;
              r.list.erase(r.list.begin()+index);
              return r;
            }
            return *this;
          }
          break;
        }
      }
      return errorInvalidOp("-", other);
    }

    Variable operator*(const Variable& other) const {
      switch (type) {
        case NUMBER: {
          if (other.type == NUMBER) {
            return Variable(number * other.number);
          }
          break;
        }
        case STRING: {
          if (other.type == NUMBER) {
            std::string s = "";
            for (int i = 0; i < other.number; ++i) {
              s += string;
            }
            return s;
          }
          break;
        }
        case BOOL: {
          if (other.type == NUMBER && other.number == -1) {
            return !toBool();
          }
          break;
        }
        case LIST: {
          switch (other.type) {
            case LIST: {
              VarList l(list.begin(), list.end());
              l.insert(l.end(), other.list.begin(), other.list.end());
              return l;
            }
            case NUMBER:  {
              if (list.size() == 0) {
                return VarList();
              }
              else if (list.size() == 1) {
                return VarList(
                  static_cast<unsigned int>(other.number), 
                  list.at(0)
                );
              }
              VarList l;
              for (int i = 0; i < other.number; ++i) {
                l.insert(l.end(), list.begin(), list.end());
              }
              return l;
            }
          }
        }
      }
      return errorInvalidOp("*", other);
    }

    Variable operator/(const Variable& other) const {
      switch (type) {
        case NUMBER: {
          if (other.type == NUMBER)
            return Variable(number / other.number);
          break;
        }
        case LIST: {
          if (other.type == NUMBER) {
            Variable r = VarList();
            if (other.number > 0) {
              for (unsigned int i = 0; i < other.number; ++i) {
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
          break;
        }
        case STRING: {
          if (other.type == NUMBER) {
            Variable r = std::string("");
            if (other.number > 0) {
              for (unsigned int i = 0; i < other.number; ++i) {
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
        }
      } 
      return errorInvalidOp("/", other);
    }

    Variable operator%(const Variable& other) const {
      switch (type) {
        case NUMBER: {
          if (other.type == NUMBER) {
            if (other.number != 0) {
              return Variable(static_cast<long double>(fmod(number, other.number)));
            }
            return error("Divison by zero");
          }
          break;
        }
        case STRING: {
          switch (other.type) {
            case NUMBER: {
              int index = int(other.number);
              if (index < 0) {
                index = string.size() + index;
              }
              if (index >= 0 && index < int(string.size())) {
                return std::string(1, string.at(index));
              }
              return error(
                std::string("Out of range(index=") + std::to_string(index) +
                 " size=" + std::to_string(string.size()) + ")"
              );
            }
            case STRING: {
              int index = string.find(other.string);
              if (index == std::string::npos) {
                return error("Inexistent substring");
              }
              return index;
            }
          }
          break;
        }
        case LIST: {
          if (other.type == NUMBER) {
            int index = int(other.number);
            if (index < 0) {
              index = list.size() + index;
            }
            if (index >= 0 && index < int(list.size())) {
              return list.at(index);
            }
            return error(
                std::string("Out of range(index=") + std::to_string(index) +
                 " size=" + std::to_string(list.size()) + ")"
            );
          }
          break;
        }
      }
      return errorInvalidOp("%", other);
    }

    Variable operator>(const Variable& other) const {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number > other.number);
      if (type == STRING && other.type == STRING) {
        return string.compare(other.string) > 0;
      }
      return errorInvalidOp(">", other);
    }
    Variable operator<(const Variable& other) const {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number < other.number);
      if (type == STRING && other.type == STRING) {
        return string.compare(other.string) < 0;
      }
      return errorInvalidOp("<", other);
    }
    Variable operator>=(const Variable& other) const {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number >= other.number);
      if (type == STRING && other.type == STRING) {
        return string.compare(other.string) >= 0;
      }
      return errorInvalidOp(">=", other);
    }
    Variable operator<=(const Variable& other) const {
      if (type == NUMBER && other.type == NUMBER)
        return Variable(number <= other.number);
      else if (type == STRING && other.type == STRING) {
        return string.compare(other.string) <= 0;
      }
      else if (type == LIST) {
        int index = 0;
        for (Variable v : list) {
          if (v.equals(other)) {
            return index;
          }
          index++;
        }
        return error("Value not found");
      }
      return errorInvalidOp("<=", other);
    }
    
    Variable operator==(const Variable& other) const {
      if (type == NIL || other.type == NIL) {
        return errorInvalidOp("==", other);
      }
      return equals(other);
    }

    Variable operator!=(const Variable& other) const {
      if (type == NIL || other.type == NIL) {
        return errorInvalidOp("~=", other);
      }
      return !equals(other);
    }
    
    bool equals(const Variable& other) const {
      if (type == NUMBER && other.type == NUMBER)
        return (number == other.number);
      if (type == STRING && other.type == STRING)
        return (string == other.string);
      if (type == BOOL && other.type == BOOL)
        return (number == other.number);
      if (type == LIST && other.type == LIST) {
        if (list.size() != other.list.size())
          return false;
        for (unsigned int i = 0; i < list.size(); ++i) {
          if (! list.at(i).equals(other.list.at(i))) {
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
      return (false);
    }

    int length() const {
      switch (type) {
        case Variable::NIL:
          return 0;
        case Variable::NUMBER:
          return int(number);
        case Variable::STRING:
          return string.size();
        case Variable::BOOL:
          return toBool()?1:0;
        case Variable::NODE:
          return 0;
        case Variable::LIST:
          return list.size();
         case Variable::CFUNC:
          return 0;
      }
      return 0;
    }

    static const char* typeName(const Variable& v) {
      switch (v.type) {
        case Variable::NIL:
          return "nil";
        case Variable::NUMBER:
          return "number";
        case Variable::STRING:
          return "string";
        case Variable::BOOL:
          return "bool";
        case Variable::NODE:
          return "node";
        case Variable::LIST:
          return "list";
        case Variable::CFUNC:
          return "cfunc";
        case Variable::TUPLE:
          return "tuple";
      }
      return "";
    }

    bool isError() const {
      return type == NIL && string.size() > 0;
    }

    int type = 0;
    long double number = 0;
    std::string string;
    Parser::Node* node;
    Datatable* context; // creation context for the node/function / Datatable referenced by tuple
    VarList list;
    Variable (*cfunc)(Datatable*);
    
    bool invert = false; // invert result for node/cfunc ?


    // ERRORS

    static Variable error(const std::string& descr) {
      Variable v;
      v.type = NIL;
      v.string = descr;
      return v;
    }

    static Variable error(const std::string& descr, const VarList& errors) {
      Variable err = error(descr);
      for (Variable r : errors) {
        err.string += ";" + r.string;
      }
      return err;
    }

    static Variable error(const std::string& descr, const Variable& back) {
      Variable err = error(
        descr + ";" + back.string
      );
      return err;
    }
    
    static Variable errorInvalidOp(const char* opSymbol, const Variable& a, const Variable& b) {
      std::string descr =
        std::string("Invalid operation")
        + "(" + typeName(a) + opSymbol + typeName(b) + ")";
      if (a.isError()) {
        descr += ";" + a.string;
      }
      if (b.isError()) {
        descr += ";" + b.string;
      }
      return error(descr);
    }
    
};

std::ostream& operator<<(std::ostream& out, const Variable& r) {
  return out << r.toString();
}

#endif