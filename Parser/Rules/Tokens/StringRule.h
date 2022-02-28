#ifndef STRINGRULE_H
#define STRINGRULE_H

#include "BaseTokenRule.h"

namespace Parser {

class StringRule : public TokenRule { 
  public:
    StringRule() {
      this->name = "string";
    }
    Node* execute(Stream* stream) {
      ridWhiteSpaces(stream);
      int pos = stream->getPos();
      if (stream->nextIf() == '"') {
        std::string string;
        while (stream->hasNext() && stream->get() != '"') {
          string += stream->next();
        }
        if (stream->get() == '"') {
          stream->next(); // consume final quote
          Token* t = new Token(this->name, string, pos);
          t->line = stream->lineNumber();
          return t;
        }
      }
      stream->setPos(pos);
      return NULL;
    }
};

}

#endif