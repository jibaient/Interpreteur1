#include "Strategy.h"
#include "traitement_fichier.h"
#include "add_object.h"
#include "Geometry.h"
#include "toolpath.h"
#include "Param_machine.h"


#include <string>
#include <string>
#include <map>
#include <iostream>
#include <math.h>
#define PI 3.14159365

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;

using namespace std;

extern map<string, string> mapEntite;
extern Param_machine* myMachine;

Unidirectional_turning::Unidirectional_turning()
{
}

Unidirectional_turning::Unidirectional_turning(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	cout << "Unidirectional turning " << endl; // !!!SUPPR!!! //
	if (optional(param[0])) { overcut_lenth = stof(param[0]); }
	if (optional(param[1])) { allow_multiple_passes = stringToBool(param[1]); }
	if (optional(param[2])) 
	{ 
		vector<string> vecCutting_depth = stringToVector(param[2]);
		for (string element : vecCutting_depth)
		{
			cutting_depth.push_back(stof(element));
		}
	}
	if (optional(param[3])) { variable_feedrate = stof(param[3]); }
	if (optional(param[4])) { feed_direction=addObject<Direction>(param[4]); }
	if (optional(param[5])) { back_path_direction = addObject<Direction>(param[5]); }
	if (optional(param[6])) { lift_direction = addObject<Direction>(param[6]); }
	if (optional(param[7])) { stepover_direction = addObject<Direction>(param[7]); }
	if (optional(param[8])) { lift_height = stof(param[8]); }
	if (optional(param[9])) { lift_feed = stof(param[9]); }
	if (optional(param[10])) { stepover_feed = stof(param[10]); }
}

Direction* Unidirectional_turning::getFeed_direction()
{
	return feed_direction;
}

float Turning_machining_strategy::getLift_height()
{
	return 0.0f;
}

std::vector<float> Turning_machining_strategy::getCutting_depth()
{
	return cutting_depth;
}

float Unidirectional_turning::getLift_height()
{
	return lift_height;
}

Ap_retract_angle::Ap_retract_angle()
{
}

Ap_retract_angle::Ap_retract_angle(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	if (optional(param[0])) { cout << "tool_orient dans Ap retract angle pas encore implement" << endl; }
	angle = stof(param[1]);
	travel_length = stof(param[2]);
}

toolpath_t Ap_retract_angle::generateApproachTP(ublas::vector<float> start_point, float retract_plane_height)
{
	toolpath_t toolpath;
	return toolpath;
}

toolpath_t Ap_retract_angle::generateRetractTP(ublas::vector<float> start_point, float retract_plane_height)
{

	toolpath_t toolpath;
	
	float a = angle; // angle en degré
	float l = travel_length;
	ublas::vector<float> retract_point(4);
	retract_point[0] = start_point[0] + l * cos(a * PI/180.0);
	retract_point[1] = start_point[1];
	retract_point[2] = start_point[2] + l * sin(a * PI/180.0);
	retract_point[3] = 1;
	toolpath.push_back(new Toolpath_section(start_point,retract_point, "linear", "move"));

	return toolpath;
}

Ap_retract_tangent::Ap_retract_tangent()
{
}
Ap_retract_tangent::Ap_retract_tangent(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	if (optional(param[0])) { cout << "tool_orient dans Ap retract angle pas encore implement" << endl; }
	radius = stof(param[1]);
}

toolpath_t Ap_retract_tangent::generateApproachTP(ublas::vector<float> start_point, float retract_plane_height)
{
	// coordonnée dans le repère de la feature défini à l'endroit où est utilisé la fonction
	toolpath_t toolpath;
	float r = radius;
	ublas::vector<float> retract_plane_point(4);
	retract_plane_point(0) = start_point(0) + r ;
	retract_plane_point(1) = start_point(1);
	retract_plane_point(2) = start_point(2) + r ;
	retract_plane_point(3) = 1;


	ublas::vector<float> center(4);
	center(0) = start_point(0)+r;
	center(1) = start_point(1);
	center(2) = start_point(2);
	center(3) = 1;

	toolpath.push_back(new Toolpath_section(myMachine->its_home_position, retract_plane_point, "linear", "rapid_move"));


	toolpath.push_back(new Toolpath_section(retract_plane_point, start_point, "circular","move", r,center));

	std::cout << " retract point : " << retract_plane_point << " start_point : " << start_point << " center : " << center << std::endl;
	return toolpath;
}

toolpath_t Ap_retract_tangent::generateRetractTP(ublas::vector<float> start_point, float retract_plane_height)
{
	return toolpath_t();
}

Direction* Turning_machining_strategy::getFeed_direction()
{
	return nullptr;
}

toolpath_t Approach_retract_strategy::generateApproachTP(ublas::vector<float> start_point, float retract_plane_height)
{
	return toolpath_t();
}

toolpath_t Approach_retract_strategy::generateRetractTP(ublas::vector<float> start_point, float retract_plane_height)
{
	return toolpath_t();
}

toolpath_t Air_strategy::generateApproachTP(ublas::vector<float> start_point, float retract_plane_height)
{
	return toolpath_t();
}

toolpath_t Air_strategy::generateRetractTP(ublas::vector<float> start_point, float retract_plane_height)
{
	return toolpath_t();
}
