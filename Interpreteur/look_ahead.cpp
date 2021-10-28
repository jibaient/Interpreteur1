#define _USE_MATH_DEFINES

#include "look_ahead.h"
#include "Param_machine.h"
#include <cmath>
#include <algorithm>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


namespace ublas = boost::numeric::ublas;
extern Param_machine* myMachine;

void lookAheadFunction(toolpath_t VSections, float feedrate, float machine_max_speed, float start_speed_current)
{
	VSections[1]->its_VEnd = 0;

	std::cout << "\n look ahead : " << std::endl;
	double Vstart_i = 0;
	double Vend_i = 0;
	double Vend_im1 = 0;
	
	for (int i = VSections.size()-1; i > 0; i--) // Parcours le buffer contenant N sections
	{
		Vstart_i = 0;
		Vend_i = Vend_im1;
		double V1; // corner speed = start speed de i
		double V2; // vitesse de départ d'une section permettant la décélération jusq'à la vitesse de fin de section
			   
		
		//calcul de V1 en fonction du type de corner 4.33
		if (VSections[i]->its_interpolation_type == "linear" and VSections[i - 1]->its_interpolation_type == "linear")
		{
			std::cout << "linear - linear : " << i << std::endl;
			V1 = determineVelocityBetweenLL(VSections[i - 1], VSections[i], feedrate, machine_max_speed);
		}
		else if(VSections[i]->its_interpolation_type == "circular" and VSections[i - 1]->its_interpolation_type == "circular")
		{
			std::cout << "circular - circular : " << i << std::endl;
			V1 = determineVelocityBetweenCC();
		}
		else if (VSections[i]->its_interpolation_type == "linear" and VSections[i - 1]->its_interpolation_type == "circular")
		{
			std::cout << "circular - linear : " << i << std::endl;
			V1 = determineVelocityBetweenCL(VSections[i - 1], VSections[i], feedrate, machine_max_speed);
		}
		else if (VSections[i]->its_interpolation_type == "circular" and VSections[i - 1]->its_interpolation_type == "linear")
		{
			std::cout << "linear - circular : " << i << std::endl;
			V1 = determineVelocityBetweenLC(VSections[i - 1], VSections[i], feedrate, machine_max_speed);
		}

		

		if (VSections[i]->its_interpolation_type =="linear") // linear
		{
			double length = determineBlockLength(VSections[i]->its_start_point, VSections[i]->its_end_point);
			float V0 = sqrt(pow(Vstart_i, 2) + 2 * myMachine->max_allowable_acceleration_Z * length);
			V2 = std::min(V0, feedrate);

			Vstart_i = std::min(V1, V2);
		}
		else // circular
		{
			Vstart_i = V1; // Devrait prendre en compte l'indice de performance voir Theory and Design of CNC Systems page 151
		}
		Vend_im1 = Vstart_i;

	}

	//
	double Vend_current = Vstart_i;
	double Vstart_current = start_speed_current;
	double decLength = abs(pow(Vstart_current, 2) - pow(Vend_current, 2)) / (2 * myMachine->max_allowable_acceleration_Z);// en [m]
	double length = determineBlockLength(VSections[0]->its_start_point, VSections[0]->its_end_point)/1000; // en [m]

	if (decLength>length)
	{
		std::cout << "decLength > length : " << decLength << " > " << length << std::endl;
		int sign = 1;
		if (Vstart_current> Vend_current)
		{
			sign = -1;
		}
		Vend_current = sqrt(pow(Vstart_current, 2) + sign * 2 * myMachine->max_allowable_acceleration_Z * length);
		std::cout << "calcul deceleration : " << sqrt(pow(Vstart_current, 2) + sign * 2 * myMachine->max_allowable_acceleration_Z * length) << std::endl;
	}

	VSections[0]->its_VEnd = Vend_current;
	VSections[1]->its_VStart = Vend_current;

	std::cout << " \n V end current : " << Vend_current << std::endl;
}

double determineVelocityBetweenLL(Toolpath_section* N1, Toolpath_section* N2, float feedrate, float machine_max_speed)
{
	double V1;
	float Tpos = myMachine->tipo;
	////méthode 2 : méthode qui marche moins bien que la méthode 1
	//double L1 = determineBlockLength(N1->its_start_point,N1->its_end_point);
	//double L2 = determineBlockLength(N2->its_start_point, N2->its_end_point);

	//double F1 = determineFeedrateMaxSpeed(N1->its_movement_type,feedrate,machine_max_speed);
	//double F2 = determineFeedrateMaxSpeed(N2->its_movement_type, feedrate, machine_max_speed);
	////std::cout << "F1 : " << F1 << std::endl;
	////std::cout << "F2 : " << F2 << std::endl;
	//double VX1 = determineSpeedAxis(F1, N1->its_start_point[0], N1->its_end_point[0], L1);
	//double VY1 = determineSpeedAxis(F1, N1->its_start_point[1], N1->its_end_point[1], L1);
	//double VZ1 = determineSpeedAxis(F1, N1->its_start_point[2], N1->its_end_point[2], L1);

	//double VX2 = determineSpeedAxis(F2, N2->its_start_point[0], N2->its_end_point[0], L2);
	//double VY2 = determineSpeedAxis(F2, N2->its_start_point[1], N2->its_end_point[1], L2);
	//double VZ2 = determineSpeedAxis(F2, N2->its_start_point[2], N2->its_end_point[2], L2);

	//double dVX = abs(VX2 - VX1);
	//double dVY = abs(VY2 - VY1);
	//double dVZ = abs(VZ2 - VZ1);
	////std::cout << "vitesse : "<<" VX1 :" << VX1<<" VZ1 :" << VZ1 <<" VX2 :" << VX2 <<  VY2 << " VZ2 :" << VZ2 << " dVX :" << dVX <<" dVZ :" << dVZ << std::endl;
	//double dVXMax = myMachine->max_allowable_acceleration_X * Tpos ; //[m/s] à configurer en fonction de la machine et des essaies
	//double dVYMax = myMachine->max_allowable_acceleration_X * Tpos; //[m/s]
	//double dVZMax = myMachine->max_allowable_acceleration_Z * Tpos; //[m/s]

	//double Q = std::min(dVXMax / dVX, std::min( dVYMax / dVY, dVZMax / dVZ));

	//std::cout << "Q " << Q << std::endl;

	//if (Q<1)
	//{
	//	V1 = Q * F1;
	//}
	//else
	//{
	//	V1 = F1;
	//}

	//méthode 1
	double angle = determineAngle(N1->its_end_point, N1->its_start_point, N2->its_end_point); // sommet en première position
	std::cout << "angle en deg : " << angle * 180 / M_PI << std::endl;

	double F1 = determineFeedrateMaxSpeed(N1->its_movement_type, feedrate, machine_max_speed);
	double F2 = determineFeedrateMaxSpeed(N2->its_movement_type, feedrate, machine_max_speed);
	std::cout << "F1 : " << F1 * 60000 <<" [mm/s] " << std::endl;
	std::cout << "F2 : " << F2 * 60000 <<" [mm/s] " << std::endl;

	double Ac = (F1 - F2 * cos(angle)) / Tpos;
	std::cout << "AC :" << Ac << std::endl;
	if (Ac > std::min(myMachine->max_allowable_acceleration_X, myMachine->max_allowable_acceleration_Z))
	{
		V1 = (std::min(myMachine->max_allowable_acceleration_X, myMachine->max_allowable_acceleration_Z) * Tpos) / (1 - cos(angle));
	}
	else
	{
		V1 = feedrate;
	}
	std::cout << "V1 LL : " << V1 << " feedrate : " << feedrate << std::endl;
	return V1;
}


double determineVelocityBetweenLC(Toolpath_section* N1, Toolpath_section* N2, float feedrate, float machine_max_speed)
{
	float Tpos = myMachine->tipo;
	double V1=0;
	ublas::vector<float> center = N2->its_center;
	ublas::vector<float> start_point2 = N2->its_start_point;
	// x = az+b équation de la tangente au cercle
	double a = -(start_point2[2] - center[2])/(start_point2[0] - center[0]); // pente de la tangent au cercle en start point 
	double b = start_point2[0] - a * start_point2[2]; // b=x-a*z en start point
	ublas::vector<float> tangent_point(4);

	if (abs(a>1000000000)) //droite vertical = coeff directeur très grand
	{
		tangent_point[1] = 0;
		tangent_point[2] = start_point2[2];
		tangent_point[3] = 1;
		tangent_point[0] = start_point2[0]+1;
	}
	else
	{
		tangent_point[1] = 0;
		tangent_point[2] = 0;
		tangent_point[3] = 1;
		tangent_point[0] = a * tangent_point[2] + b;
	}

	double angle = determineAngle(N2->its_start_point,N1->its_start_point, tangent_point); // sommet en première position
	std::cout <<"a : "<<a<<" b : "<<b << " angle en deg : " << angle * 180 / M_PI << std::endl;

	double F1 = determineFeedrateMaxSpeed(N1->its_movement_type, feedrate, machine_max_speed);
	double F2 = determineFeedrateMaxSpeed(N2->its_movement_type, feedrate, machine_max_speed);
	std::cout << "F1 : " << F1 * 60000 << " [mm/s] " << std::endl;
	std::cout << "F2 : " << F2 * 60000 << " [mm/s] " << std::endl;

	double Ac = (F1-F2*cos(angle))/Tpos;
	std::cout << "AC :" << Ac << std::endl;
	if (Ac > std::min(myMachine->max_allowable_acceleration_X, myMachine->max_allowable_acceleration_Z))
	{
		V1 = (std::min(myMachine->max_allowable_acceleration_X, myMachine->max_allowable_acceleration_Z) * Tpos) / (1 - cos(angle));
	}
	else
	{
		V1 = feedrate; // A FINIR
	}
	std::cout << "V1 LC : " << V1 << " feedrate : " << feedrate<<std::endl;
	return V1;
}

double determineVelocityBetweenCL(Toolpath_section* N1, Toolpath_section* N2, float feedrate, float machine_max_speed)
{
	float Tpos = myMachine->tipo;
	double V1 = 0;
	ublas::vector<float> center = N1->its_center;
	ublas::vector<float> end_point1 = N1->its_end_point;
	// x = az+b équation de la tangente au cercle
	double a = -1 / ((end_point1[0] - center[0]) / (end_point1[2] - center[2])); // pente de la tangent au cercle en start point 
	double b = end_point1[0] - a * end_point1[2]; // b=x-a*z en start point
	ublas::vector<float> tangent_point(4);

	if (abs(a > 1000000000))
	{
		tangent_point[1] = 0;
		tangent_point[2] = end_point1[2];
		tangent_point[3] = 1;
		tangent_point[0] = end_point1[0] + 1;
	}
	else
	{
		tangent_point[1] = 0;
		tangent_point[2] = 0;
		tangent_point[3] = 1;
		tangent_point[0] = a * tangent_point[2] + b;
	}

	double angle = determineAngle(N1->its_start_point, N2->its_start_point, tangent_point);
	std::cout << "a : " << a << " b : " << b << " angle traduit en degre : " << angle * 180 / M_PI << std::endl;


	double F1 = determineFeedrateMaxSpeed(N1->its_movement_type, feedrate, machine_max_speed);
	double F2 = determineFeedrateMaxSpeed(N2->its_movement_type, feedrate, machine_max_speed);
	std::cout << "F1 : " << F1 * 60000 << " [mm/s] " << std::endl;
	std::cout << "F2 : " << F2 * 60000 << " [mm/s] " << std::endl;

	double Ac = (F1 - F2 * cos(angle)) / Tpos;

	if (Ac > std::min(myMachine->max_allowable_acceleration_X, myMachine->max_allowable_acceleration_Z))
	{
		V1 = (std::min(myMachine->max_allowable_acceleration_X, myMachine->max_allowable_acceleration_Z) * Tpos) / (1 - cos(angle));
	}
	else
	{
		V1 = feedrate;
	}
	std::cout << "V1 CL : " << V1 << " feedrate : " << feedrate << std::endl;
	return V1;
}

double determineVelocityBetweenCC()
{
	return 0.0;
}


double determineSpeedAxis(double feedrate , double startPointComponent, double endPointComponent, double blockLength)
{
	double F = feedrate; // [m/s]
	double Xs = startPointComponent; // [mm]
	double Xe = endPointComponent; // [mm]
	double L = blockLength; // [mm]
	double V; // [m/s]

	
	V = F*(Xe*0.001-Xs*0.001)/(L*0.001);
	return V;
}

double determineBlockLength(ublas::vector<float> point1, ublas::vector<float> point2)
{
	double L;
	L = sqrt(pow(point1[0] - point2[0], 2) + pow(point1[1] - point2[1], 2) + pow(point1[2] - point2[2], 2));
	return L;
}

double determineAngle(ublas::vector<float> point1, ublas::vector<float> point2, ublas::vector<float> point3)
{
	double L12 = determineBlockLength(point1, point2);
	double L13 = determineBlockLength(point1, point3);
	double L23 = determineBlockLength(point2, point3);
	double angle = acos((pow(L12, 2) + pow(L13, 2) - pow(L23, 2)) / (2 * L12 * L13));

	return M_PI - angle;
}

double determineFeedrateMaxSpeed(std::string movement_type, float feedrate, float machine_max_speed)
{
	double F;

	if (movement_type == "approach" or movement_type == "retract" or movement_type == "machining" or movement_type == "move")
	{
		F = feedrate;
	}
	else if (movement_type == "rapid_move")
	{
		F = machine_max_speed;
	}

	return F;
}
