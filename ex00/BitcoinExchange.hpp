#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>

struct Date
{
	int year;
	int month;
	int day;

	bool operator<(const Date &rhs) const;
};

class BitcoinExchange
{
public:
	// 생성자
	BitcoinExchange(const char *price_db_path);

	// 소멸자
	~BitcoinExchange();

	// 예외 클래스 정의
	class NotPositiveNumberException : public std::exception
	{
		virtual const char *what() const throw();
	};
	class FileOpenException : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
	class BadHeaderException : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
	class BadInputException : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
	class TooLargeNumberException : public std::exception
	{
		virtual const char *what() const throw();
	};
	class EmptyFileException : public std::exception
	{
		virtual const char *what() const throw();
	};
	class BadDbException : public std::exception
	{
		virtual const char *what() const throw();
	};

	void factored_price(std::istream &ifs);

private:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &copy);
	BitcoinExchange &operator=(const BitcoinExchange &assign);

	typedef std::map<Date, double> Prices;

	Prices serialize(std::istream &ifs);
	void delim(std::istream &is, std::string fmt);

	Prices prices;
};

std::istream &operator>>(std::istream &ss, Date &date);

#endif