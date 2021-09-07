#ifndef INTEGERRULE_H
#define INTEGERRULE_H

#include "BaseTokenRule.h"

namespace Parser {

class IntegerRule : public TokenRule {
  bool isnum(char c){
    return c>='0' && c<='9';
  }
  public:
    IntegerRule() {
      this->name = "integer";
    }
    Node* execute(Stream* stream) {
      ridWhiteSpaces(stream);
      int pos = stream->getPos();
      if (isnum(stream->get())) {
        std::string string = "";
        while (isnum(stream->get())) {
          string += stream->get();
          stream->next();
        }
        return new Token("integer", string, pos);
      }
      stream->setPos(pos);
      return NULL;
    }
};

}

#endif