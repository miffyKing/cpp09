
#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &ref)
{
  *this = ref;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &ref)
{
  if (this != &ref)
    this->data_map = ref.data_map;
  return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::makeMap()
{
  std::ifstream csv;
  std::string oneline;
  int i;
  size_t commalocation;
  float price;

  csv.open("data.csv");
  if (csv.is_open())
  {
    i = 0;
    while (getline(csv, oneline))
    {
      if (i == 0 && oneline != "date,exchange_rate")
        throw std::runtime_error("Error: invalid header.");
      else if (i > 0)
      {
        commalocation = oneline.find(',');
        std::istringstream str(oneline.substr(commalocation + 1, oneline.length()));
        str >> price;
        data_map[oneline.substr(0, commalocation)] = price;
      }
      i++;
    }
  }
  else
    throw std::runtime_error("Error: could not open file.");
}

void BitcoinExchange::executeExchange(char *file)
{
  makeMap();
  checkInputFile(file);
}

void BitcoinExchange::checkInputFile(char *file)
{
  std::ifstream readfile;
  std::string read;
  int i;

  readfile.open(file);
  if (readfile.is_open())
  {
    i = 0;
    while (getline(readfile, read))
    {
      if (i == 0 && read != "date | value")
        throw std::runtime_error("Error: invalid header.");
      else if (i > 0)
      {
        checkValidInfo(read);
      }
      i++;
    }
  }
  else
    throw std::runtime_error("Error: could not open file.");
}

void BitcoinExchange::checkValidInfo(std::string line)
{
  std::istringstream str(line);
  std::string s;
  std::string date;
  float value;
  int i;

  i = 0;
  while (std::getline(str, s, ' '))
  {
    if (i == 0)
    {
      if (!checkDate(s))
        return;
      date = s;
    }
    else if (i == 1 && s != "|")
    {
      std::cout << "Error: bad input => " << line << std::endl;
      return;
    }
    else if (i == 2)
    {
      value = std::atof(s.c_str());
      if (!checkValue(s, value))
        return;
    }
    i++;
  }
  if (i != 3)
  {
    std::cout << "Error: bad input => " << s << std::endl;
    return;
  }
  printLine(date, value);
}

bool BitcoinExchange::checkDate(std::string date)
{
  std::istringstream str(date);
  std::string date_line;
  int year;
  int month;
  int day;
  int i;
  if (date.size() != 10)
  {
    std::cout << "Error: bad input => " << date << std::endl;
    return (0);
  }
  else if (date.find('-') != 4 || date.find('-', 7) != 7)
  {
    std::cout << "Error: bad input => " << date << std::endl;
    return (0);
  }
  i = 0;
  while (std::getline(str, date_line, '-'))
  {
    if (i == 0)
    {
      year = std::atof(date_line.c_str());
      if (date_line.length() != 4 || year < 2009 || year > 2023)
      {
        std::cout << "Error: bad input => " << date << std::endl;
        return (0);
      }
    }
    else if (i == 1)
    {
      month = std::atof(date_line.c_str());
      if (date_line.length() != 2 || month < 1 || month > 12)
      {
        std::cout << "Error: bad input => " << date << std::endl;
        return (0);
      }
    }
    else if (i == 2)
    {
      day = std::atof(date_line.c_str());
      if (date_line.length() != 2 || day < 1 || day > 31)
      {
        std::cout << "Error: bad input => " << date << std::endl;
        return (0);
      }
      if (month == 2)
      {
        if (day > 29)
        {
          std::cout << "Error: bad input => " << date << std::endl;
          return (0);
        }
        if (day == 29 && !(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
        {
          std::cout << "Error: bad input => " << date << std::endl;
          return (0);
        }
      }
      if (day == 31 && (month == 4 || month == 6 || month == 9 || month == 11))
      {
        std::cout << "Error: bad input => " << date << std::endl;
        return (0);
      }
    }
    i++;
  }
  return (1);
}

bool BitcoinExchange::checkValue(std::string value, float f)
{
  if (!isNumber(value, f) || ((value == "0" || value == "0.0") && f != 0))
  {
    std::cout << "Error: bad input => " << value << std::endl;
    return (0);
  }
  else if (f > 1000)
  {
    std::cout << "Error: too large a number." << std::endl;
    return (0);
  }
  else if (f < 0)
  {
    std::cout << "Error: not a positive number." << std::endl;
    return (0);
  }
  return (1);
}

bool BitcoinExchange::isNumber(std::string value, float f)
{
  int i;
  int dot_flag;
  int minus_flag;

  if (!value.empty() && value.find_first_not_of("0123456789.-") != std::string::npos)
    return (0);
  i = 0;
  dot_flag = 0;
  minus_flag = 0;
  while (value[i])
  {
    if (value[i] == '.')
      dot_flag++;
    if (value[i] == '-')
      minus_flag++;
    i++;
  }
  if (dot_flag > 1 || minus_flag > 1 || (f > 0 && value.find('-') != std::string::npos))
    return (0);
  return (1);
}

void BitcoinExchange::printLine(std::string date, float value)
{
  float new_value;

  new_value = getValue(date) * value;
  std::cout << date << " => " << value << " = " << new_value << std::endl;
}

float BitcoinExchange::getValue(std::string date)
{
  std::map<std::string, float>::iterator it;

  it = data_map.lower_bound(date);
  if (it == data_map.end() || it->first != date)
  {
    --it;
    return (it->second);
  }
  return (it->second);
}
