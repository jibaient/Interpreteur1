#include "Parameter.h"
#include <string>
#include <map>
#include <iostream>
#include "traitement_fichier.h"
using namespace std;
extern map<string, string> mapEntite;
Property_parameter::Property_parameter()
{
}

Property_parameter::Property_parameter(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	parameter_name = param[0];

}
Desciptive_parameter::Desciptive_parameter()
{
}
Desciptive_parameter::Desciptive_parameter(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	parameter_name = param[0];
	cout << param[0] << " property parameter" << endl; // !!!SUPPR!!! //
	descriptive_string = param[1];
}
Numeric_parameter::Numeric_parameter()
{
}
Numeric_parameter::Numeric_parameter(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	parameter_name = param[0];
	cout << param[0] << " property parameter" << endl; // !!!SUPPR!!! //
	its_parameter_value = stoi(param[1]);
	its_parameter_unit = param[0];
}

float Numeric_parameter::getParameter_value() const
{
	return its_parameter_value;
}
