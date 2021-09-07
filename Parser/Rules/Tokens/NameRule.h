#ifndef NAMERULE_H
#define NAMERULE_H

#include "BaseTokenRule.h"

namespace Parser {

class NameRule : public TokenRule { 
  
  bool validFirst(char c) {
    return isalpha(c);
  }
  bool validSecond(char c) {
    return isalpha(c) || (c >= '0' and c <= '9');
  }
  public:
    NameRule(std::string name = "name") {
      this->name = name;
    }
    Node* execute(Stream* stream) {
      ridWhiteSpaces(stream);
      int pos = stream->getPos();
      if (validFirst(stream->get())) {
        std::string string = "";
        string += stream->next();
        while (stream->hasNext() && validSecond(stream->get())) {
          string += stream->next();
        }
        return new Token(this->name, string, pos);
      }
      stream->setPos(pos);
      return NULL;
    }
};

}

#endif