#ifndef CHECKPOINTRULE_H
#define CHECKPOINTRULE_H

#include "BaseRule.h"

namespace Parser {

class CheckpointRule : public BaseRule {
  public:
    std::string msg;
    CheckpointRule(std::string msg = "") {
      this->msg = msg;
    }
    Node* execute(Stream* stream) {
      printf("ERROR: Checkpoint rule (%s) must be child of Sequence Rule\n", this->name.c_str());
      return NULL;
    }
};

CheckpointRule* Checkpoint(std::string msg = "") {
  return new CheckpointRule(msg);
}

}

#endif