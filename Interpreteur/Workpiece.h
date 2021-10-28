
#ifndef WORKPIECE_HPP
#define WORKPIECE_HPP

#include <string>
#include <vector>

#include "Geometry.h"


class Material
{
public:
	Material();
	Material(std::string num);
protected:
	std::string standard_identifier;
	std::string material_identifier;
	std::vector<Property_parameter*> material_property;
};

class Workpiece
{
public:
	Workpiece();
	Workpiece(std::string num); 
protected:
	std::string its_id;
	Material* its_material; // OPTIONAL
	float global_tolerance; // OPTIONAL
	Workpiece* its_rawpiece; // OPTIONAL (+problème d'appel récurent)
	//Advanced_brep_shape_representation its_geometry; // OPTIONAL
	//Bounding_geometry_select its_bounding_geometry; // OPTIONAL
	std::vector<Cartesian_point*> clamping_positions; // OPTIONAL [0,?]
};

#endif // WORKPIECE_HPP