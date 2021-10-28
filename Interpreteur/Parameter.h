#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <string>
#include <map>
#include <iostream>

class Property_parameter
{
public:
	Property_parameter();
	Property_parameter(std::string num);
protected:
	std::string parameter_name;
};

class Desciptive_parameter :
	public Property_parameter
{
public:
	Desciptive_parameter();
	Desciptive_parameter(std::string num);
protected:
	std::string descriptive_string;
};

class Numeric_parameter :
	public Property_parameter
{
public:
	Numeric_parameter();
	Numeric_parameter(std::string num);
	float getParameter_value() const;
protected:
	float its_parameter_value;
	std::string its_parameter_unit;
};

#endif // PARAMETER_HPP