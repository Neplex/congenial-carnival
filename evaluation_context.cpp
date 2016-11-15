#include "evaluation_context.hpp"

using namespace std;

double Evaluation_Context_No_Variable::get_value(string const &id) {
  assert(false);
  return NAN;
}

void Evaluation_Context_No_Variable::valuate(string const &id, double value) {
  assert(false);
}

double Evaluation_Context_Simple::get_value(string const &id) {
  if (valuation.find(id) == valuation.end()) {
    return NAN;
  } else {
    return valuation[id];
  }
}

void Evaluation_Context_Simple::valuate(string const &id, double value) {
  valuation[id] = value;
}
