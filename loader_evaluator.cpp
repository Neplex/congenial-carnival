#include <stack>
#include <stdlib.h>

#include <iostream>
#include <sstream>

#include "exparith_binary.hpp"
#include "exparith_unary.hpp"
#include "exparith_unary.hpp"
#include "exparith_variable.hpp"
#include "loader_evaluator.hpp"

#include <assert.h>

#define NDEBUG 1

using namespace std;

string const sign_read_stop = ".";

// help write a lot of simular cases
#define CASE_UNARY(sign, exp)                                                  \
  else if (sign == string_read) {                                              \
    assert(1 <= l.size());                                                     \
    Expr *f = l.top();                                                         \
    l.pop();                                                                   \
    l.push(new exp(f));                                                        \
  }

// help write a lot of simular cases
#define CASE_BINARY(sign, exp)                                                 \
  else if (sign == string_read) {                                              \
    assert(2 <= l.size());                                                     \
    Expr *right = l.top();                                                     \
    l.pop();                                                                   \
    Expr *left = l.top();                                                      \
    l.pop();                                                                   \
    l.push(new exp(left, right));                                              \
  }

Loader_Evaluator::Loader_Evaluator(istream &postfixe_stream) {
  string string_read;
  stack<Expr *> l;
  double d;

  while (postfixe_stream >> string_read && string_read != sign_read_stop) {
    stringstream oss(string_read);
    if (oss >> d) {
      l.push(new Constant(d));
    }
    CASE_UNARY("log", Log)
    CASE_UNARY("exp", Exp)
    CASE_BINARY("+", Add)
    CASE_BINARY("-", Sub)
    CASE_BINARY("*", Mul)
    CASE_BINARY("/", Div)
    else if (":=" == string_read) {
      assert(2 <= l.size());
      Expr *val = l.top();
      l.pop();
      Variable *var = (Variable *)l.top();
      l.pop();
      l.push(new Set(var, val));
    }
    else {
      l.push(new Variable(string_read));
    }
  }

  expression = l.top();
}

/*!
 * This is a simple context evaluation (variables are managed correctly) with
 * the special characteristic that if a value is not valuated, it reads the
 * value of the variable on an input stream.
 */
class Evaluation_Context_IStream : public Evaluation_Context_Simple {
  istream &in;

public:
  Evaluation_Context_IStream(istream &in) : in(in) {}
  /*! Read from the map otherwise from the input stream \c in.*/
  double get_value(string const &id) {
    if (valuation.find(id) != valuation.end()) {
      return valuation[id];
    } else {
      double val;
      if (in >> val) {
        return val;
      }
    }
    return NAN;
  }
};

double Loader_Evaluator::evaluate(istream &in) {
  Evaluation_Context_IStream ec(in);
  return evaluate(ec);
}

double Loader_Evaluator::evaluate(Evaluation_Context &ec) {
  return expression->eval(ec);
}
