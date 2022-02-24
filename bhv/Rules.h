#ifndef BHV_RULES_H
#define BHV_RULES_H

#include "../Parser/Rules/CompositeRule.h"
#include "../Parser/Rules/DecoratorRule.h"
#include "../Parser/Rules/BaseRule.h"
#include "../Parser/Rules/Tokens/StringRule.h"
#include "../Parser/Rules/Tokens/IntegerRule.h"
#include "../Parser/Rules/Tokens/PatternRule.h"
#include "../Parser/Rules/Tokens/NameRule.h"

#include "BHV.h"

Rule Bhv::Symbol(std::string patt) {
  Rule rule = Pattern(patt);
  rule->name = "symbol";
  return rule;
}

void Bhv::constructRules() {
  Rule string = new StringRule();
  Rule integer = new IntegerRule();
  Rule name = new NameRule();
  Rule var = new NameRule("var");
  
  Rule decimal = Sequence("decimal", {
    integer,
    Discard(Symbol(".")),
    integer
  });

  BaseCompositeRule* value = Select({
    decimal,
    integer,
    string,
    var,
  });

  Rule unary = Sequence("unary", {
    Select({
      Symbol("~"),
      Symbol("-"),
      Symbol("#")
    }),
    value
  });
  value->rules.push_back(unary);

  Rule multiplication = Sequence("multiplication", {
    value, 
    Multiple("ops", 
      Sequence("op", {
        Select({
          Symbol("*"),
          Symbol("/"),
          Symbol("%"),
          Symbol("^"),
        }),
        Checkpoint("expression expected after $token"),
        value,
      })
    )
  });

  Rule addition = Sequence("addition", {
    multiplication, 
    Multiple("ops", 
      Sequence("op", {
        Select({
          Symbol("+"),
          Symbol("-"),
        }),
        Checkpoint("expression expected after $token"),
        multiplication,
      })
    )
  });

  Rule comparation = Sequence("comparation", {
    addition,
    Optional("comp", 
      Sequence("comp", {
        Select({
          Symbol(">="),
          Symbol("<="),
          Symbol("=="),
          Symbol("~="),
          Symbol(">"),
          Symbol("<"),
        }),
        Checkpoint("expression expected after $token"),
        addition
      })
    )
  });
  
  Rule ifcond = Sequence("ifcond", {
    comparation,
    Optional("cond",
      Sequence("cond", {
        Symbol("|"),
        comparation
      })
    )
  });

  Rule expression = Sequence("expression", {
    ifcond,
    Discard(Optional(Symbol(";")))
  });
  

  
  Rule list = Sequence("list", {
    Discard(Symbol("{")),
    Multiple("values", expression),
    Checkpoint("} expected"),
    Discard(Symbol("}")),
  });
  value->rules.push_back(list);
  
  BaseCompositeRule* cmd = Select({});
  value->rules.insert(value->rules.end()-3, cmd);
  
  Rule print = Sequence("print", {
    Discard(Symbol("@")),
    Optional(Symbol("@")),
    Checkpoint("expression expected"),
    expression
  });
  cmd->rules.push_back(print);
  
  Rule assign = Sequence("assign", {
    name, 
    Select({
      Symbol("="),
      Symbol("+="), 
      Symbol("-="),
      Symbol("*="),
      Symbol("/="),
      Symbol("%="),
    }),
    //Checkpoint("expression expected after $token for assign"),
    expression
  });
  cmd->rules.push_back(assign);

  Rule args = Sequence("args", {
    Discard(Symbol(":")),
    expression,
    Multiple("args", 
      Sequence("arg", {
        Discard(Symbol(",")),
        expression
      })
    )
  });

  Rule executeWithArgs = Sequence("execute", {
    Discard(Optional(Symbol("!"))),
    var,
    Sequence("args", {args})
  });
  cmd->rules.push_back(executeWithArgs);
  
  Rule execute = Sequence("execute", {
    Discard(Symbol("!")),
    expression,
    Optional(args)
  });
  cmd->rules.push_back(execute);

  Rule sequence = Sequence("sequence", {
    Discard(Pattern("(")),
    Checkpoint("expression expected on sequence"),
    Multiple("sequence", expression),
    Checkpoint(") expected"),
    Discard(Pattern(")")),
  });
  cmd->rules.push_back(sequence);

  Rule bhvselect = Sequence("select", {
    Discard(Pattern("[")),
    Checkpoint("expression expected"),
    Multiple("exprs", expression),
    Checkpoint("] expected"),
    Discard(Pattern("]")),
  });
  cmd->rules.push_back(bhvselect);

  Rule optional = Sequence("optional", {
    Discard(Symbol("?")),
    Checkpoint("expression expected"),
    expression
  });
  cmd->rules.push_back(optional);
  
  Rule getTree = Sequence("getTree", {
    Discard(Symbol("&")),
    Optional(Symbol("&")),
    Checkpoint("expression expected after $token"),
    expression
  });
  cmd->rules.push_back(getTree);
  
  Rule repeat = Sequence("repeat", {
    Discard(Symbol("\\")),
    Checkpoint("expression expected"),
    expression
  });
  cmd->rules.push_back(repeat);

  main = Sequence("main", {
    Multiple("exprs", expression)
  });

  collectRules(main);
  
  //std::cout << "value rules:" << "\n";
  for (unsigned int i = 0; i < value->rules.size(); ++i) {
    //std::cout << value->rules.at(i)->name << "\n";
  }
  
}



#endif