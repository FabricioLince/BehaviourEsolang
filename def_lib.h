
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
Variable readLine(Datatable* data);
Variable showdata(Datatable* data);
Variable datakeys(Datatable* data);
Variable printvar(Datatable* data);
Variable parseInt(Datatable* data);
Variable toAscii(Datatable* data);
Variable fromAscii(Datatable* data);
Variable ascii(Datatable* data);
Variable time(Datatable* data);
Variable wait(Datatable* data);
Variable removeFromDatatable(Datatable* data);
Variable execSystemFunc(Datatable* data);
Variable error(Datatable* data);

void addToDatatable(Datatable* data) {
  
  data->setCFunc("type", &type);
  data->setCFunc("rand", &rand);
  data->setCFunc("read", &readLine);
  data->setCFunc("data", &showdata);
  data->setCFunc("datakeys", &datakeys);
  data->setCFunc("print", &printvar);
  data->setCFunc("remove", &removeFromDatatable);
  
  data->setCFunc("int", &parseInt);
  data->setCFunc("ascii", &ascii);
  data->setCFunc("time", &time);
  data->setCFunc("wait", &wait);
  data->setCFunc("sys", &execSystemFunc);
  data->setCFunc("error", &error);
  
}

Variable type(Datatable* data) {
  return Variable::typeName(data->get("a"));
}
Variable rand(Datatable* data) {
  if (!data->has("seed")) {
    srand(std::time(NULL));
    data->setLocal("seed", (int)std::time(NULL));
    //std::cout << "setting seed " << data->get("seed").number << "\n";
  }
  return rand();
}
Variable readLine(Datatable* data) {
  std::string s;
  getline(std::cin, s);
  return s;
}

Datatable* getDatatableByArgDepth(Datatable* data) {
  const Variable& arg = data->getLocal("a");
  Datatable* show = data->context;
  int depth = 0;
  if (arg.type == Variable::NUMBER) depth = int(arg.number);
  if (depth < 0) {
    show = show->global();
  }
  else while (depth > 0 && show->parent) {
    show = show->parent;
    depth -= 1;
  }
  return show;
}

Variable showdata(Datatable* data) {
  Datatable* show = getDatatableByArgDepth(data);
  std::cout << "Data:\n" << show << std::endl;
  return true;
}
Variable datakeys(Datatable* data) {
  Datatable* show = getDatatableByArgDepth(data);
  Variable list = Variable::VarList();
  list.list.reserve(show->memory.size());
  for(std::map<std::string, Variable>::iterator it = show->memory.begin(); it != show->memory.end(); ++it) {
    list.list.push_back(it->first);
  }
  return list;
}

Variable printvar(Datatable* data) {
  std::cout << data->getLocal("a").toString();
  return true;
}

Variable parseInt(Datatable* data) {
  const Variable& arg = data->get("a");
  if (arg.type == Variable::STRING) {
    try {
      return std::stoi(arg.string);
    }
    catch (std::invalid_argument& a) {
      return Variable::error("Could parse arg");
    }
  }
  return nil;
}

Variable toAscii(Datatable* data) {
  const Variable& arg = data->get("a");
  if (arg.type == Variable::STRING) {
    return static_cast<int>(arg.string[0]);
  }
  return nil;  
}

Variable fromAscii(Datatable* data) {
  const Variable& arg = data->get("a");
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
  return Variable::error("arg needs to be number or string");
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
  const Variable& arg = data->get("a");
  #ifdef _WIN32
        Sleep(arg.number);
    #else
        usleep(arg.number * 1000);
    #endif // _WIN32
  return true;
}


Variable removeFromDatatable(Datatable* data) {
  const Variable& alv = data->getLocal("al");
  if (alv.type == Variable::LIST) {
    for (const Variable& v : alv.list) {
      if (v.type == Variable::STRING) {
        data->context->removeLocal(v.string);
      }
    }
  }
  return true;
}

Variable execSystemFunc(Datatable* data) {
  const Variable& f = data->get("a");
  if (f.type == Variable::STRING) {
    system(f.string.c_str());
  }
  return true;
}

Variable error(Datatable* data) {
  if (data->has("b")) {
    const Variable& b = data->get("b");
    if (b.isError())
      return Variable::error(data->get("a").toString(), b);
  }
  return Variable::error(data->get("a").toString());
}