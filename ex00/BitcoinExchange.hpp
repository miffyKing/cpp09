#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

class BitcoinExchange
{
private:
	BitcoinExchange(const BitcoinExchange &ref);
	BitcoinExchange &operator=(const BitcoinExchange &ref);

	std::map<std::string, float> data_map;
	void checkInput(char *file);
	void checkValidity(std::string line);
	bool checkDate(std::string date);
	bool checkValue(std::string value, float f);
	bool isNumber(std::string value, float f);
	void printLine(std::string date, float value);
	float getValue(std::string date);
	void makeMap();
	bool checkNumber2(const std::string &s);
	int stringToInt(const std::string &s);
	// float stringToFloat(const std::string &s);
	bool isLeapYear(int year);
	bool isValidDay(int y, int m, int d);

public:
	BitcoinExchange();

	~BitcoinExchange();
	void executeExchange(char *file);
};

#endif