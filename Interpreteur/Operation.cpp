#include "Operation.h"
#include "traitement_fichier.h"
#include "Tool.h"
#include "add_object.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;

using namespace std;

extern map<string, string> mapEntite;

Turning_machining_strategy* Operation::getMachining_Strategy() const
{
	return nullptr;
}
float Operation::getAllowance()
{
	return 0.0f;
}
Approach_retract_strategy* Operation::getApproach() const
{
	return nullptr;
}
Approach_retract_strategy* Operation::getRetract() const
{
	return nullptr;
}
float Operation::getRetract_plane() const
{
	return 0.0f;
}
float Operation::getFeedrate() const
{
	return 0.0f;
}
Turning_machining_strategy* Machining_operation::getMachining_Strategy() const
{
	return nullptr;
}
float Machining_operation::getAllowance()
{
	return 0.0f;
}
Approach_retract_strategy* Machining_operation::getApproach() const
{
	return nullptr;
}
Approach_retract_strategy* Machining_operation::getRetract() const
{
	return nullptr;
}
float Machining_operation::getRetract_plane() const
{
	return retract_plane;
}
float Machining_operation::getFeedrate() const
{
	return its_technology->getFeedrate();
}
Turning_machining_strategy* Turning_machining_operation::getMachining_Strategy() const
{
	return its_machining_strategy;
}
float Turning_machining_operation::getAllowance()
{
	return 0.0f;
}
Approach_retract_strategy* Turning_machining_operation::getApproach() const
{
	return approach;
}
Approach_retract_strategy* Turning_machining_operation::getRetract() const
{
	return retract;
}
Facing_rough::Facing_rough()
{
}

Facing_rough::Facing_rough(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	if (optional(param[0])) { cout << "tool_direction de Facing_rough implemente" << endl; }
	if (optional(param[1])) { cout << "toolpath de Facing_rough implemente" << endl; }
	its_id = param[2];
	cout << "instanciation de Facing rough " << param[2] << endl;
	retract_plane = NAN; //initialisation de la hauteur de retract_plane
	if (optional(param[3])) { retract_plane = stof(param[3]); }
	if (optional(param[4])) { cout << "Start_point de Facing_rough pas implemente" << endl; }
	string nameTool = getNameEntity(mapEntite[param[5]]);
	if (nameTool == "TURNING_MACHINE_TOOL")
	{
		its_tool=addObject<Turning_machine_tool>(param[5]);
	}
	else
	{
		cout << nameTool << " de tool pas implemente" << endl;
	}
	string nameTechnology = getNameEntity(mapEntite[param[6]]);
	if (nameTechnology == "TURNING_TECHNOLOGY")
	{
		its_technology=addObject<Turning_technology>(param[6]);
	}
	else
	{
		cout << nameTechnology << "  de Turning technology pas implemente";
	}
	string nameFunction = getNameEntity(mapEntite[param[7]]);
	if (nameFunction == "TURNING_MACHINE_FUNCTIONS")
	{
		its_machine_function=addObject<Turning_machine_functions>(param[7]);
	}
	else
	{
		cout << nameFunction << "  de Turning machine functions pas encore implemente";
	}
	string nameApproach = getNameEntity(mapEntite[param[8]]);
	if (nameApproach == "AP_RETRACT_TANGENT")
	{
		approach = addObject<Ap_retract_tangent>(param[8]);
	}
	if (nameApproach == "AP_RETRACT_ANGLE")
	{
		approach = addObject<Ap_retract_angle>(param[8]);
	}
	string nameRetract = getNameEntity(mapEntite[param[9]]);
	if (nameRetract == "AP_RETRACT_TANGENT")
	{
		retract = addObject<Ap_retract_tangent>(param[9]);
	}
	if (nameRetract == "AP_RETRACT_ANGLE")
	{
		retract = addObject<Ap_retract_angle>(param[9]);
	}
	string nameStrategy = getNameEntity(mapEntite[param[10]]);
	if (nameStrategy == "UNIDIRECTIONAL_TURNING")
	{
		its_machining_strategy =addObject<Unidirectional_turning>(param[10]);
	}
	else
	{
		cout << nameStrategy << "  de Strategy pas implemente";
	}

	if (optional(param[11])) { allowance = stof(param[11]); }
}


Facing_finish::Facing_finish()
{
}

Facing_finish::Facing_finish(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);

	if (optional(param[0])) { cout << "tool_direction de Facing_finish implemente" << endl; }
	if (optional(param[1])) { cout << "toolpath de Facing_finish implemente" << endl; }

	its_id = param[2];
	cout << "instanciation de Facing rough " << param[2] << endl;

	if (optional(param[3])) { cout << "Retract_plane de Facing_finish pas implemente" << endl; }
	if (optional(param[4])) { cout << "Start_point de Facing_finish pas implemente" << endl; }


	string nameTool = getNameEntity(mapEntite[param[5]]);
	if (nameTool == "TURNING_MACHINE_TOOL")
	{
		its_tool = addObject<Turning_machine_tool>(param[5]);
	}
	else
	{
		cout << nameTool << " de tool pas implemente" << endl;
	}


	string nameTechnology = getNameEntity(mapEntite[param[6]]);
	if (nameTechnology == "TURNING_TECHNOLOGY")
	{
		its_technology = addObject<Turning_technology>(param[6]);
	}
	else
	{
		cout << nameTechnology << "  de Turning technology pas encore implemente";
	}

	string nameFunction = getNameEntity(mapEntite[param[7]]);

	if (nameFunction == "TURNING_MACHINE_FUNCTIONS")
	{
		its_machine_function = addObject<Turning_machine_functions>(param[7]);
	}
	else
	{
		cout << nameFunction << "  de Turning machine functions pas encore implemente";
	}

	string nameApproach = getNameEntity(mapEntite[param[8]]);
	if (nameApproach == "AP_RETRACT_TANGENTS")
	{
		approach = addObject<Ap_retract_tangent>(param[8]);
	}
	if (nameApproach == "AP_RETRACT_ANGLE")
	{
		approach = addObject<Ap_retract_angle>(param[8]);
	}

	string nameRetract = getNameEntity(mapEntite[param[9]]);
	if (nameRetract == "AP_RETRACT_TANGENTS")
	{
		retract = addObject<Ap_retract_tangent>(param[9]);
	}
	if (nameRetract == "AP_RETRACT_ANGLE")
	{
		retract = addObject<Ap_retract_angle>(param[9]);
	}

	string nameStrategy = getNameEntity(mapEntite[param[10]]);
	if (nameStrategy == "UNIDIRECTIONAL_TURNING")
	{
		its_machining_strategy = addObject<Unidirectional_turning>(param[10]);
	}
	else
	{
		cout << nameStrategy << "  de Strategy pas implemente";
	}

	if (optional(param[11])) { allowance = stof(param[11]); }
}

float Facing::getAllowance()
{
	return allowance;
}
