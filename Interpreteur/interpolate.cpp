#include "interpolate.h"

#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "toolpath.h"
#include "look_ahead.h"
#include "Param_machine.h"
#define PI 3.14159265

extern Param_machine* myMachine;

void linearIPO_Pre(Toolpath_section* section)
{
	std::cout << section->its_interpolation_type << "  " << section->its_movement_type << std::endl;
	std::vector< ublas::vector<float>> interpolation_points;
	std::vector<float> length_movement;
	double S = 0;
	double BL = determineBlockLength(section->its_start_point, section->its_end_point); // [mm]
	std::cout << "L mm : " << BL << std::endl;

	std::vector<float> SP = section->its_speed_profile;
	double Tipo = myMachine->tipo;
	double Vim1 = 0;

	for (auto Vi : SP)
	{
		double Si = (Tipo * ((Vi + Vim1) / 2))*1000; // [mm]
		S += Si;
		length_movement.push_back(S);
		Vim1 = Vi;
	}

	double Srem = BL-S;
	double eps = Srem / length_movement.size();
	std::cout << "S : " << S << std::endl;
	std::cout << "Srem : " << Srem << std::endl;
	std::cout << "eps : " << eps << std::endl;

	for (int i = 0; i < length_movement.size(); i++)
	{
		length_movement[i] += eps; // on ajoute la longeur remaining
	}

	//Détermination de la position
	double m = 0; //coeff directeur de la droite [start point, end point]
	float signeDx = (section->its_end_point[0] - section->its_start_point[0])/ abs(section->its_end_point[0] - section->its_start_point[0]);
	float signeDz = (section->its_end_point[2] - section->its_start_point[2]) / abs(section->its_end_point[2] - section->its_start_point[2]);
	m = (section->its_end_point[0] - section->its_start_point[0]) / (section->its_end_point[2] - section->its_start_point[2]);
	std::cout << "m : " << m << std::endl;
	std::cout << "end point[0] " << section->its_end_point[0]<< " start point[0] " << section->its_start_point[0] << std::endl;
	std::cout << "end point[2] " << section->its_end_point[2] << " start point[2] " << section->its_start_point[2] << std::endl;


	if (m < -999999 or m>999999)
	{
		std::cout << "m = -inf" << std::endl;

		for (auto L : length_movement)
		{
			double dx = L;

			ublas::vector<float> interpolated_point(4); // point de départ de la matière à usiner
			interpolated_point(0) = signeDx * dx + section->its_start_point[0];
			interpolated_point(1) = 0;
			interpolated_point(2) = section->its_start_point[2];
			interpolated_point(3) = 1;
			interpolation_points.push_back(interpolated_point);
		}
	}
	else if(m==0)
	{
		std::cout << "m = 0" << std::endl;

		for (auto L : length_movement)
		{
			double dz = L;

			ublas::vector<float> interpolated_point(4); // point de départ de la matière à usiner
			interpolated_point(0) = section->its_start_point[0];
			interpolated_point(1) = 0;
			interpolated_point(2) = signeDz * dz + section->its_start_point[2];
			interpolated_point(3) = 1;
			interpolation_points.push_back(interpolated_point);
		}
	}
	else
	{
		for (auto L : length_movement)
		{

			double dz = sqrt(pow(L, 2) / (1 + pow(m, 2)));// déplacement suivant l'axe Z
			double dx = abs(m) * dz;

			ublas::vector<float> interpolated_point(4); // point de départ de la matière à usiner
			interpolated_point(0) = signeDx * dx + section->its_start_point[0];
			interpolated_point(1) = 0;
			interpolated_point(2) = signeDz * dz + section->its_start_point[2];
			interpolated_point(3) = 1;
			interpolation_points.push_back(interpolated_point);
		}
	}

	double error = sqrt(pow(interpolation_points.back()[0] - section->its_end_point [0], 2) + pow(interpolation_points.back()[1] - section->its_end_point[1], 2) + pow(interpolation_points.back()[2] - section->its_end_point[2], 2));
	double errorRelative = error / BL;
	std::cout << "error % : " << errorRelative*100 << std::endl;
	std::cout << interpolation_points.back() << " | " << section->its_end_point << std::endl;
	section->its_rough_interpolation = interpolation_points;
}

void circleIPO_Pre(Toolpath_section* section)
{
	std::cout << section->its_interpolation_type << "  " << section->its_movement_type << std::endl;
	std::vector< ublas::vector<float>> interpolation_points;
	std::vector<float> length_movement;
	double theta = 0;
	double theta0 = acos((section->its_start_point[2]-section->its_center[2])/(section->its_parameter)); // [rad]
	double thetaTotal = determineAngle(section->its_center, section->its_start_point, section->its_end_point);
	std::cout << "angle depart deg : " << theta0*180/3.1415 << std::endl;
	std::cout << "angle total def: " << thetaTotal * 180 / 3.1415 << std::endl;

	std::vector<float> SP = section->its_speed_profile;
	double Tipo = myMachine->tipo;
	double Wim1 = 0;

	for (auto Wi : SP)
	{
		double thetai = (Tipo * ((Wi + Wim1) / 2)); // [rad]
		theta += thetai;
		length_movement.push_back(theta);
		Wim1 = Wi;
	}

	double thetaRem = thetaTotal - theta;
	double eps = thetaRem / length_movement.size();
	std::cout << "theta : " << theta << std::endl;
	std::cout << "thetaRem : " << thetaRem << std::endl;
	std::cout << "eps : " << eps << std::endl;

	for (int i = 0; i < length_movement.size(); i++)
	{
		length_movement[i] += eps; // on ajoute la longeur remaining
	}

	//Détermination de la position

	std::cout << "end point[0] " << section->its_end_point[0] << " start point[0] " << section->its_start_point[0] << std::endl;
	std::cout << "end point[2] " << section->its_end_point[2] << " start point[2] " << section->its_start_point[2] << std::endl;

	for (auto beta : length_movement)
	{
		ublas::vector<float> interpolated_point(4); // point de départ de la matière à usiner
		interpolated_point(0) = section->its_parameter * sin(beta+theta0) + section->its_center[0];
		interpolated_point(1) = 0;
		interpolated_point(2) = section->its_parameter * cos(beta+theta0) + section->its_center[2];
		interpolated_point(3) = 1;
		interpolation_points.push_back(interpolated_point);
	}

	double error = sqrt(pow(interpolation_points.back()[0] - section->its_end_point[0], 2) + pow(interpolation_points.back()[1] - section->its_end_point[1], 2) + pow(interpolation_points.back()[2] - section->its_end_point[2], 2));
	double errorRelative = error / determineBlockLength(section->its_start_point,section->its_end_point);
	std::cout << "error % : " << errorRelative * 100 << std::endl;
	std::cout << interpolation_points.back() << " | " << section->its_end_point << std::endl;
	section->its_rough_interpolation = interpolation_points;
}

void mapping(Toolpath_section* section)
{
	float xm1=0;
	float zm1=0;
	float x1=0;
	float z1=0;
	bool notFirst = false;
	for (auto element : section->its_rough_interpolation )
	{
		x1 = element[0];
		z1 = element[2];
		if (notFirst)
		{
			section->z_displacement.push_back(z1-zm1);
			section->x_displacement.push_back(x1-xm1);
		}
		notFirst = true;
		xm1 = x1;
		zm1 = z1;
	}

}
