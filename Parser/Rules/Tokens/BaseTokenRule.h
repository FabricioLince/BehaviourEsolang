#ifndef BASETOKENRULE_H
#define BASETOKENRULE_H

#include "../BaseRule.h"

namespace Parser {

class TokenRule : public BaseRule {
  protected:
    void ridWhiteSpaces(Stream* stream) {
      while (stream->hasNext() && isspace(stream->next())) {}
      stream->setPos(stream->getPos() - 1);
    }
};

}

#endif