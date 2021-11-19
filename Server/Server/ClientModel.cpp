#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cctype>

#include "ClientModel.h"

std::vector<std::string> splitStr(std::string& inputStr, char separator)
{
	std::string tmpBuff{ "" };
	std::vector<std::string> resultVector;

	std::string convertedPChar(inputStr);

	for (auto i : convertedPChar)
	{
		if (i != separator)
		{
			tmpBuff.push_back(i);
		}
		else if (i == separator && tmpBuff != "")
		{
			resultVector.push_back(tmpBuff);
			tmpBuff = "";
		}
	}

	if (tmpBuff != "")
		resultVector.push_back(tmpBuff);
	return resultVector;
}

ClientModel::ClientModel()
{
}

ClientModel::~ClientModel()
{
	
}

std::pair<std::string, errors> ClientModel::GetIterations(std::string& str)
{
	std::vector<std::string> splitedStr = splitStr(str,' ');
	
	if (splitedStr.size() < 3)
	{
		return std::make_pair("0", fewArguments);
	}

	for (auto& i : splitedStr)
	{
		for (int j = 0; j < i.size(); j++)
		{
			if (isalpha(i[j]))
				return std::make_pair("0", alphaInNumber);
		}
	}

	long long iteration = 0; 
	std::vector<int> numbers;
	for (auto& i : splitedStr)
	{
		int iIteratin = atoi(i.c_str());
		if (iIteratin > 10000)
		{
			return std::make_pair("0", bigInt);
		}
		if (iIteratin == 0)
		{
			return std::make_pair(std::to_string(iteration), Corrected);
		}
		//iteration = iteration == 0 ? iIteratin : iteration * iIteratin;
		numbers.push_back(iIteratin);
	}

	for (int i = 0; i < numbers[0]; ++i)
		for (int j = 0; j < numbers[1]; ++j)
			for (int k = 0; k < numbers[2]; ++k)
				++iteration;

	return std::make_pair(std::to_string(iteration), Corrected);
}
