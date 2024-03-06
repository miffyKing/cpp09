#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// 생성자
BitcoinExchange::BitcoinExchange(const char *price_db_path)
{
  std::ifstream price_db(price_db_path);
  if (!price_db.is_open())
    throw FileOpenException();
  prices = serialize(price_db);
}

// 소멸자
BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::Prices BitcoinExchange::serialize(
    std::istream &ifs)
{
  Prices prices;

  std::string line;
  std::string header_fmt = "date,exchange_rate";

  std::getline(ifs, line);
  if (line != header_fmt)
    throw BadHeaderException();
  // 헤더검사

  while (std::getline(ifs, line))
  {
    std::istringstream ss(line);
    std::string datePart, pricePart;
    std::getline(ss, datePart, ','); // 쉼표까지 읽고, 날짜 부분을 datePart에 저장
    std::getline(ss, pricePart);     // 남은 부분(가격)을 pricePart에 저장

    // 날짜 파싱
    std::istringstream dateStream(datePart);
    std::string yearPart, monthPart, dayPart;
    std::getline(dateStream, yearPart, '-');  // 첫 번째 하이픈까지 읽고, 년도 부분을 yearPart에 저장
    std::getline(dateStream, monthPart, '-'); // 두 번째 하이픈까지 읽고, 월 부분을 monthPart에 저장
    std::getline(dateStream, dayPart);        // 남은 부분(일)을 dayPart에 저장

    Date date;
    date.year = myStoi(yearPart);   // 문자열을 정수로 변환하여 년도 저장
    date.month = myStoi(monthPart); // 문자열을 정수로 변환하여 월 저장
    date.day = myStoi(dayPart);     // 문자열을 정수로 변환하여 일 저장

    double price = myStod(pricePart); // 문자열을 double로 변환하여 가격 저장

    if (price < 0)
      throw BadDbException(); // 가격이 음수인 경우 예외 처리

    prices[date] = price; // 날짜와 가격 정보를 prices에 저장
  }
  if (prices.size() == 0)
    throw EmptyFileException();

  return prices;
}

void BitcoinExchange::factored_price(
    std::istream &ifs)
{
  std::string line;

  std::string header_fmt = "date | value";

  std::getline(ifs, line);

  if (line != header_fmt)
    throw BadHeaderException();

  while (std::getline(ifs, line))
  {
    try
    {
      std::string dateStr, priceStr;

      // ' | '를 기준으로 날짜와 가격 부분을 분리합니다.
      size_t delimPos = line.find(" | ");
      if (delimPos == std::string::npos)
        throw BadInputException(); // 구분자가 없으면 예외를 던집니다.

      dateStr = line.substr(0, delimPos);
      priceStr = line.substr(delimPos + 3);

      // 날짜 파싱
      std::istringstream dateStream(dateStr);
      Date date;
      char delim;
      dateStream >> date.year >> delim >> date.month >> delim >> date.day;
      if (dateStream.fail())
        throw BadInputException(); // 날짜 파싱 실패 시 예외 처리

      // 가격 파싱
      double price = std::stod(priceStr); // 문자열을 double로 변환

      if (price < 0)
        throw NotPositiveNumberException();
      if (price > 1000)
        throw TooLargeNumberException();

      Prices::const_iterator it = prices.find(date);
      if (it == prices.end())
      {
        it = prices.lower_bound(date);
        if (it != prices.begin())
          --it;
      }
      if (date < it->first)
        throw BadInputException();
      std::cout //<< it->first.year << '-' << it->first.month << '-' << it->first.day << " : "
          << date.year << '-' << date.month << '-' << date.day << " => "
          << price
          //<< " * " << it->second
          << " = " << price * (it->second) << std::endl;
    }
    catch (const BadInputException &e)
    {
      std::cerr << "Error: " << e.what() << line << std::endl;
    }
    catch (const std::exception &e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  }
}

void BitcoinExchange::delim(std::istream &is, std::string fmt)
{
  for (std::string::const_iterator it = fmt.begin(); it != fmt.end(); ++it)
  {
    if (is.get() != *it)
      throw BadInputException();
  }
}

int BitcoinExchange::myStoi(const std::string &str)
{
  std::istringstream iss(str);
  int result;
  iss >> result;
  if (iss.fail())
  {
    // 오류 처리: 변환 실패
    throw std::runtime_error("Conversion failed");
  }
  return result;
}

// 문자열에서 double로 변환
double BitcoinExchange::myStod(const std::string &str)
{
  std::istringstream iss(str);
  double result;
  iss >> result;
  if (iss.fail())
  {
    // 오류 처리: 변환 실패
    throw std::runtime_error("Conversion failed");
  }
  return result;
}

// 예외 what() 구현
const char *BitcoinExchange::NotPositiveNumberException::what() const throw()
{
  return "not a positive number.";
}

const char *BitcoinExchange::FileOpenException::what() const throw()
{
  return "could not open file.";
}

const char *BitcoinExchange::BadHeaderException::what() const throw()
{
  return "there's problem in the header.";
}

const char *BitcoinExchange::BadInputException::what() const throw()
{
  return "bad input => ";
}

const char *BitcoinExchange::TooLargeNumberException::what() const throw()
{
  return "too large a number.";
}

const char *BitcoinExchange::EmptyFileException::what() const throw()
{
  return "empty file.";
}

const char *BitcoinExchange::BadDbException::what() const throw()
{
  return "bad db";
}

std::istream &operator>>(std::istream &ss, Date &date)
{
  ss >> date.year;
  if (date.year < 1970 || date.year > 2038 || ss.get() != '-')
  {
    ss.setstate(std::ios_base::failbit);
    return ss;
  }
  ss >> date.month;
  if (date.month < 1 || date.month > 12 || ss.get() != '-')
  {
    ss.setstate(std::ios_base::failbit);
    return ss;
  }
  ss >> date.day;
  if (((date.month == 1 || date.month == 3 || date.month == 5 || date.month == 7 || date.month == 8 || date.month == 10 || date.month == 12) && (date.day < 1 || date.day > 31)) ||
      ((date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11) && (date.day < 1 || date.day > 30)) ||
      ((date.month == 2) && (((date.year % 4 == 0) && (date.year % 100 != 0 || date.year % 400 == 0)) && (date.day < 1 || date.day > 29))) ||
      ((date.month == 2) && (!((date.year % 4 == 0) && (date.year % 100 != 0 || date.year % 400 == 0)) && (date.day < 1 || date.day > 28))))
  {
    ss.setstate(std::ios_base::failbit);
    return ss;
  }
  return ss;
}

bool Date::operator<(const Date &rhs) const
{
  return std::make_pair(year, std::make_pair(month, day)) < std::make_pair(rhs.year, std::make_pair(rhs.month, rhs.day));
}