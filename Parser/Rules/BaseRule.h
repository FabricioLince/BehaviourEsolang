#ifndef BASERULE_H
#define BASERULE_H

#include "../Stream.h"
#include "../Tree.h"
#include <iostream>

namespace Parser {

  class BaseRule {
    public:
      std::string name = "unnamed rule";
      bool discard = false;
      virtual Node* execute(TokenStream* stream) = 0;
      virtual ~BaseRule() {}
  };
  
  class TokenRule : public BaseRule {
    public:
      std::string tokenName;
      Node* execute(TokenStream* stream) {
        if (stream->hasCurrent()) {
          Token current = stream->currentToken();
          if (current.ruleName == tokenName) {
            stream->index += 1;
            return new Leaf(current);
          }
        }
        return NULL;
      }
  };
  class CaptureRule : public BaseRule {
    public:
      std::string capture;
      CaptureRule(std::string capture) {
        this->capture = capture;
      }
      Node* execute(TokenStream* stream) {
        if (stream->hasCurrent()) {
          Token current = stream->currentToken();
          if (current.capture == capture) {
            stream->index += 1;
            return new Leaf(current);
          }
        }
        return NULL;
      }
  };
  

  class ParsingError : public std::exception {
    std::string msg;
    public:
      ParsingError(std::string msg) {
        this->msg = msg;
      }
      const char* what() const throw() {
        return msg.c_str();
      }
  };

  class DebugRule : public BaseRule {
    public:
      std::string msg;
      
      DebugRule(std::string msg) {
        this->msg = msg;
        this->discard = true;
      }
      
      Node* execute(TokenStream* stream) {
        std::cout << msg << "\n";
        int previousIndex = stream->index;
        
        int np = previousIndex-5;
        if (np<0) np = 0;
        int count = 10;
        //stream->setPos(np);
        while (count-- > 0 && stream->hasCurrent()) {
          std::cout << stream->currentToken().toString();
        }
        std::cout << "\n";
        
        stream->index = previousIndex;
        return (Node*)1;
      }
      
  };

  class ForceStopRule : public BaseRule {
    public:
    Node* execute(TokenStream* stream) {
      return NULL;
    }
  };
  
  TokenRule* GetToken(std::string tokenName) {
    TokenRule* rule = new TokenRule();
    rule->name = tokenName;
    rule->tokenName = tokenName;
    return rule;
  }
  CaptureRule* Capture(std::string capture) {
    return new CaptureRule(capture);
  }

  DebugRule* Debug(std::string msg) {
    return new DebugRule(msg);
  }

  ForceStopRule* ForceStop() {
    return new ForceStopRule();
  }

}
#endif