
#include <ctime>
#include <iostream>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif // _WIN32


const Variable nil;

Variable type(Datatable* data);
Variable rand(Datatable* data);
Variable input(Datatable* data);
Variable showdata(Datatable* data);
Variable printvar(Datatable* data);
Variable parseInt(Datatable* data);
Variable toAscii(Datatable* data);
Variable fromAscii(Datatable* data);
Variable ascii(Datatable* data);
Variable time(Datatable* data);
Variable wait(Datatable* data);

void addToDatatable(Datatable* data) {
  
  data->setOrphanCFunc("type", &type);
  data->setCFunc("rand", &rand);
  data->setCFunc("read", &input);
  data->setCFunc("data", &showdata);
  data->setOrphanCFunc("print", &printvar);
  
  data->setOrphanCFunc("int", &parseInt);
  data->setOrphanCFunc("ascii", &ascii);
  data->setOrphanCFunc("time", &time);
  data->setOrphanCFunc("wait", &wait);
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
    srand(std::time(NULL));
    data->setLocal("seed", (int)std::time(NULL));
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

Variable time(Datatable* data) {
  
  std::time_t t = std::time(0);
  std::tm* now = std::localtime(&t);
  Variable l = Variable::VarList();
  l.list.push_back(now->tm_sec);
  l.list.push_back(now->tm_min);
  l.list.push_back(now->tm_hour);
  
  return l;
}

Variable wait(Datatable* data) {
  Variable arg = data->get("a");
  #ifdef _WIN32
        Sleep(arg.number);
    #else
        usleep(arg.number * 1000);
    #endif // _WIN32
  return true;
}
