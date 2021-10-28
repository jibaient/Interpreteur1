#ifndef SETUP_HPP
#define SETUP_HPP

#include <string>
#include <vector>

#include "Workpiece.h"
#include "Geometry.h"


class Workpiece_setup
{
public:
	Workpiece_setup();

	Workpiece_setup(std::string num);
private:
	Workpiece* its_workpiece;
	Axis2_placement_3d* its_origin;
	//Offset_vector its_offset; // OPTIONAL
	//Resticted_area_select its_restricted_area; // OPTIONAL
	//std::vector<setup_instruction> its_instructions; //[0,?]
};

class Setup
{
public:
	Setup();
	Setup(std::string num);
private:
	std::string its_id;
	Axis2_placement_3d* its_origin; // OPTIONAL
	Elementary_surface* its_secplane; // OPTIONAL
	std::vector<Workpiece_setup*> its_workpiece_setup; //[0,?]
};

#endif // SETUP_HPP