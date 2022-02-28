#ifndef BASETOKENRULE_H
#define BASETOKENRULE_H

#include "../BaseRule.h"

namespace Parser {

class TokenRule : public BaseRule {
  protected:
    void ridWhiteSpaces(Stream* stream) {
      stream->get();
      while (stream->hasNext() && isspace(stream->next())) {}
      stream->setPos(stream->getPos() - 1);
      ridComments(stream);
      
      while (stream->hasNext() && isspace(stream->next())) {}
      stream->setPos(stream->getPos() - 1);
    }
    
    void ridComments(Stream* stream) {
      if (stream->get() == '/') {
        stream->next();
        if (stream->get() == '/') {
          while (stream->hasNext() && stream->next() != '\n') {}
        }
        stream->setPos(stream->getPos() - 1);
        
      }
    }
};

}

#endif