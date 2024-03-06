#ifndef RPN
#define RPN

#include <iostream>
#include <stack>
#include <string>

class Rpn
{
private:
  Rpn();
  Rpn(const Rpn &copy);
  Rpn &operator=(const Rpn &assign);
  double _answer;

public:
  double getResult();
  Rpn(std::string operand);
  ~Rpn();
  void calculate(std::string input);

  class WrongToken : public std::exception
  {
    virtual const char *what() const throw();
  };
  class DividedZero : public std::exception
  {
    virtual const char *what() const throw();
  };

  // std::stack<double> st;
};

#endif