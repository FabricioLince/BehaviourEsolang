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

Variable Evaluator::execute(Tree* tree, Datatable* data) {
  //std::cout << tree << "\n";
  
  Variable bt = evaluate(tree->children.at(0), data);
  //std::cout << bt << "\n";
  if (bt.type == Variable::STRING) {
    return loadstring(bt, data);
  }
  Variable special;
  if (bt.type == Variable::LIST) {
    for (Variable item : bt.list) {
      if (item.type == Variable::NODE) {
        special = bt;
        bt = item;
        break;
      }
    }
  }
  if (bt.type == Variable::NODE) {
    Datatable* childData = bt.context;
    bool del = false;
    if (childData == NULL) {
      childData = data->makeOrphan();
    }
    childData->context = data;
    
    if (special.type != Variable::NIL) {
      childData->setLocal("s", special);
    }
    
    evaluateArgs(tree, data, childData);
    
    Variable result = evaluate(bt.node, childData);
    
    childData->context = NULL;
    return result;
  }
  
  if (bt.type == Variable::CFUNC) {
    
    Datatable* childData = bt.context;
    if (childData == NULL) {
      childData = data->global()->makeChild();
    }
    childData->context = data;
    
    if (special.type != Variable::NIL) {
      childData->setLocal("s", special);
    }
    
    evaluateArgs(tree, data, childData);
    
    Variable result = (*bt.cfunc)(childData);
    childData->context = NULL;
    
    return result;
  }
  
  return bt.toBool();
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
    if (argList.type == Variable::LIST && argList.list.size() == 1) {
      childData->setLocal("a", argList.list.at(0));
    }
    else if (argList.type != Variable::NIL) {
      childData->setLocal("a", argList);
    }
    childData->removeLocal("_a");
  }
}

Variable Evaluator::executeChildTree(Node* tree, Datatable* data, Variable arg) {
  Datatable* childData = data->makeChild();
  ///childData->context = tree; /// WHAT TO DO
  if (arg.type != Variable::NIL) {
    childData->setLocal("a", arg);
  }
  Variable result = evaluate(tree, childData);

  return result;
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
  return Variable(tree->children.at(1), treeData);
}

#endif