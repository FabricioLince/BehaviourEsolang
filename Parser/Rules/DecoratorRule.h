#ifndef DECORATORRULE_H
#define DECORATORRULE_H

#include "BaseRule.h"

namespace Parser {

class BaseDecoratorRule : public BaseRule {
  public:
    BaseRule* child;
};

class MultipleRule : public BaseDecoratorRule {
  public:
    unsigned int min = 0;
    MultipleRule(std::string name, BaseRule* child, int min = 0) {
      this->min = min;
      this->child = child;
      this->name = name;
    }
    MultipleRule(BaseRule* child, unsigned int min = 0) {
      this->min = min;
      this->child = child;
      this->name = child->name;
    }
    Node* execute(TokenStream* stream) {
      int previousIndex = stream->index;
      Tree* tree = new Tree(this->name);
      Node* result = this->child->execute(stream);
      while (result) {
        tree->children.push_back(result);
        if (tree->pos.size() == 0)
          tree->pos = result->pos;
        result = this->child->execute(stream);
      }
      if (tree->children.size() >= this->min) {
        return tree;
      }
      stream->index = previousIndex;
      return NULL;
    }
};

class OptionalRule : public BaseDecoratorRule {
  public:
    OptionalRule(std::string name, BaseRule* child) {
      this->child = child;
      this->name = name;
    }
    OptionalRule(BaseRule* child) {
      this->child = child;
      this->name = "optional";
    }
    Node* execute(TokenStream* stream) {
      Tree* tree = new Tree(this->name);
      Node* result = this->child->execute(stream);
      if (result) {
        tree->children.push_back(result);
        tree->pos = result->pos;
      }
      return tree;
    }
};

MultipleRule* Multiple(std::string name, int min, BaseRule* child) {
  return new MultipleRule(name, child, min);
}
MultipleRule* Multiple(std::string name, BaseRule* child) {
  return new MultipleRule(name, child);
}
MultipleRule* Multiple(BaseRule* child) {
  return new MultipleRule(child);
}

OptionalRule* Optional(BaseRule* child) {
  return new OptionalRule(child);
}
OptionalRule* Optional(std::string name, BaseRule* child) {
  return new OptionalRule(name, child);
}

BaseRule* Discard(BaseRule* rule) {
  rule->discard = true;
  return rule;
}

}

#endif