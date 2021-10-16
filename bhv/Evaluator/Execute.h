#ifndef EVAL_EXECUTE_H
#define EVAL_EXECUTE_H

#include "Evaluator.h"

void Evaluator::evaluateArg(Node* arg, Datatable* data, Datatable* childData) {
  //std::cout << arg << "\n";
  if (arg->name == "assign") {
    assign(arg, data, childData);
    //std::cout << "arg data:\n" << childData << "\n";
    //std::cout << arg->asTree()->getToken(0)->string << " = " << value << "\n";
  }
  else {
    Variable value = evaluate(arg, data);
    Variable args = childData->getLocal("_a"); // temporary, so it doesn't overwrite the current context's args
    if (args.type != Variable::LIST) {
      args = Variable(Variable::VarList());
    }
    args.list.push_back(value);
    childData->setLocal("_a", args);
  }
}

void Evaluator::evaluateArgs(Tree* tree, Datatable* data, Datatable* childData) {
  Tree* argTree = tree->subTree(1);
  if (argTree->children.size() > 0) {
    argTree = argTree->subTree(0);
    //std::cout << "args:\n";
    evaluateArg(argTree->children.at(0), childData, childData);

    Tree* args = argTree->subTree("args");
    for (int i = 0; i < args->children.size(); ++i) {
      evaluateArg(args->children.at(i), data, childData);
    }
    
    Variable argList = childData->getLocal("_a");
    if (argList.type == Variable::LIST) {
      std::vector<std::string> names = std::vector<std::string>({"a", "b", "c", "d", "e"});
      for (int i = 0; i < argList.list.size(); ++i) {
        childData->setLocal(names.at(i), argList.list.at(i));
      }
    }
    if (argList.type != Variable::NIL) {
      childData->setLocal("al", argList);
    }
    childData->removeLocal("_a");
  }
}

Variable Evaluator::execute(Tree* tree, Datatable* data) {
  //std::cout << tree << "\n";
  
  Variable bt = evaluate(tree->children.at(0), data);
  switch (bt.type) {
    case Variable::STRING:
      return bt.string.size() > 0;
      break;
    case Variable::NUMBER:
      return int(bt.number) != 0;
      break;
    case Variable::LIST:
      return bt.list.size() > 0;
      break;
    case Variable::NODE:
    case Variable::CFUNC:
    {
      Datatable* childData = bt.context;
      childData->context = data;
      //std::cout << "childData:\n" << childData << "\n";
      
      evaluateArgs(tree, data, childData);
      
      Variable result = justExecute(bt, childData);
      
      childData->context = NULL;
      return result;
    }
  }
  
  return bt.toBool();
}


Variable Evaluator::executeChildTree(Node* tree, Datatable* data) {
  Variable result = evaluate(tree, data);
  return result;
}
Variable Evaluator::executeCFunc(Variable cfunc, Datatable* data) {
  Variable result = (*cfunc.cfunc)(data);
  return result;
}
Variable Evaluator::executeAny(Variable nodeOrFunc, Datatable* data, Variable arg, Variable b) {
  Datatable* childData = nodeOrFunc.context;
  if (childData == NULL) {
    std::cout << "No context detected for " << nodeOrFunc.toString() << "\n";
    childData = data->makeOrphan();
  }
  
  if (arg.type != Variable::NIL) {
    childData->setLocal("a", arg);
  }
  if (b.type != Variable::NIL) {
    childData->setLocal("b", b);
  }
  
  childData->context = data;
  
  return justExecute(nodeOrFunc, childData);
}
Variable Evaluator::justExecute(Variable nodeOrFunc, Datatable* data) {
  Variable r = Variable();
  if (nodeOrFunc.type == Variable::NODE) {
    r = executeChildTree(nodeOrFunc.node, data);
  }
  if (nodeOrFunc.type == Variable::CFUNC) {
    r = executeCFunc(nodeOrFunc, data);
  }
  if (data->isOrphan) {
      data->clear();
    }
  if (nodeOrFunc.invert) {
    return !r.toBool();
  }
  return r;
}

Variable Evaluator::loadstring(Variable str, Datatable* data) {
  //std::cout <<"loading "<<str <<"\n";
  std::ifstream file;
  file.open(str.string.c_str());
  if (file.is_open()) {
    Parser::Stream* stream = new Parser::FileStream(str.string);
    Bhv bhv;
    Node* node = bhv.extractTree(stream);
    delete stream;
    if (node) {
      return evaluate(node, data);
    }
  }
  else {
    Parser::Stream* stream = new Parser::StringStream(str.string+"  ");
    Bhv bhv;
    Node* node = NULL;
    try {
      node = bhv.extractTree(stream);
    }
    catch (Parser::ParsingError e) {
    }
    delete stream;
    if (node) {
      return evaluate(node, data);
    }
  }
  return false;
}

Variable Evaluator::getTree(Tree* tree, Datatable* data) {
  //std::cout << tree << "\n";
  Datatable* treeData = NULL;
  if (tree->subTree(0)->children.size() > 0) {
    treeData = data->makeChild();
  }
  else {
    treeData = data->makeOrphan();
  }
  return Variable(tree->children.at(1), treeData);
}

#endif