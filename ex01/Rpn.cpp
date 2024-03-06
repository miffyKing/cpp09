#include "Rpn.hpp"
#include <sstream>

Rpn::Rpn()
{
}

Rpn::Rpn(std::string input)
{
  calculate(input);
}

void Rpn::calculate(std::string input)
{
  std::stringstream ss(input);
  std::string token;
  std::stack<int> numbers;

  while (ss >> token)
  {
    if (token[0] >= '0' && token[0] <= '9') 
    {
      if (token.length() != 1)
        throw(WrongToken());
      int number = token[0] - '0'; 
      numbers.push(number);
    }
    else if (token == "+" || token == "-" || token == "*" || token == "/")
    {
      if (numbers.size() < 2)
      {
        throw(WrongToken());
      }
      int val2 = numbers.top();
      numbers.pop();
      int val1 = numbers.top();
      numbers.pop();
      int result = 0;
      switch (token[0])
      {
      case '+':
        result = val1 + val2;
        break;
      case '-':
        result = val1 - val2;
        break;
      case '*':
        result = val1 * val2;
        break;
      case '/':
        if (val2 == 0)
          throw(WrongToken());
        result = val1 / val2;
        break;
      }
      numbers.push(result);
    }
    else
    {
      throw(WrongToken());
    }
  }

  if (numbers.size() != 1)
  {
    // 스택에 남은 숫자가 한 개가 아닌 경우
    throw(WrongToken());
  }
  _answer = numbers.top();
}
// 예외 what() 구현
const char *Rpn::WrongToken::what() const throw()
{
  return "Error";
}

Rpn::~Rpn() {}

double Rpn::getResult()
{
  return this->_answer;
}