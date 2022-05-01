#ifndef EVALDATA_H
#define EVALDATA_H

#include "Variable.h"
#include <map>

class Datatable {
  public:
    std::vector<Datatable*> children;
    std::vector<Datatable*> orphans;
    
  public:
    std::map<std::string, Variable> memory;
    Datatable* parent = NULL;
    Datatable* context = NULL; // execution context, for when a tree is executed
    bool isOrphan = false;
    int references = 0;
    std::vector<std::string> refNames;
    
    /// set value on the first parent to have varname, if none saves locally
    void set(const std::string& varName, const Variable& value) {
      Datatable* p = firstParentToHave(varName);
      if (p) {
        p->setLocal(varName, value);
      }
      else {
        setLocal(varName, value);
      }
    }
    
    /// set value locally for varName
    void setLocal(const std::string &varName, const Variable &value) {
      
      /// if saving tuple or full node/cfunc value, increase reference counter on the table
      if (value.type == Variable::TUPLE) {
        value.context->references++;
        value.context->refNames.push_back(varName);
      }
      else if ((value.type == Variable::NODE || value.type == Variable::CFUNC) && value.context) {
        value.context->references++;
        value.context->refNames.push_back(varName);
      }
      // if erasing a tuple or full node/cfunc value, decrease reference counter on table
      if (memory.count(varName) > 0) {
        Variable old = memory[varName];
        if (isReference(old)) {
          old.context->references--;
          auto r = std::find(
            old.context->refNames.begin(), 
            old.context->refNames.end(), varName);
          old.context->refNames.erase(r);
        }
      }
      
      memory[varName] = value;
    }
    
    /// set a cfunc with a child context
    void setCFunc(const std::string &varName, Variable cfunc) {
      cfunc.context = makeChild();
      set(varName, cfunc); 
    }
    
    /// set a cfunc with an orphan context
    void setOrphanCFunc(const std::string& varName, Variable cfunc) {
      cfunc.context = NULL;
      set(varName, cfunc);
    }
    
    /// get value for varName on the first parent to have it
    Variable get(const std::string& varName) {
      Datatable* p = firstParentToHave(varName);
      if (p) {
        return p->memory[varName];
      }
      if (context && context != this) {
        return context->get(varName);
      }
      return Variable::error(varName + " has no value");
    }
    
    /// get value locally for varName
    Variable getLocal(const std::string& varName) {
      return memory[varName];
    }
    
    /// does varName exists locally?
    bool has(const std::string& varName) {
      return memory.count(varName) > 0;
    }
    
    /// search in the ancestry which Datatable is the first to contain varName, from here to global
    Datatable* firstParentToHave(const std::string& varName) {
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
    
    void removeLocal(const std::string& varName) {
      setLocal(varName, Variable());
      memory.erase(varName);
    }
    
    ~Datatable() {
      for (Datatable* child : children) {
        delete child;
      }
      for (Datatable* orphan : orphans) {
        delete orphan;
      }
      children.clear();
      orphans.clear();      
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
    
    bool isReference(const Variable& var) {
      switch (var.type) {
        case Variable::TUPLE:
        case Variable::NODE:
        case Variable::CFUNC:
          return var.context != NULL;
      }
      return false;
    }

    void checkRefs(int depth=0) {
      std::string spc = (Variable("  ")*depth).toString();
      std::cout << spc;
      std::cout << (isOrphan?"Orphan":"Full")
        << std::hex << "{0x"
        << reinterpret_cast<uintptr_t>(this)
        << std::dec << "}"
        << "r" << references ;
      if (has("id"))
        std::cout << " id:" << getLocal("id").toString();
      for (auto const& refName : refNames) {
        std::cout << " " << refName;
      }
      std::cout << std::endl;
      for (Datatable* c : children) {
        c->checkRefs(depth+1);
      }
      for (Datatable* o : orphans) {
        o->checkRefs(depth+1);
      }
    }

    
    int clean(bool unreferenceMine = true) {
      if (references == 0 && unreferenceMine) {
        for (auto const& imap : memory) {
          if (isReference(imap.second)) {
            //std::cout << "unreferencing " << imap.first << std::endl;
            setLocal(imap.first, Variable::error(
              std::string("unreferenced tuple ")+imap.first));
          }
        }
      }
      int sum = 0;
      std::vector<Datatable*> toRemove;
      for (Datatable* o : orphans) {
        int r = o->clean();
        if (r == 0) 
          toRemove.push_back(o);
        sum += r;
      }
      for (Datatable* c : children) {
        int r = c->clean();
        if (r == 0) 
          toRemove.push_back(c);
        sum += r;
      }
      for (Datatable* data : toRemove) {
        auto d = std::find(orphans.cbegin(), orphans.cend(), data);
        if (d != orphans.end()) {
          orphans.erase(d);
        }
        else {
          d = std::find(children.cbegin(), children.cend(), data);
          children.erase(d);
        }
        delete data;
      }
      
      return references + sum;
    }

};

std::ostream& operator<<(std::ostream& out, Datatable* data) {
  if (data->memory.size() == 0) {
    return out << "Empty Datatable\n";
  }
  else {
    for (auto const& it : data->memory) {
      out << it.first << " = " << it.second.toString() << "\n";
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, Datatable& data) {
  return out << &data;
}

#endif