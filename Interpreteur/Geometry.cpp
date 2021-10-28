#include "Geometry.h"
#include "traitement_fichier.h"
#include "Parameter.h"
#include <string>
#include <vector>
#include <map>
#include "add_object.h"

#include <iostream>

using namespace std;
extern map<string, string> mapEntite;

Cartesian_point::Cartesian_point()
{
}
Cartesian_point::Cartesian_point(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	name = param[0];
	cout << "instanciation de Cartesian point " << param[0] << endl;
	vector<string> vecCoord = stringToVector(param[1]);
	for (string element : vecCoord)
	{
		coordinates.push_back(stof(element));
	}

}

std::vector<float> Cartesian_point::getCoordinates()
{
	return coordinates;
}

Placement::Placement()
{
}

std::vector<float> Placement::getLocation()
{
	return location.getCoordinates();
}

Direction::Direction()
{
}

Direction::Direction(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	name = param[0];
	cout << "instanciation de direction " << param[0] << endl;
	vector<string> vecDir_ratios = stringToVector(param[1]);
	for (string element : vecDir_ratios)
	{
		direction_ratios.push_back(stof(element));
	}
}
std::vector<float> Direction::getDirection_ratios()
{
	return direction_ratios;
}
Axis2_placement_3d::Axis2_placement_3d()
{
}
Axis2_placement_3d::Axis2_placement_3d(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	name = param[0];
	cout << "instanciation Axis2_placement_3d " << param[0] << endl;
	location = Cartesian_point(param[1]);
	if (optional(param[2])) { axis = addObject<Direction>(param[2]);}
	if (optional(param[3])) { ref_direction = addObject<Direction>(param[3]);}
}
std::vector<float> Axis2_placement_3d::getZAxis()
{
	return axis->getDirection_ratios();
}
std::vector<float> Axis2_placement_3d::getXAxis()
{
	return ref_direction->getDirection_ratios();
}
Direction* Axis2_placement_3d::getZDirection()
{
	return axis;
}
Direction* Axis2_placement_3d::getXDirection()
{
	return ref_direction;
}
Elementary_surface::Elementary_surface()
{
}

Elementary_surface::Elementary_surface(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	name = param[0];
	cout << "instanciation Elementary_surface " << param[0] << endl;
	position = addObject<Axis2_placement_3d>(param[1]);
}
std::vector<float> Elementary_surface::getLocation()
{
	return position->getLocation();
}
Plane::Plane()
{
}
Plane::Plane(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	name = param[0];
	cout << "instanciation Elementary_surface " << param[0] << endl;
	position = addObject<Axis2_placement_3d>(param[1]);
}
std::vector<float> Plane::getLocation()
{
	return position->getLocation(); // retourne les coordonnées de l'origine du repère du plan
}
Linear_profile::Linear_profile()
{
}

Linear_profile::Linear_profile(std::string num)
{
	cout << "instanciation Elementary_surface " << endl;
	vector<string> param = getParam(mapEntite[num]);
	if (optional(param[0])) { placement = addObject<Axis2_placement_3d>(param[0]);}
	profile_length = addObject<Numeric_parameter>(param[1]);
}

float Linear_profile::getProfile_length()
{
	return profile_length->getParameter_value();
}

Axis2_placement_3d* Profil::getPlacement()
{
	return placement;
}
