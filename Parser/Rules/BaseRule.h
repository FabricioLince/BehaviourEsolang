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
      virtual Node* execute(Stream* stream) = 0;
      virtual ~BaseRule() {}
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
      
      Node* execute(Stream* stream) {
        std::cout << msg << "\n";
        int pos = stream->getPos();
        
        int np = pos-5;
        if (np<0) np = 0;
        int count = 10;
        //stream->setPos(np);
        while (count-- > 0 && stream->hasNext()) {
          std::cout << stream->next();
        }
        std::cout << "\n";
        
        stream->setPos(pos);
        return (Node*)1;
      }
      
  };

  class ForceStopRule : public BaseRule {
    public:
    Node* execute(Stream* stream) {
      return NULL;
    }
  };

  DebugRule* Debug(std::string msg) {
    return new DebugRule(msg);
  }

  ForceStopRule* ForceStop() {
    return new ForceStopRule();
  }

}
#endif