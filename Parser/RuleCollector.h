
#include <set>

namespace Parser {

typedef std::set<BaseRule*> RuleCollection;
void collectRules(BaseRule* rule, RuleCollection& collection) {
  if (collection.find(rule) != collection.end())
  {
    return;
  }
  collection.insert(rule);

  BaseDecoratorRule* decorator = dynamic_cast<BaseDecoratorRule*>(rule);
  if (decorator) {
    //printf("rule %s is decorator\n", rule->name.c_str());
    collectRules(decorator->child, collection);
    return;
  }

  BaseCompositeRule* composite = dynamic_cast<BaseCompositeRule*>(rule);
  if (composite) {
    //printf("rule %s is composite\n", rule->name.c_str());
    for (int i = 0; i < composite->rules.size(); ++i) {
      collectRules(composite->rules.at(i), collection);
    }
    return;
  }

  //printf("rule %s is base\n", rule->name.c_str());
}


RuleCollection collectRules(BaseRule* rule) {
  RuleCollection collection;
  collectRules(rule, collection);
  return collection;
}

}