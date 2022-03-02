#ifndef BASETOKENRULE_H
#define BASETOKENRULE_H

#include "../BaseRule.h"

namespace Parser {

class TokenRule : public BaseRule {
  protected:
    void ridWhiteSpaces(Stream* stream) {
      
      bool isCmt = ridComments(stream);
      bool isSpc = isspace(stream->get());
      while (isCmt || isSpc) {
        isCmt = ridComments(stream);
        isSpc = isspace(stream->get());
        while (isspace(stream->get())) {
          stream->next();
        }
      }
    }
    
    bool ridComments(Stream* stream) {
      if (stream->get() == '/') {
        stream->next();
        bool isCmt = false;
        if (stream->get() == '/') {
          isCmt = true;
          while (stream->hasNext() && stream->next() != '\n') {}
        }
        stream->setPos(stream->getPos() - 1);
        return isCmt;
      }
      return false;
    }
};

}

#endif