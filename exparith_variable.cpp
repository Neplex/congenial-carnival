#include "exparith_variable.hpp"

#include <assert.h>
#define NDEBUG 1

using namespace std;

std::string const sign_set = ":=";

double Variable::eval(Evaluation_Context &ec) const { return ec.get_value(id); }

string Variable::toString() const { return id; }

double Set::eval(Evaluation_Context &ec) const {
  double v = value->eval(ec);
  ec.valuate(variable->toString(), v);
  return v;
}

string Set::toString() const {
  return variable->toString() + sign_set + value->toString();
}
