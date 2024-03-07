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
	void checkInputFile(char *file);
	void checkValidInfo(std::string line);
	bool checkDate(std::string date);
	bool checkValue(std::string value, float f);
	bool isNumber(std::string value, float f);
	void printLine(std::string date, float value);
	float getValue(std::string date);
	void makeMap();

public:
	BitcoinExchange();

	~BitcoinExchange();
	void executeExchange(char *file);
};

#endif