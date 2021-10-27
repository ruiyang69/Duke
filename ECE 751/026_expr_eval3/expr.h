#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

class Expression {
 public:
  virtual ~Expression() {}
  virtual string toString() const = 0;
  virtual long evaluate() const = 0;
};

class NumExpression : public Expression {
  long number;

 public:
  NumExpression(long x) : number(x) {}
  virtual ~NumExpression() {}
  virtual string toString() const {
    stringstream ss;
    ss << number;
    return ss.str();
  }
  virtual long evaluate() const { return number; }
};

class OperatorExpression : public Expression {
 public:
  Expression * lhs;
  Expression * rhs;
  char oprt;
  OperatorExpression(Expression * lhs, Expression * rhs, char oprt) :
    lhs(lhs),rhs(rhs),oprt(oprt) {}

  virtual ~OperatorExpression() {
    delete lhs;
    delete rhs;
  }

  virtual string toString() const {
    stringstream ss;
    ss << "(" << lhs->toString() << " " << oprt << " " << rhs->toString() << ")";
    return ss.str();
  }
};

class PlusExpression : public OperatorExpression {
 public:
  PlusExpression(Expression * lhs, Expression * rhs) :
      OperatorExpression(lhs, rhs, '+') {}
  virtual long evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
};

class MinusExpression : public OperatorExpression {
 public:
  MinusExpression(Expression * lhs, Expression * rhs) :
      OperatorExpression(lhs, rhs, '-') {}
  virtual long evaluate() const { return lhs->evaluate() - rhs->evaluate(); }
};

class TimesExpression : public OperatorExpression {
 public:
  TimesExpression(Expression * lhs, Expression * rhs) :
      OperatorExpression(lhs, rhs, '*') {}
  virtual long evaluate() const { return lhs->evaluate() * rhs->evaluate(); }
};

class DivExpression : public OperatorExpression {
 public:
  DivExpression(Expression * lhs, Expression * rhs) : OperatorExpression(lhs, rhs, '/') {}
  virtual long evaluate() const { return lhs->evaluate() / rhs->evaluate(); }
};
