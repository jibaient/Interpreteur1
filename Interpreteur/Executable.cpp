#include "Executable.h"
#include "Class_express.h"
#include "traitement_fichier.h"
#include "Geometry.h"
#include "add_object.h"
#include "Setup.h"
#include "Executable.h"
#include "interpolate.h"
#include "toolpath.h"
#include "Param_machine.h"
#include "look_ahead.h"
#include "accDec_Controller.h"


#include <string>
#include <map>
#include <iostream>
#include <math.h>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;

using namespace std;
extern map<string, string> mapEntite;
extern Param_machine* myMachine;

Executable::Executable()
{
}

Machining_workingstep::Machining_workingstep()
{
}
Machining_workingstep::Machining_workingstep(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	cout << "instanciation de Machining workinstep " << param[0] << endl;
	its_id = param[0];

	string nameSecplane = getNameEntity(mapEntite[param[1]]);
	if (nameSecplane == "PLANE")
	{
		its_secplane = addObject<Plane>(param[1]);
	}
	else
	{
		cout << nameSecplane << "  pas implemente dans elementary_surface" << endl;
	}

	string nameFeature = getNameEntity(mapEntite[param[2]]);
	if (nameFeature == "REVOLVED_FLAT")
	{
		its_feature = addObject<Revolved_flat>(param[2]);
	}
	else
	{
		cout << nameFeature << "  pas implemente dans feature" << endl;
	}

	string nameOperation = getNameEntity(mapEntite[param[3]]);
	if (nameOperation == "FACING_ROUGH")
	{
		its_operation = addObject<Facing_rough>(param[3]);
	}
	else if (nameOperation == "FACING_FINISH")
	{
		its_operation = addObject<Facing_finish>(param[3]);
	}
	else
	{
		cout << nameOperation << " pas implemente dans Operation" << endl;
	}

	//if (optional(param[4])) { its_effect = addObject<In_process_geometry>(param[4]); }
}

void Executable::generateTP()
{
}
void Executable::coutMachiningTP()
{
}
void Executable::plotPyTP()
{
}
void Executable::plotPyTPInterpolation()
{
}
void Executable::roughInterpolation()
{
}
toolpath_t Executable::getTP()
{
	return toolpath_t();
}
void Executable::lookAhead()
{
}
void Executable::accDec()
{
}
void Workingstep::generateTP()
{
}

void Workingstep::coutMachiningTP()
{
}

void Workingstep::plotPyTP()
{
}

void Workingstep::plotPyTPInterpolation()
{
}

void Workingstep::roughInterpolation()
{
}

toolpath_t Workingstep::getTP()
{
	return toolpath_t();
}

void Workingstep::lookAhead()
{

}

void Workingstep::accDec()
{
}

void Machining_workingstep::generateTP()
{
	generated_machining_toolpath = its_feature->generateFeatureTP(its_operation, its_secplane);
}

void Machining_workingstep::coutMachiningTP()
{
	cout << "\nParcours d'outils de : "<< its_id << endl;
	coutTP(generated_machining_toolpath);
}

void Machining_workingstep::plotPyTP()
{
	plotPy(generated_machining_toolpath);
}

void Machining_workingstep::plotPyTPInterpolation()
{
	plotPyInterpolation(generated_machining_toolpath);
}

void Machining_workingstep::roughInterpolation()
{
	for (auto element : generated_machining_toolpath)
	{
		if (element->its_interpolation_type == "linear")
		{
			linearIPO_Pre(element);
		}
		else if (element->its_interpolation_type == "circular")
		{
			circleIPO_Pre(element);
		}

		mapping(element);
	}


}

toolpath_t Machining_workingstep::getTP()
{
	return generated_machining_toolpath;
}

void Machining_workingstep::lookAhead()
{
	const int nbrSectionLookAhead = 5 ;

	float feedrate = its_operation->getFeedrate();
	float machine_max_speed = myMachine->rapid_movement_speed;
	float start_speed_current = 0;
	
	for (int i = 0; i < generated_machining_toolpath.size()-1; i++)
	{
		if (i+nbrSectionLookAhead<generated_machining_toolpath.size())
		{
			toolpath_t vectorLookAheadSections = toolpath_t(generated_machining_toolpath.begin() + i, generated_machining_toolpath.begin() + i + nbrSectionLookAhead +1); // +1 car derniere borne est exclue
			lookAheadFunction(vectorLookAheadSections, feedrate, machine_max_speed, start_speed_current);
			start_speed_current = generated_machining_toolpath[i]->its_VEnd;
		}
		else
		{
			toolpath_t vectorLookAheadSections = toolpath_t(generated_machining_toolpath.begin() + i, generated_machining_toolpath.end());
			lookAheadFunction(vectorLookAheadSections, feedrate, machine_max_speed, start_speed_current);
			start_speed_current = generated_machining_toolpath[i]->its_VEnd;
		}
	}
}

void Machining_workingstep::accDec()
{
	for (auto element : generated_machining_toolpath)
	{
		float feedrate = its_operation->getFeedrate();
		if (element->its_movement_type == "rapid_move") // Si c'est un mouvement rapide le feedrate est la vitesse de déplacement rapide de la machine
		{
			feedrate = myMachine->rapid_movement_speed;
		}
		determineVelocityProfile(element, feedrate);
	}
}

Workplan::Workplan()
{
}

Workplan::Workplan(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	cout << "instanciation de " << param[0] << endl;
	its_id = param[0];
	for (auto element : stringToVector(param[1]))
	{
		std::string nameOp = getNameEntity(mapEntite[element]);
		if (nameOp == "MACHINING_WORKINGSTEP")
		{
			its_elements.push_back(addObject<Machining_workingstep>(element));
		}
		else if (nameOp == "WORKPLAN")
		{
			its_elements.push_back(addObject<Workplan>(element));
		}
		else
		{
			std::cout << nameOp << " pas implemente" << std::endl;
		}
	}
	//if (optional(param[2])) { its_channel = addObject<Channel>(param[2]);}
	if (optional(param[3])) { its_setup = addObject<Setup>(param[3]);}
	//if (optional(param[4])) { its_effect = addObject<In_process_geometry>(param[4]);}
}

