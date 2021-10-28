#include "toolpath.h"
#include "Executable.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace std;
namespace ublas = boost::numeric::ublas;

/*
* Une section de parcours d'outils correspond au déplacement entre 2 points du parcours d'outils
* Il est défini par son point d'arriver, son type (circulaire, linéaire)
*/


Toolpath_section::Toolpath_section()
{
}

Toolpath_section::Toolpath_section(ublas::vector<float> start_point,ublas::vector<float> end_point, std::string interpolation_type, std::string movement_type)
{
	its_start_point = start_point;
	its_end_point = end_point;
	its_interpolation_type = interpolation_type;
	its_movement_type = movement_type;
	its_parameter = NAN;
}

Toolpath_section::Toolpath_section(ublas::vector<float> start_point, ublas::vector<float> end_point, std::string interpolation_type, std::string movement_type, float parameter, ublas::vector<float> center)
{
	its_start_point = start_point;
	its_end_point = end_point;
	its_interpolation_type = interpolation_type;
	its_movement_type = movement_type;
	its_parameter = parameter;
	its_center = center;
}

void Toolpath_section::coutTPS()
{
	std::cout << "start point : " << "(" << its_start_point[0] << "," << its_start_point[1] << "," << its_start_point[2] << ")" << " | ";
	std::cout << "end point" << "(" << its_end_point[0] << "," << its_end_point[1] << "," << its_end_point[2] << ")" << " : " << its_interpolation_type << " " << its_movement_type;
	
	if (its_parameter == its_parameter)
	{
		std::cout << " parameter : " << its_parameter <<" center : "<<its_center << std::endl;
	}
	else
	{
		std::cout << endl;
	}
	std::cout << " V start : " << its_VStart * 60000 << " [mm/min] | V end : " << its_VEnd *60000<<" [mm/min]" << std::endl;
}



void coutTP(toolpath_t toolpath)
{
	for (auto element : toolpath)
	{
		element->coutTPS();
	}
}


void plotPy(toolpath_t toolpath)
{
	ofstream myfile;
	myfile.open("point.txt");
	for (auto element : toolpath)
	{
		myfile << element->its_end_point[0] << "," << element->its_end_point[1] << "," << element->its_end_point[2] <<"," << element->its_interpolation_type << "," << element->its_movement_type << "," << endl;
	}
	myfile.close();
}

void plotPyInterpolation(toolpath_t toolpath)
{
	ofstream myfile;
	myfile.open("pointInter.txt");
	for (auto element : toolpath)
	{
		for (auto point : element->its_rough_interpolation)
		{
			myfile << point[0] << "," << point[1] << "," << point[2] << "," << endl;
		}
		
	}
	myfile.close();
}


