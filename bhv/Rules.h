#ifndef BHV_RULES_H
#define BHV_RULES_H

#include "../Parser/Rules/CompositeRule.h"
#include "../Parser/Rules/DecoratorRule.h"
#include "../Parser/Rules/BaseRule.h"

#include "BHV.h"

void Bhv::constructRules() {
  

  BaseCompositeRule* value = Select({
  });

  Rule unary = Sequence("unary", {
    Select({
      GetToken("negator"),
      Capture("-"),
      GetToken("length")
    }),
    value
  });
  value->rules.push_back(unary);

  Rule multiplication = Sequence("multiplication", {
    value, 
    Multiple("ops", 
      Sequence("op", {
        GetToken("mulop"),
        Checkpoint("expression expected after $token"),
        value,
      })
    )
  });

  Rule addition = Sequence("addition", {
    multiplication, 
    Multiple("ops", 
      Sequence("op", {
        GetToken("addop"),
        Checkpoint("expression expected after $token"),
        multiplication,
      })
    )
  });

  Rule comparation = Sequence("comparation", {
    addition,
    Optional("comp", 
      Sequence("comp", {
        GetToken("comparation"),
        Checkpoint("expression expected after $token"),
        addition
      })
    ),
    Optional("postop", GetToken("reduce"))
  });
  
  Rule ifcond = Sequence("ifcond", {
    comparation,
    Optional("cond",
      Sequence("cond", {
        GetToken("if"),
        comparation
      })
    )
  });
  
  BaseCompositeRule* expression = Sequence("expression", {});
  
  Rule matchCase = Sequence("case", {
    expression,
    Discard(GetToken("case")),
    expression,
  });
  Rule match = Sequence("match", {
    ifcond,
    Optional("cases", Sequence("cases", {
      Discard(GetToken("match")),
      Checkpoint("match needs at least one case"),
      Multiple("cases", 1, matchCase),
    }))
  });
  
  expression->rules.push_back(match);
  expression->rules.push_back(Discard(Optional(GetToken("semicolon"))));
  
  Rule list = Sequence("list", {
    Discard(Capture("{")),
    Multiple("values", expression),
    Checkpoint("} expected"),
    Discard(Capture("}")),
  });
  value->rules.push_back(list);
  
  BaseCompositeRule* cmd = Select({});
  value->rules.push_back(cmd);

  value->rules.push_back(GetToken("decimal"));
  value->rules.push_back(GetToken("integer"));
  value->rules.push_back(GetToken("string"));
  value->rules.push_back(GetToken("word"));
  
  Rule print = Sequence("print", {
    Discard(GetToken("print")),
    Checkpoint("expression expected"),
    expression
  });
  cmd->rules.push_back(print);
  
  Rule assign = Sequence("assign", {
    GetToken("word"), 
    GetToken("assign"),
    Checkpoint("expression expected after $token for assign"),
    expression
  });
  cmd->rules.push_back(assign);

  Rule tuple = Sequence("tuple", {
    GetToken("tuple"),
    Multiple("entries", assign),
    Checkpoint("} expected"),
    Discard(Capture("}")),
  });
  value->rules.push_back(tuple);

  Rule args = Sequence("args", {
    Discard(GetToken("colon")),
    expression,
    Multiple("args", 
      Sequence("arg", {
        Discard(GetToken("comma")),
        expression
      })
    )
  });

  Rule executeWithArgs = Sequence("execute", {
    Discard(Optional(GetToken("execute"))),
    GetToken("word"),
    Sequence("args", {args})
  });
  cmd->rules.push_back(executeWithArgs);
  
  Rule execute = Sequence("execute", {
    Discard(GetToken("execute")),
    expression,
    Optional(args)
  });
  cmd->rules.push_back(execute);

  Rule sequence = Sequence("sequence", {
    Discard(Capture("(")),
    Checkpoint("expression expected on sequence"),
    Multiple("sequence", expression),
    Checkpoint(") expected"),
    Discard(Capture(")")),
  });
  cmd->rules.push_back(sequence);

  Rule bhvselect = Sequence("select", {
    Discard(Capture("[")),
    Checkpoint("expression expected on selector"),
    Multiple("exprs", expression),
    Checkpoint("] expected"),
    Discard(Capture("]")),
  });
  cmd->rules.push_back(bhvselect);

  Rule optional = Sequence("optional", {
    Discard(GetToken("optional")),
    Checkpoint("expression expected"),
    expression
  });
  cmd->rules.push_back(optional);
  
  Rule getTree = Sequence("getTree", {
    Discard(GetToken("gettree")),
    Optional(GetToken("gettree")),
    Checkpoint("expression expected after $token"),
    expression
  });
  cmd->rules.push_back(getTree);
  
  Rule repeat = Sequence("repeat", {
    Optional(Sequence("max", {
      Discard(GetToken("repeater")),
      Select({
        GetToken("integer"),
        GetToken("word"),
      })
    })),
    Discard(GetToken("repeater")),
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