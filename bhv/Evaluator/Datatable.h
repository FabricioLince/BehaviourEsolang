#ifndef EVALDATA_H
#define EVALDATA_H

#include "Variable.h"
#include <map>

class Datatable {
    std::vector<Datatable*> children;
    std::vector<Datatable*> orphans;
    
  public:
    std::map<std::string, Variable> memory;
    Datatable* parent = NULL;
    Datatable* context = NULL; // execution context, for when a tree is executed
    bool isOrphan = false;
    
    /// set value on the first parent to have varname, if none saves locally
    void set(std::string varName, Variable value) {
      Datatable* p = firstParentToHave(varName);
      if (p) {
        p->setLocal(varName, value);
      }
      else {
        setLocal(varName, value);
      }
    }
    
    /// set value locally for varName
    void setLocal(std::string varName, Variable value) {
      //std::cout << varName << " being set on " << std::hex << static_cast<void*>(this) << std::dec << "\n";
      memory[varName] = value;
    }
    
    /// set a cfunc with a child context
    void setCFunc(std::string varName, Variable cfunc) {
      cfunc.context = makeChild();
      set(varName, cfunc); 
    }
    /// set a cfunc with an orphan context
    void setOrphanCFunc(std::string varName, Variable cfunc) {
      cfunc.context = makeOrphan();
      set(varName, cfunc); 
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
      return memory.count(varName) > 0 && memory[varName].type != Variable::NIL;
    }
    
    /// search in the ancestry which Datatable is the first to contain varName, from here to global
    Datatable* firstParentToHave(std::string varName) {
      if (has(varName)) {
        return this;
      }
      if (parent) {
        return parent->firstParentToHave(varName);
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
    
    /// creates new orphan Datatable (no parent), but its deleted by this
    Datatable* makeOrphan() {
      Datatable* orphan = new Datatable();
      orphans.push_back(orphan);
      orphan->isOrphan = true;
      return orphan;
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
    
    void clear() {
      memory.clear();
    }
    
    ~Datatable() {
      for (Datatable* child  : children) {
        delete child;
      }
      for (Datatable* orphan : orphans) {
        delete orphan;
      }
      children.clear();
    }
    
    void printFamily() {
      std::cout << std::hex;
      Datatable* p = this;
      while (p) {
        std::cout << static_cast<void*>(p) << " << ";
        p = p->parent;
      }
      
      std::cout << std::dec << "\n";
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