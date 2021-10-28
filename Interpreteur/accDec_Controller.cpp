
#include "accDec_Controller.h"
#include "Param_machine.h"
#include "toolpath.h"
#include "look_ahead.h"

#include <string>
#include <map>
#include <iostream>
#include <cmath>
#include <algorithm>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace ublas = boost::numeric::ublas;
extern Param_machine* myMachine;

void determineVelocityProfile(Toolpath_section* section, float feedrate)
{
	if (section->its_interpolation_type == "linear")
	{
		if (isNormalBlock(section,feedrate))
		{
			std::cout << "linear normal block" << std::endl;
			lineNormalBlock(section,feedrate);
		}
		else
		{
			std::cout << "linear short block" << std::endl;
			lineSmallBlock(section);
		}
	}
	else if (section->its_interpolation_type == "circular")
	{
		if (isNormalBlock(section,feedrate))
		{
			std::cout << "circular normal block" << std::endl;
			circleNormalBlock(section, feedrate);
			
		}
		else
		{
			//circleSmallBlock();
			std::cout << "circular short block" << std::endl;
		}
	}
	else
	{
		std::cout << "probleme dans le type d'interpolation dans le determineVelocityProfile" << std::endl;
	}
}

void lineNormalBlock(Toolpath_section* section, float feedrate)
{
	std::vector<float> speed_profile;
	
	double maxAcc = std::min(myMachine->max_allowable_acceleration_X, myMachine->max_allowable_acceleration_Z);
	double tipo = myMachine->tipo;
	double tAcc = (feedrate - section->its_VStart) / maxAcc; //durée de la phase d'accélération [s]
	double tDec = (feedrate - section->its_VEnd ) / maxAcc; // durée de la phase de décélération [s]
	double tConst = 0; // durée de la phase à vitesse constante [s]

	int jAcc = int((tAcc / tipo) + 1); //durée de l'accélération en terme de nombre d'interpolation
	double Acc = (feedrate - section->its_VStart) / (jAcc * tipo); // calcul de l'accélération réel
	double dAcc = abs(section->its_VStart * tAcc + 0.5 * maxAcc * pow(tAcc, 2));


	int jDec = int((tDec / tipo) + 1); // de même pour la phase de décéleration
	double Dec = (section->its_VEnd - feedrate) / (jDec * tipo);
	double dDec = abs(section->its_VEnd * tDec + 0.5 * maxAcc * pow(tDec, 2));

	double blockLength = determineBlockLength(section->its_start_point, section->its_end_point)/1000; //[m]
	tConst = (blockLength-(dAcc+dDec))/(feedrate);
	int jConst = int(tConst / tipo)+1;

	double L = (blockLength - (dAcc + dDec));


	float Vi = 0;
	std::cout << "dconst : " << L * 1000 << " dAcc : " << dAcc * 1000 << " dDec : " << dDec * 1000 << std::endl;
	std::cout << "maxAcc : " << maxAcc << " Acc : " << Acc << " Dec : " << Dec << std::endl;
	std::cout << "vstart : " <<section->its_VStart *60000 << "  Vend : " <<section->its_VEnd *60000 << " feedrate : "<<feedrate *60000<< std::endl;
	std::cout << "jAcc : " << jAcc << " jConst : " << jConst << " jDec : " << jDec << std::endl;

	float l = 0;
	for (int i = 0; i < jAcc; i++)
	{
		speed_profile.push_back(Vi);
		//std::cout << "Vi acc : " << Vi * 60000 << " time : " << i * tipo << std::endl;
		l += tipo * Vi;
		Vi = speed_profile.back()+Acc*tipo;
	}
	for (int i = 0; i < jConst; i++)
	{
		speed_profile.push_back(Vi);
		//std::cout << "Vi const : " << Vi * 60000 << " time : " << i * tipo  <<std::endl;
		Vi = speed_profile.back();
		l += tipo * Vi;
	}
	for (int i = 0; i < jDec; i++)
	{
		speed_profile.push_back(Vi);
		//std::cout << "Vi dec : " << Vi * 60000 << " time : " << i * tipo  << std::endl;
		Vi = speed_profile.back() + Dec * tipo;
		l += tipo * Vi;
	}

	double erreur = abs(((blockLength - l) / blockLength) * 100);

	section->its_speed_profile = speed_profile;
}

void lineSmallBlock(Toolpath_section* section)
{
	double maxAcc = std::min(myMachine->max_allowable_acceleration_X, myMachine->max_allowable_acceleration_Z);
	double blockLength = determineBlockLength(section->its_start_point, section->its_end_point) / 1000; //[m]
	double vMax = sqrt((blockLength * 2 * maxAcc + pow(section->its_VStart, 2) + pow(section->its_VEnd, 2)) / 2);
	lineNormalBlock(section, vMax);
}

void circleNormalBlock(Toolpath_section* section, float feedrate)
{
	std::vector<float> speed_profile;
	
	double radius = (section->its_parameter / 1000); // [m]
	double wMax = feedrate / radius; // vitesse angulaire max  [rad/s]
	double wEnd = section->its_VEnd / radius;
	double wStart = section->its_VStart / radius;
	
	double wMaxAcc = std::min(myMachine->max_allowable_acceleration_X, myMachine->max_allowable_acceleration_Z)/radius;
	double wTipo = myMachine->tipo;
	std::cout << "wTipo : " << wTipo <<" tipo " << myMachine->tipo <<std::endl;
	double tAcc = (wMax - wStart) / wMaxAcc; //durée de la phase d'accélération [s]
	double tDec = (wMax - wEnd) / wMaxAcc; // durée de la phase de décélération [s]
	double tConst = 0; // durée de la phase à vitesse constante [s]

	int jAcc = int((tAcc / wTipo) + 1); //durée de l'accélération en terme de nombre d'interpolation
	double Acc = (wMax - wStart) / (jAcc * wTipo); // calcul de l'accélération réel
	double dAcc = abs(wStart * tAcc + 0.5 * wMaxAcc * pow(tAcc, 2));


	int jDec = int((tDec / wTipo) + 1); // de même pour la phase de décéleration
	double Dec = (wEnd - wMax) / (jDec * wTipo);
	double dDec = abs(wEnd * tDec + 0.5 * wMaxAcc * pow(tDec, 2));

	//double blockLength = determineBlockLength(wStart, wEnd) / 1000; //[m]
	double blockAngle = determineAngle(section->its_center,section->its_start_point,section->its_end_point); // test longeur angulaire du bloc ???? => calcul de l'angle entre end start center
	std::cout << "angle en rad : " << blockAngle << std::endl;
	
	tConst = (blockAngle - (dAcc + dDec)) / (wMax);
	int jConst = int(tConst / wTipo) + 1;

	double L = (blockAngle - (dAcc + dDec));


	float Vi = 0;
	std::cout << "dconst : " << L * 1000 << " dAcc : " << dAcc * 1000 << " dDec : " << dDec * 1000 << std::endl;
	std::cout << "maxAcc : " << wMaxAcc << " Acc : " << Acc << " Dec : " << Dec << std::endl;
	std::cout << "vstart : " << section->its_VStart * 60000 << "  Vend : " << section->its_VEnd * 60000 << " feedrate : " << feedrate * 60000 << std::endl;
	std::cout << "jAcc : " << jAcc << " jConst : " << jConst << " jDec : " << jDec << std::endl;

	float l = 0;
	for (int i = 0; i < jAcc; i++)
	{
		speed_profile.push_back(Vi);
		//std::cout << "Vi acc : " << Vi * 60000 << " time : " << i * tipo << std::endl;
		l += sin(wTipo * Vi)*radius;
		Vi = speed_profile.back() + Acc * wTipo;
	}
	for (int i = 0; i < jConst; i++)
	{
		speed_profile.push_back(Vi);
		//std::cout << "Vi const : " << Vi * 60000 << " time : " << i * tipo << std::endl;
		Vi = speed_profile.back();
		l += sin(wTipo * Vi) * radius;
	}
	for (int i = 0; i < jDec; i++)
	{
		speed_profile.push_back(Vi);
		//std::cout << "Vi dec : " << Vi * 60000 << " time : " << i * tipo << std::endl;
		Vi = speed_profile.back() + Dec * wTipo;
		l += sin(wTipo * Vi) * radius;
	}

	double erreur = abs(((radius * blockAngle - l) / radius * blockAngle) * 100);

	section->its_speed_profile = speed_profile;
}

bool isNormalBlock(Toolpath_section* section, float feedrate)
{
	float maxAllowableAcc = std::min(myMachine->max_allowable_acceleration_X, myMachine->max_allowable_acceleration_Z);
	double decLength = (pow(feedrate, 2) / maxAllowableAcc)*1000; //feedrate [m/s], acc [m/s²], decLength [mm]
	double L = determineBlockLength(section->its_start_point, section->its_end_point); // [m]
	std::cout <<section->its_interpolation_type<<"  " << section->its_movement_type << " decLength : " << decLength << " L : " << L << std::endl;
	return decLength < L; // si validé, alors est un block normal, sinon est un block court
}


