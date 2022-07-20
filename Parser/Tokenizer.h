

#include <vector>
#include <iostream>
#include <fstream>
#include <regex>

namespace Parser {

class RegexRule {
  public:
    RegexRule(std::string name, std::regex regex) {
      setName(name);
      this->regex = regex;
    }
    RegexRule(std::string name, std::regex regex, bool discard) {
      setName(name);
      this->regex = regex;
      this->discard = discard;
    }
    RegexRule(std::string name, std::string symbols) {
      setName(name);
      std::string regexString("^(");
      for (char c : symbols) 
      {
        regexString += "\\";
        regexString += c;
        regexString += "|";
      }
      regexString = regexString.substr(0, regexString.length()-1);
      regexString += ")";
      this->regex = std::regex(regexString);
    }
    RegexRule(std::string name, std::vector<std::string> symbols) {
      setName(name);
      std::string regexString("^(");
      for (std::string str : symbols)
      {
        regexString += "(";
        for (char c : str)
        {
          regexString += "\\";
          regexString += c;
        }
        
        regexString += ")|";
      }
      regexString = regexString.substr(0, regexString.length()-1);
      regexString += ")";
    
      this->regex = std::regex(regexString);
    }
    void setName(std::string name) {
      this->name = name;
      this->id = 0; // TODO unique id based on name (hash)
    }
    int getId() {
      return id;
    }
    std::string name;
    std::regex regex;
    bool discard = false;
    int id = 0;
};

class Token {
  public:
    std::string ruleName;
    int ruleId;
    std::string capture;
    std::string string;
    int line = 0;
    int col = 0;
    std::string toString() {
      return position() + ruleName + ":" + capture;
    }
    std::string position() {
      return std::string("[") + std::to_string(line) + "," + std::to_string(col) + "]";
    }
};

class Tokenizer {
  std::vector<RegexRule> rules;
  
  // count the occurences of what inside of str
  int countOccurences(std::string str, std::string what) {
    int count = 0;
    int pos = 0;
    while (str.find(what, pos) != std::string::npos) {
      count += 1;
      pos = str.find(what, pos)+1;
    }
    return count;
  }
  public:
    Tokenizer() {
      this->rules = {
        RegexRule("comment", std::regex("^\\/\\/[^\n]*\n"), true),
        RegexRule("space", std::regex("^\\s+"), true),
        RegexRule("word", std::regex("^[a-zA-Z][0-9a-zA-Z]*")),
        RegexRule("string", std::regex("^\\\"[^\\\"]*\\\"")),
        RegexRule("string", std::regex("^\\\'[^\\\']*\\\'")),
        RegexRule("decimal", std::regex("^\\d+\\.\\d+")),
        RegexRule("integer", std::regex("^\\d+")),
        RegexRule("reduce", std::vector<std::string>({">+", ">*", ">>", "><"})),
        RegexRule("comparation", {"==", "~=", ">=", "<=", ">", "<", "..", "|>", "|<"}),    
        RegexRule("assign", {"=", "+=", "-=", "*=", "/=", "%="}),
        RegexRule("tuple", std::vector<std::string>({"$${", "${"})),
        RegexRule("match", std::vector<std::string>({"?="})),
        RegexRule("case", std::vector<std::string>({"->"})),
        RegexRule("paren", "()"),
        RegexRule("brack", "[]"),
        RegexRule("curly", "{}"),
        RegexRule("addop", "+-"),
        RegexRule("mulop", "*/%^"),
        RegexRule("if", "|"),
        RegexRule("repeater", "\\"),
        RegexRule("negator", "~"),
        RegexRule("optional", "?"),
        RegexRule("execute", "!"),
        RegexRule("colon", ":"),
        RegexRule("comma", ","),
        RegexRule("print", "@"),
        RegexRule("gettree", "&"),
        RegexRule("length", "#"),
        RegexRule("semicolon", ";"),
        RegexRule("index", "."),
        RegexRule("invalid", std::regex("^."))
      };
    }
    std::vector<Token> extractTokens(std::string content) {
      std::match_results<std::string::iterator> result;
      std::vector<Token> tokens;
      int lineCount = 1;
      int colCount = 0;
      while (true) {
        bool ok = false;

        // search which rule can extract a token in the current position
        for (RegexRule rule : rules)
        {
          // apply rule's regex
          if (std::regex_search(content.begin(), content.end(), result, rule.regex))
          {
            // does the rule generate a token?
            if (!rule.discard)
            {
              Token token;
              token.ruleName = rule.name;
              token.ruleId = rule.getId();
              // get last non-empty capture from regex
              for (int i=0; i < result.size(); ++i) {
                if (result[i].length() > 0)
                  token.capture = result[i];
              }
              token.string = token.capture;
              token.line = lineCount;
              token.col = colCount;
              tokens.push_back(token);
            }
            // how many lines did we jump in the token
            lineCount += countOccurences(result.str(), "\n");
            // how many characters deep are we in the current line
            int rfind = result.str().rfind("\n");
            if (rfind != std::string::npos) {
              colCount = result.str().length() - rfind;
            }
            else {
              colCount += result.str().length();
            }

            // clip the content to what the regex didn't capture
            content = result.suffix().str();

            // we have a capture
            ok = true;
            // don't test any more rules
            break;
          }
        }
        
        // if didn't get any captures stop the loop
        if (!ok) break;
      }
      // having any content left means something in the content wasn't capture by any rule
      if (content.length() > 0) 
        std::cout << "stopped at:\n" << content << "\n" << std::endl;
      
      return tokens;
    }
};

}