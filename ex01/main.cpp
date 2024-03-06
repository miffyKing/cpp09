#include <iostream>
#include "Rpn.hpp"

int main(int argc, char** argv)
{
  if (argc == 1)
  {
    std::cout<<"Not enough parameter\n";
    return 0;
  }
  if (argc > 2)
  {
    std::cout<< "Too many parameter\n";
    return 0;
  }
  if (argv[1] == NULL)
  {
    std::cout<<"No expressions\n";
    return 0;
  }

  try {
    Rpn rpn(argv[1]);
    std::cout << rpn.getResult() << "\n";

  } catch (std::exception &e)
  {
   std::cout<<"Error\n";
   return 0;
  }

  return 0;
}
