#include "Technology.h"
#include "traitement_fichier.h"
#include "add_object.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;
extern map<string, string> mapEntite;


float Speed_select::getRot_speed()
{
	return 0.0f;
}
Const_spindle_speed::Const_spindle_speed()
{
}
Const_spindle_speed::Const_spindle_speed(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	rot_speed = stof(param[0]);
}

float Const_spindle_speed::getRot_speed()
{
	return rot_speed;
}

Const_cutting_speed::Const_cutting_speed()
{
}
Const_cutting_speed::Const_cutting_speed(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	speed_measure = stof(param[0]);
	if (optional(param[1])) { rot_speed = stof(param[1]); }
}

Turning_technology::Turning_technology()
{
}

Turning_technology::Turning_technology(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	if (optional(param[0])) { feedrate = stof(param[0]); }
	else { feedrate = NAN;}
	feedrate_reference = param[1];

	string nameSpindle_s = getNameEntity(mapEntite[param[2]]);
	if (nameSpindle_s == "CONST_CUTTING_SPEED")
	{
		spindle_speed = addObject<Const_cutting_speed>(param[2]);
	}
	else if (nameSpindle_s == "CONST_SPINDLE_SPEED")
	{
		spindle_speed = addObject<Const_spindle_speed>(param[2]);
	}
	if (optional(param[3])) { feedrate_per_revolution = stof(param[3]); }

	sync_spindle_and_z_feed = stringToBool(param[4]);
	inhibit_feedrate_override = stringToBool(param[5]);
	inhibit_spindle_override = stringToBool(param[6]);
}



Turning_machine_functions::Turning_machine_functions()
{
}

Turning_machine_functions::Turning_machine_functions(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	coolant = stringToBool(param[0]);
	if (optional(param[1])) { cout << "coolant type dans turning machine functions pas encore implemente" << endl; }
	if (optional(param[2])) { cout << "coolant pressure dans turning machine functions pas encore implemente" << endl; }
	if (optional(param[3])) { cout << "axis clamping dans turning machine functions pas encore implemente" << endl; }
	chip_removal = stringToBool(param[4]);
	if (optional(param[5])) { cout << "oriented spindle stop dans turning machine functions pas encore implemente" << endl; }
	if (optional(param[6])) { cout << "process model dans turning machine functions pas encore implemente"  << endl; }
	if (optional(param[7])) { cout << "other function dans turning machine functions pas encore implemente" << endl; }
	if (optional(param[8])) { tail_stock = stringToBool(param[8]); }
	if (optional(param[9])) { steady_rest = stringToBool(param[9]); }
	if (optional(param[10])) { follow_rest = stringToBool(param[10]); }
}

void Technology::calculateFeedrate()
{

}

float Technology::getFeedrate()
{
	return 0.0f;
}

void Turning_technology::calculateFeedrate()
{
	if (feedrate != feedrate) // Si le feedrate n'est pas donné, cad est un NaN
	{
		float spindle_rot_speed = spindle_speed->getRot_speed() ;
		feedrate = spindle_rot_speed * feedrate_per_revolution/1000;
	}
}

float Turning_technology::getFeedrate()
{
	if (feedrate != feedrate) // Si le feedrate n'est pas donné, cad est un NaN
	{
		calculateFeedrate();
	}
	return feedrate;
}


