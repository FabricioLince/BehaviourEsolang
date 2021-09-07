#ifndef EVALDATA_H
#define EVALDATA_H

#include "Variable.h"
#include <map>

class Datatable {
    std::vector<Datatable*> children;
  public:
    std::map<std::string, Variable> memory;
    Datatable* parent = NULL;
    Datatable* context = NULL;
    
    /// set value on the first parent to have varname, if none saves locally
    void set(std::string varName, Variable value) {
      Datatable* p = firstParentToHave(varName, this->context);
      if (p) {
        p->setLocal(varName, value);
      }
      else {
        setLocal(varName, value);
      }
    }
    
    /// set value locally for varName
    void setLocal(std::string varName, Variable value) {
      memory[varName] = value;
    }
    
    /// get value for varName on the first parent to have it
    Variable get(std::string varName) {
      Datatable* p = firstParentToHave(varName);
      if (p) {
        return p->memory[varName];
      }
      if (context && context != this) {
        return context->get(varName);
      }
      return Variable();
    }
    
    /// get value locally for varName
    Variable getLocal(std::string varName) {
      return memory[varName];
    }
    
    /// does varName exists locally?
    bool has(std::string varName) {
      return memory.count(varName) > 0;
    }
    
    /// search in the ancestry which Datatable is the first to contain varName, from here to global
    /// stops search if context is different and not null
    Datatable* firstParentToHave(std::string varName, Datatable* context=NULL) {
      if (context && this->context != context) {
        return NULL;
      }
      if (has(varName)) {
        return this;
      }
      if (parent) {
        return parent->firstParentToHave(varName, context);
      }
      return NULL;
    }
    
    /// gets last Datatable in the ancestry (the one with no parent)
    Datatable* global() {
      if (parent) {
        return parent->global();
      }
      return this;
    }
    
    /// creates new Datatable that has this as parent
    Datatable* makeChild() {
      Datatable* child = new Datatable();
      child->parent = this;
      child->context = this->context;
      children.push_back(child);
      return child;
    }
    
    /// number of parents in the ancestry (0 means global)
    int depth() {
      int d = 0;
      Datatable* p = parent;
      while (p) {
        d += 1;
        p = p->parent;
      }
      return d;
    }
    
    void removeLocal(std::string varName) {
      memory.erase(varName);
    }
    
    ~Datatable() {
      for (Datatable* child  : children) {
        delete child;
      }
      children.clear();
    }
    
};

std::ostream& operator<<(std::ostream& out, Datatable& data) {
  for (std::map<std::string, Variable>::iterator it = data.memory.begin(); it != data.memory.end(); ++it) {
    out << it->first << " = " << it->second << "\n";
  }

  return out;
}

std::ostream& operator<<(std::ostream& out, Datatable* data) {
  for (std::map<std::string, Variable>::iterator it = data->memory.begin(); it != data->memory.end(); ++it) {
    out << it->first << " = " << it->second << "\n";
  }
  return out;
}

#endif