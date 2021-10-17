


const Variable nil;

Variable parseInt(Datatable* data) {
  Variable arg = data->get("a");
  if (arg.type == Variable::STRING) {
    try {
      return std::stoi(arg.string);
    }
    catch (std::invalid_argument& a) {
      return nil;
    }
  }
  return nil;
}

Variable toAscii(Datatable* data) {
  Variable arg = data->get("a");
  if (arg.type == Variable::STRING) {
    return static_cast<int>(arg.string[0]);
  }
  return nil;  
}

Variable fromAscii(Datatable* data) {
  Variable arg = data->get("a");
  if (arg.type == Variable::NUMBER) {
    return std::string(1, static_cast<char>(arg.number));
  }
  return nil;  
}

Variable ascii(Datatable* data) {
  Variable arg = data->get("a");
  if (arg.type == Variable::NUMBER) {
    return std::string(1, static_cast<char>(arg.number));
  }
  if (arg.type == Variable::STRING) {
    return static_cast<int>(arg.string[0]);
  }
  return nil;
}

void addToDatatable(Datatable* data) {
  data->setOrphanCFunc("int", &parseInt);
  data->setOrphanCFunc("ascii", &ascii);
}