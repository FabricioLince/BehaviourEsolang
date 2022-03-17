#ifndef STREAM_H
#define STREAM_H

#include <cstring>
#include <fstream>
#include "Tokenizer.h"

namespace Parser {
  
  class TokenStream {
    public:
      
      TokenStream(std::string expression) {
        Tokenizer tokenizer;
        tokens = tokenizer.extractTokens(expression);
        index = 0;
      }
    
      std::vector<Token> tokens;
      int index = 0;
      bool hasCurrent() {
        return index < tokens.size();
      }
      Token currentToken() {
        if (!hasCurrent())
          std::cout << "HAS NO TOKEN!" << std::endl;
        return tokens[index];
      }
      
      int lineNumber() {
        if (hasCurrent())
          return currentToken().line;
        return 0;
      }
      int colNumber() {
        if (hasCurrent()) 
          return currentToken().col;
        return 0;
      }

      bool ended() {
        return index >= tokens.size();
      }
        
  };
}

#endif