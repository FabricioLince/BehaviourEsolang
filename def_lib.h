


const Variable nil;

Variable type(Datatable* data);
Variable rand(Datatable* data);
Variable input(Datatable* data);
Variable showdata(Datatable* data);
Variable printvar(Datatable* data);
Variable printData(Datatable* data);
Variable parseInt(Datatable* data);
Variable toAscii(Datatable* data);
Variable fromAscii(Datatable* data);
Variable ascii(Datatable* data);


void addToDatatable(Datatable* data) {
  
  data->setOrphanCFunc("type", &type);
  data->setCFunc("rand", &rand);
  data->setCFunc("read", &input);
  data->setCFunc("data", &showdata);
  data->setOrphanCFunc("print", &printvar);
  data->setOrphanCFunc("pdata", &printData);
  
  data->setOrphanCFunc("int", &parseInt);
  data->setOrphanCFunc("ascii", &ascii);
}



Variable type(Datatable* data) {
  Variable arg = data->get("a");
  switch (arg.type) {
    case Variable::NIL:
          return "nil";
        case Variable::NUMBER:
          return "number";
        case Variable::STRING:
          return "string";
        case Variable::BOOL:
          return "bool";
        case Variable::NODE:
          return "node";
        case Variable::LIST:
          return "list";
        case Variable::CFUNC:
          return "cfunc";
  }
  return Variable();
}
Variable rand(Datatable* data) {
  if (!data->has("seed")) {
    srand(time(NULL));
    data->setLocal("seed", (int)time(NULL));
    //std::cout << "setting seed " << data->get("seed").number << "\n";
  }
  return rand();
}
Variable input(Datatable* data) {
  if (!data->has("number")) {
    data->setLocal("number", 0);
    data->setLocal("string", 1);
  }
  Variable arg = data->getLocal("a");
  if (arg.type == Variable::NUMBER) {
    switch ((int)arg.number) {
      case 0:
      {
        int n = 0;
        std::cin >> n;
        return n;
      }
      case 1:
      {
        std::string s;
        getline(std::cin, s);
        return s;
      }
    }
  }
  return Variable();
}
Variable showdata(Datatable* data) {
  std::cout << "Data:\n" << data->context << "\n";
  return true;
}


Variable printData(Datatable* data) {
  std::cout << data->context->parent << "\n";
  return true;
}

Variable printvar(Datatable* data) {
  Variable arg = data->getLocal("a");
  std::cout << arg.toString() << "\n";
  return true;
}


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
