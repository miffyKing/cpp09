#include <iostream>
#include <fstream>
#include "BitcoinExchange.hpp"

int main(int argc, char **argv)
{
  std::cout << argv[0];
  if (argc != 2)
  {
    std::cout << "Error: could not open file.\n";
    return 0;
  }

  try
  {
    BitcoinExchange bitExchanger("data.csv");

    std::ifstream rate_db(argv[1]);
    if (!rate_db.is_open())
      std::cout << "Error: could not open file.";

    bitExchanger.factored_price(rate_db);
  }
  catch (const std::exception &e)
  {
    std::cout << "Error: " << e.what() << "\n";
    return 0;
  }

  return 0;
}