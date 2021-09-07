#ifndef PATTERNRULE_H
#define PATTERNRULE_H

#include "BaseTokenRule.h"

namespace Parser {

class PatternRule : public TokenRule {
    std::string pattern;
  public:
    PatternRule(std::string pattern) {
      this->pattern = pattern;
      this->name = "pattern";
    }
    
    PatternRule(std::string name, std::string pattern) {
      this->pattern = pattern;
      this->name = name;
    }

    Node* execute(Stream* stream) {
      ridWhiteSpaces(stream);
      int pos = stream->getPos();
      bool ok = true;
      for (int i = 0; i < pattern.size(); ++i) {
        if (stream->hasNext() && stream->next() == pattern.at(i)) {
          //std::cout << "ok " << pattern.at(i) << "\n";
        }
        else {
          ok = false;
          break;
        }
      }
      if (ok) {
        Token* token = new Token(this->name, pattern, pos);
        return token;
      }
      stream->setPos(pos);
      return NULL;
    }
};

PatternRule* Pattern(std::string patt) {
  return new PatternRule(patt);
}

}

#endif