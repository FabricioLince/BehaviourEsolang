#ifndef VARACC
#define VARACC

Variable indexTuple(const Variable& tuple, const std::string& index) {
  if (tuple.type == Variable::TUPLE) {
    return tuple.context->get(index);
  }
  else return Variable::error(
    + Variable::typeName(tuple)
    + std::string(" can't be indexed by name")
  );
}

Variable indexList(const Variable& list, const Variable& index) {
  if (list.type == Variable::LIST) {
    return list%index;
  }
  else return Variable::error(
    + Variable::typeName(list)
    + std::string(" can't be indexed by number")
  );
}

Variable Evaluator::var_acc(Tree* tree, Datatable* data) {
  // std::cout << tree << std::endl;
  
  Variable val = var(tree->children.at(0), data);
  if (val.isError()) {
    return val;
  }
  
  for (Node* op : tree->subTree(1)->children) {
    
    // std::cout << "val = " << val << std::endl;
    // std::cout << op << std::endl;
    Token* tok = op->asToken();
    if (tok) {
      if (tok->ruleName == "word") {
        val = indexTuple(val, tok->capture);
      }
      else if (tok->ruleName == "string") {
        val = indexTuple(val, tok->capture.substr(1, tok->capture.length()-2));
      }
      else if (tok->ruleName == "integer") {
        val = indexList(val, evaluate(op, data));
      }
    }
    else {
      Variable indexVal = evaluate(op, data);
      // std::cout << "index = " << indexVal << std::endl;
      if (indexVal.type == Variable::STRING) {
        val = indexTuple(val, indexVal.string);
      }
      else if (indexVal.type == Variable::NUMBER) {
        val = indexList(val, indexVal);
      }
      else {
        if (indexVal.isError()) {
          val = Variable::error(
            Variable::typeName(val)
            + std::string(" can't be indexed by ")
            + Variable::typeName(indexVal),
            indexVal
          );
        }
        val = Variable::error(
          Variable::typeName(val)
          + std::string(" can't be indexed by ")
          + Variable::typeName(indexVal)
        );
      }
    }
    
    if (val.isError()) {
      val.string += op->pos;
      break;
    }
  }
  if (val.isError()) {
    val.string += tree->pos;
    return val;
  }
  return val;
}



#endif