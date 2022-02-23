#ifndef SEQUENCERULE_H
#define SEQUENCERULE_H

#include "BaseRule.h"
#include "CheckpointRule.h"
#include <vector>
#include <string>

namespace Parser {

typedef std::vector<BaseRule*> RuleList;

class BaseCompositeRule : public BaseRule {
  public:
    RuleList rules;
    typedef RuleList::iterator iterator;
};

class SequenceRule : public BaseCompositeRule {
  public:

    Node* execute(Stream* stream) {
      int pos = stream->getPos();
      Tree* tree = new Tree(this->name);
      CheckpointRule* cp = NULL;

      for (iterator it = rules.begin(); it != rules.end(); ++it) {
        BaseRule* rule = (*it);
        if(dynamic_cast<CheckpointRule*>(rule)) {
          cp = dynamic_cast<CheckpointRule*>(rule);
        }
        else {
          Node* result = rule->execute(stream);
          if (result) {
            if (!rule->discard)
              tree->children.push_back(result);
          }
          else {
            if (cp) {
              std::string err = cp->msg;
              if (err.size() == 0) {
                err = "Error constructing " + this->name + "\n";
              }
              else {
                int pos = err.find("$token");
                if (pos > 0) {
                  Token* token = tree->lastToken();
                  if (token) {
                    err.replace(pos, 6, token->toString());
                  }
                  else {
                    err.replace(pos, 6, "$none");
                  }
                }
              }
              err += " @(" + std::to_string(stream->lineNumber()) + ", " + std::to_string(stream->colNumber()) +")";
              throw ParsingError(err);
            }
            else {
              stream->setPos(pos); // revert stream to the position it was before 
              return NULL;
            }
          }
        }
      }
      return tree;
    }
};

class SelectRule : public BaseCompositeRule {
  public:

    Node* execute(Stream* stream) {
      for (unsigned int i = 0; i < rules.size(); ++i) {
        Node* result = rules.at(i)->execute(stream);
        if (result) {
          return result;
          //Tree* tree = new Tree(this->name);
          //tree->children.push_back(result);
          //return tree;
        }
      }
      return NULL;
    }
};

SequenceRule* Sequence(std::string name, RuleList rules) {
  SequenceRule* rule = new SequenceRule();
  rule->name = name;
  rule->rules = rules;
  return rule;
}

SelectRule* Select(RuleList rules) {
  SelectRule* rule = new SelectRule();
  rule->name = "select";
  rule->rules = rules;
  return rule;
}

}

#endif