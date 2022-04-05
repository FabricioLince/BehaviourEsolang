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
    bool dangerous = false;
    int lastPos = -1;

    Node* execute(TokenStream* stream) {

      if (stream->ended()){
        return NULL;
      }

      if (dangerous && stream->index == lastPos) {
        //std::cout << this->name << " repeating on " << stream->currentToken().toString() << std::endl;
        lastPos = -1;
        return NULL;
      }
      lastPos = stream->index;
      
      
      int previousIndex = stream->index;
      Tree* tree = new Tree(this->name);
      CheckpointRule* cp = NULL;

      for (iterator it = rules.begin(); it != rules.end(); ++it) {
        BaseRule* rule = (*it);
        if(dynamic_cast<CheckpointRule*>(rule)) {
          // hit a checkpoint, save it as current cp and move on to next rule
          cp = dynamic_cast<CheckpointRule*>(rule);
        }
        else {
          Node* result = rule->execute(stream);
          if (result) {
            if (!rule->discard)
              tree->children.push_back(result);
            //std::cout << "current tree:\n" << tree << std::endl;
          }
          else {
            if (cp) {
              // if has hit a checkpoint can't roll back
              // throw a ParsingError
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
              //err += " @(" + std::to_string(stream->lineNumber()) + ", " + std::to_string(stream->colNumber()) +")";
              throw ParsingError(err);
            }
            else {
              // didn't hit a checkpoint
              // revert stream to the index it was before
              stream->index = previousIndex; 
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

    Node* execute(TokenStream* stream) {
      for (unsigned int i = 0; i < rules.size(); ++i) {
        Node* result = rules.at(i)->execute(stream);
        if (result) {
          return result;
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

// Dangerous Sequence is one that can cause infinite loops, so its one that stops preemptively if it's called again but the stream didn't move
SequenceRule* DangerousSequence(std::string name, RuleList rules) {
  SequenceRule* rule = new SequenceRule();
  rule->name = name;
  rule->rules = rules;
  rule->dangerous = true;
  return rule;
}

}

#endif