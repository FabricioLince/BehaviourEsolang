#ifndef TREE_H
#define TREE_H

#include <vector>
#include <string>
#include <set>
#include <map>
#include "Printable.h"

namespace Parser {

class Tree;
class Token;

class Node : public Printable { 
  protected:
    Node() {}
  public:
    std::string name;
    int id = -1;
    virtual ~Node() {}
    Tree* asTree(); 
    Token* asToken();
};
typedef std::vector<Node*> NodeList;

class Token : public Node {
  
  std::hash<std::string> hasher;
  public:
    Token(std::string name, std::string string, int pos) {
      this->name = name;
      this->string = string;
      this->pos = pos;
      this->id = hasher(name);
    }
    
    Token(int id, std::string string, int pos) {
      this->id = id;
      this->string = string;
      this->pos = pos;
    }
    std::string toString() {
      if (this->name.size() == 0) {
        return std::to_string(this->id) + ":'" + this->string + "'";
      }
      return this->name + ":'" + this->string + "'";//("+       std::to_string(this->pos)+")";
    }
    std::string string;
    int pos;
    int col = 0;
    int line = 0;
};

class Tree : public Node {
  static int t(int a = 0) {
    static int tc = 0;
    tc += a;
    return tc;
  }
  static std::string tab() {
    std::string s = "";//+std::to_string(t());
    for (int i = 0; i < t(); ++i) {
      s += "|  ";
    }
    return s;
  }
  std::hash<std::string> hasher;
  
  public:
    NodeList children;

    Tree() {}
    Tree(std::string name) {
      this->name = name;
      this->id = hasher(name);
    }
    Tree(int id, std::string name = "") {
      this->id = id;
      this->name = name;
    }

    Node* get(std::string childName) {      
      for (unsigned int i = 0; i < children.size(); ++i) {
        if (children.at(i)->name == childName) {
          return children.at(i);
        }
      }
      return NULL;
    }
    Node* getById(int id) {
      for (unsigned int i = 0; i < children.size(); ++i) {
        if (children.at(i)->id == id) {
          return children.at(i);
        }
      }
      return NULL;
    }
    Tree* subTree(std::string childName) {
      Node* child = get(childName);
      if (child) {
        return child->asTree();
      }
      return NULL;
    }
    Tree* subTree(unsigned int index) {
      if (index >= 0 && index < children.size()) {
        return children.at(index)->asTree();
      }
      return NULL;
    }
    Tree* subTreeById(int id) {
      Node* node = getById(id);
      if (node) {
        return node->asTree();
      }
      return NULL;
    }
    
    Token* getToken(unsigned int index) {
      if (index >= 0 && index < children.size()) {
        return children.at(index)->asToken();
      }
      return NULL;
    }
    Token* getToken(std::string name) {
      Node* child = get(name);
      if (child) {
        return child->asToken();
      }
      return NULL;
    }
    Token* getTokenById(int id) {
      Node* child = getById(id);
      if (child) {
        return child->asToken();
      }
      return NULL;
    }

    std::string toString() {
      std::string s = name;
      t(1);
      if (children.size() == 0) {
        s += "\n" + tab() + "EMPTY";
      }
      else {
        for (unsigned int i = 0; i < children.size(); ++i) {
          s += "\n" + tab() + children.at(i)->toString();
        }
      }
      t(-1);
      return s;
    }

    ~Tree() {
      //printf("deleting tree %s\n", name.c_str());
      for (unsigned int i = 0; i < children.size(); ++i) {
        delete children.at(i);
      }
      children.clear();
    }

    
    void removeEmpty(std::set<std::string> names) {
      std::vector<NodeList::iterator> toRemove;
      for (NodeList::iterator it = this->children.begin(); it != this->children.end(); ++it) {
        Tree* ctree = (*it)->asTree();
        if (ctree) {
          if (ctree->children.size() == 0) {
            //printf("%s is empty\n", ctree->name.c_str());
            if (names.count(ctree->name)>0)
              toRemove.push_back(it);
          }
          else {
            ctree->removeEmpty(names);
          }
        }
      }
      for (std::vector<NodeList::iterator>::reverse_iterator rit = toRemove.rbegin(); rit != toRemove.rend(); ++rit) {
        this->children.erase(*rit);
      }
    }
    
    void subsOnlyChild(std::set<std::string> names) {
      std::vector<NodeList::iterator> toRemove;
      for (NodeList::iterator it = this->children.begin(); it != this->children.end(); ++it) {
        Tree* ctree = (*it)->asTree();
        if (ctree) {
          ctree->subsOnlyChild(names);
          if (ctree->children.size() == 1) {
            //printf("%s has only 1 child\n", ctree->name.c_str());
            if (names.count(ctree->name)>0)
              (*it) = *ctree->children.begin();
          }
        }
      }
      for (std::vector<NodeList::iterator>::reverse_iterator rit = toRemove.rbegin(); rit != toRemove.rend(); ++rit) {
        this->children.erase(*rit);
      }
    }

    Token* lastToken() {
      if (children.size() > 0) {
        Node* last = children.back();
        Token* token = dynamic_cast<Token*>(last);
        if (token) {return token;}
        return last->asTree()->lastToken();
      }
      return NULL;
    }
    
    Token* firstToken() {
      for (Node* child : children) {
        Token* token = child->asToken();
        if (token) return token;
        token = child->asTree()->firstToken();
        if (token) return token;
      }
      return NULL;
    }

};

Tree* Node::asTree() {
  return dynamic_cast<Tree*>(this); 
}
Token* Node::asToken() {
  return dynamic_cast<Token*>(this);
}

}
#endif