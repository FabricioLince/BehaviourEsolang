#ifndef PRINTABLE_H
#define PRINTABLE_H

#include <ostream>

namespace Parser {

class Printable {
  
  public:
    virtual std::string toString() = 0;

};

std::ostream& operator<<(std::ostream& o, Printable& p) {
  return o << p.toString();
}
std::ostream& operator<<(std::ostream& o, Printable* p) {
  return o << p->toString();
}

}

#endif