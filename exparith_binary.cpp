#include "exparith_binary.hpp"

#include <assert.h>
#define NDEBUG 1

using namespace std;

std::string const sign_add = "+";
std::string const sign_sub = "-";
std::string const sign_mul = "*";
std::string const sign_div = "/";

double Op_Binary::eval(Evaluation_Context &ec) const {
  double l = left->eval(ec);
  double r = right->eval(ec);
  return this->compute(l, r);
}

string Op_Binary::toString() const {
  std::string res = "";
  if (left->get_priority() < get_priority()) {
    res += "( " + left->toString() + " )";
  } else {
    res += left->toString();
  }
  res += " " + sign + " ";
  if (right->get_priority() < get_priority()) {
    res += "( " + right->toString() + " )";
  } else {
    res += right->toString();
  }
  return res;
}

#define COMPUTE_BINARY(class, op)                                              \
  double class ::compute(double left, double right) const {                    \
    return left op right;                                                      \
  }

COMPUTE_BINARY(Add, +)
COMPUTE_BINARY(Sub, -)
COMPUTE_BINARY(Mul, *)
COMPUTE_BINARY(Div, /)
