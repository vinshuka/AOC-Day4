#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>
#include <regex>

//Day4 Part1
//Given a list of data that represent the fields on a passport, with each passport separated by a blank line, check and make sure each passport has the 
//required fields byr (Birth Year), iyr(Issue Year), eyr(Expiration Year), hgt(Height), hcl(Hair Color), ecl(Eye Color), pid(Passport ID) and 
//cid(Country ID) which is optional, each field is a key:value pair. Count the valid passports that have all the required fields.

//For this we need to struture our data in groups of key value pairs, so a list of maps, with each map having the fields for each passport, then we can
//traverse our list, and check if all of the required keys are present. We can store the required keys in a static array, and loop through it and run
//a check function using the count function for maps.

//Day4 Part2
//Now we must check that the values for each field are valid, 
//byr (Birth Year) - four digits; at least 1920 and at most 2002.
//iyr(Issue Year) - four digits; at least 2010 and at most 2020.
//eyr(Expiration Year) - four digits; at least 2020 and at most 2030.
//hgt(Height) - a number followed by either cm or in:
//If cm, the number must be at least 150 and at most 193.
//If in, the number must be at least 59 and at most 76.
//hcl(Hair Color) - a # followed by exactly six characters 0 - 9 or a - f.
//ecl(Eye Color) - exactly one of : amb blu brn gry grn hzl oth.
//pid(Passport ID) - a nine - digit number, including leading zeroes.
//cid(Country ID) - ignored, missing or not.

//For this we can create a validator function for each field, since we will remove all passports that are missing fields first. Byr, iyr, eyr all need
//a length check and range check, pid just needs a length check. hgt needs to end with either 'cm' or 'in' we can use ends with to find it, and then
//get the index of where it starts, then use substring to get the number and compare it to a range based on which length type it ends with. hcl will
//need a regex. ecl can use a list of valid eye colours and check if the value is in that list.

enum MEASUREMENTS
{
	CM,
	IN,
	NONE
};

std::regex rx ("[0-9a-f]+");
std::regex rnum("[0-9]+");

std::vector<std::string> getPuzzleInput()
{
	std::vector<std::string> input;

	std::ifstream file;
	std::string line;
	file.open("puzzle-input.txt");
	int count = 0;
	while (std::getline(file, line))
	{
		input.push_back(line);
	}

	return input;
}

std::vector<std::string> split(const std::string s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> tokens;
	while (getline(ss, item, delim))
	{
		tokens.push_back(item);
	}

	return tokens;
}

std::vector<std::map<std::string, std::string>> sortIntoMaps(std::vector<std::string> strings)
{
	std::vector<std::map<std::string, std::string>> maps;
	std::map<std::string, std::string> m;
	for (auto s : strings)
	{
		if (!s.empty())
		{
			std::vector<std::string> tokens = split(s , ' ');
			
			for (auto t : tokens)
			{
				int index = t.find(':');
				std::string field = t.substr(0, index);
				std::string value = t.substr(index + 1);
				m.insert(std::pair<std::string, std::string>(field, value));
			}
		}
		else
		{
			maps.push_back(m);
			m.clear();
		}
	}
	maps.push_back(m); //push the final map on to the vector
		
	return maps;
}

int findAnswerPart1(std::vector<std::map<std::string, std::string>> maps)
{
	std::vector<std::string> required = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
	int valid = 0;

	for (int i = 0; i < maps.size(); i++)
	{
		bool flag = true;
		for (auto r : required)
		{
			if (!maps[i].count(r) > 0)
				flag = false;
		}

		if (flag)
			valid++;
	}
	return valid;
}
// this can be used to validate byr, iyr, and eyr
bool checkValidRange(std::string value, int min, int max)
{
	int x = std::stoi(value);

	return (x >= min && x <= max);
}

MEASUREMENTS returnEnum(std::string m)
{
	if (m == "cm")
		return CM;
	if (m == "in")
		return IN;
	return NONE;
}

bool checkHeight(std::string value)
{
	std::string measurement = value.substr(value.length() - 2);
	int x = std::stoi(value.substr(0, value.length() - 2));

	switch (returnEnum(measurement))
	{
	case CM:
		return (x >= 150 && x <= 193);
	case IN:
		return (x >= 59 && x <= 76);
	case NONE:
		return false;
	}
}

bool checkHair(std::string value)
{
	if (value[0] != '#')
		return false;
	if (value.length() != 7)
		return false;
	if (!std::regex_match(value.substr(1), rx))
		return false;

	return true;
}

bool checkEye(std::string value)
{
	std::vector<std::string> eyeColours = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
	
	for (auto e : eyeColours)
	{
		if (value == e)
			return true;
	}

	return false;
}

bool checkID(std::string value)
{
	return(value.length() == 9 && std::regex_match(value, rnum));
}

std::vector<std::map<std::string, std::string>> getValidRequiredFields(std::vector<std::map<std::string, std::string>> maps)
{
	std::vector<std::string> required = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
	std::vector<std::map<std::string, std::string>> valid;
	int count = 0;
	for (int i = 0; i < maps.size(); i++)
	{
		bool flag = true;
		for (auto r : required)
		{
			if (!maps[i].count(r) > 0)
				flag = false;
		}

		if (flag)
		{
			count++;
			//std::cout << count << '\n';
			//for (auto m : maps[i])
			//{
			//	std::cout << m.first << ":" << m.second << '\n';
			//}
			//std::cout << '\n';
			valid.push_back(maps[i]);
		}
	}
	return valid;
}

int findAnswerPart2(std::vector<std::map<std::string, std::string>> validFields)
{
	int valid = 0;
	bool check = true;

	for (int i = 0; i < validFields.size(); i++)
	{
		if (!checkValidRange(validFields[i].at("byr"), 1920, 2002))
			continue;
		if (!checkValidRange(validFields[i].at("iyr"), 2010, 2020))
			continue;
		if (!checkValidRange(validFields[i].at("eyr"), 2020, 2030))
			continue;
		if (!checkHeight(validFields[i].at("hgt")))
			continue;
		if (!checkHair(validFields[i].at("hcl")))
			continue;
		if (!checkEye(validFields[i].at("ecl")))
			continue;
		if (!checkID(validFields[i].at("pid")))
			continue;

		valid++;
	}
	return valid;
}

int main()
{
	std::vector<std::string> input = getPuzzleInput();

	std::vector<std::map<std::string, std::string>> maps = sortIntoMaps(input);

	//std::cout << findAnswerPart1(maps) << '\n';

	std::vector<std::map<std::string, std::string>> validFields = getValidRequiredFields(maps);

	//std::cout << maps[0].at("byr") << '\n';

	//std::string h = "80em";
	//std::cout << checkHeight(h) << '\n';

	//std::string test = "abz123";

	//std::cout << checkHair("123abc") << '\n';

	//std::cout << checkEye("wat") << '\n';

	//std::cout << checkID("01234567a") << '\n';

	std::cout << findAnswerPart2(validFields) << '\n';

	return 0;
}