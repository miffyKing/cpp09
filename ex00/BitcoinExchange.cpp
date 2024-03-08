#include "BitcoinExchange.hpp"
#include <cstdlib>

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

void BitcoinExchange::executeExchange(char *file)
{
  generateMap();
  checkInput(file);
}

void BitcoinExchange::generateMap()
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

void BitcoinExchange::checkInput(char *file)
{
  std::ifstream readfile;
  std::string oneline;
  int i;

  readfile.open(file);
  if (readfile.is_open())
  {
    i = 0;
    while (getline(readfile, oneline))
    {
      if (i == 0)
      {
        if (oneline != "date | value")
          throw std::runtime_error("Error: invalid header.");
      }
      else
        checkValidity(oneline);
      i++;
    }
  }
  else
    throw std::runtime_error("Error: could not open file.");
}

void BitcoinExchange::checkValidity(std::string line)
{

  std::istringstream str(line);
  std::string tokens[3]; // 토큰을 저장할 배열
  // vector<std::string> token 이거로 하는게 더 편한데 벡터 뒤에[ 쓰느라..
  std::string token;
  int tokenCount = 0;

  while (std::getline(str, token, ' ') && tokenCount < 3)
  {
    tokens[tokenCount++] = token;
  }

  if (tokenCount != 3 || tokens[1] != "|")
  {
    std::cout << "Error: bad input => " << line << std::endl;
    return;
  }

  std::string remaining;
  if (str >> remaining)
  {
    std::cout << "Error: too many arguments => " << line << std::endl;
    return;
  }

  std::string &date = tokens[0];
  // float value = stringToFloat(tokens[2]);
  float value = atof(tokens[2].c_str());

  if (!checkDate(date))
  {
    return;
  }
  if (!checkValue(tokens[2], value))
  {
    return;
  }
  printLine(date, value);
}

bool BitcoinExchange::checkNumber2(const std::string &s)
{
  for (std::string::size_type i = 0; i < s.length(); ++i)
  {
    if (!std::isdigit(s[i]))
      return false;
  }
  return true;
}

// 문자열을 정수로 변환하는 함수
int BitcoinExchange::stringToInt(const std::string &s)
{
  std::istringstream iss(s);
  int result;
  iss >> result;
  if (iss.fail() || !iss.eof())
  {
    throw std::runtime_error("Conversion failed");
  }
  return result;
}

bool BitcoinExchange::isLeapYear(int year)
{
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool BitcoinExchange::isValidDay(int year, int month, int day)
{
  if (day < 1)
    return false;
  if (month == 2)
  {
    return isLeapYear(year) ? day <= 29 : day <= 28;
  }
  if (month == 4 || month == 6 || month == 9 || month == 11)
  {
    return day <= 30;
  }
  return day <= 31;
}

bool BitcoinExchange::checkDate(std::string date)
{
  std::istringstream str(date);
  std::string date_line;

  if (date.size() != 10 || date[4] != '-' || date[7] != '-')
  {
    std::cout << "Error: bad input => " << date << std::endl;
    return false;
  }

  std::string yearStr = date.substr(0, 4);
  std::string monthStr = date.substr(5, 2);
  std::string dayStr = date.substr(8, 2);

  if (!checkNumber2(yearStr) || !checkNumber2(monthStr) || !checkNumber2(dayStr))
  {
    std::cout << "Error: bad input => " << date << std::endl;
    return false;
  }

  int year = stringToInt(yearStr);
  int month = stringToInt(monthStr);
  int day = stringToInt(dayStr);

  if (month < 1 || month > 12)
  {
    std::cout << "Error: invalid month => " << date << std::endl;
    return false;
  }
  if (year < 2009 || year > 2023)
  {
    std::cout << "Error: invalid year => " << date << std::endl;
    return false;
  }
  if (!isValidDay(year, month, day))
  {
    std::cout << "Error: invalid day => " << date << std::endl;
    return false;
  }
  return (1);
}

bool BitcoinExchange::checkValue(std::string value, float f)
{
  if (!isNumber(value, f) || ((value == "0" || value == "0.0")))
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
  // string <> float 를 atof 한 결과가, 기존string 크기와 같아야 숫자 뒤에 이상한게 없었다는 것.
  // std::ostringstream streamobj;
  // streamobj << f;
  // std::string fToStr = streamobj.str();
  // if (fToStr.size() != value.size())
  // {
  //   std::cout << "value is not correct" << fToStr << "\n";
  //   return 0;
  // }

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

  it = data_map.lower_bound(date); // date 랑 같은거 있으면 같은거, 없으면 그거보다 큰 값 중 첫번째 리턴
  if (it == data_map.end() || it->first != date)
  {
    --it;
    return (it->second);
  }
  return (it->second);
}
