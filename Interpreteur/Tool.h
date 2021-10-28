#ifndef TOOL_HPP
#define TOOL_HPP

#include <string>
#include <vector>

#include "Workpiece.h"
#include "Geometry.h"

class Tool_body //ABS
{
public:
protected:
};

class Turning_tool_dimension
{
public:
	Turning_tool_dimension();
	Turning_tool_dimension(std::string num);
protected:
	float cutting_edge_length; // OPTIONAL
	float side_rake_angle; // OPTIONAL
	float side_relief_angle; // OPTIONAL
	float side_clearance_angle; // OPTIONAL
	float end_cutting_edge_angle; // OPTIONAL
	float side_cutting_edge_angle; // OPTIONAL
	float back_rake_angle; // OPTIONAL
	float end_relief_angle; // OPTIONAL
	float end_clearance_angle; // OPTIONAL
	float nose_radius; // OPTIONAL
	float circle_diameter; // OPTIONAL
};

class Turning_machine_tool_body //ABS
	: public Tool_body
{
public:
protected:
	Turning_tool_dimension* dimension;
	std::string hand_of_tool; // OPTIONAL //Format un peu special .left. pas vraiment un string
	float maximum_cutting_depth; // OPTIONAL
	float tool_allowance_length; // OPTIONAL
	float tool_body_height; // OPTIONAL
	float tool_body_width; // OPTIONAL
	//Rot_direction* rotational_direction; // OPTIONAL
};

class General_turning_tool //  appelé Turning_tool dans la norme mais pas les exemples????
	: public Turning_machine_tool_body
{
public:
	General_turning_tool();
	General_turning_tool(std::string num);
protected:
};

class Machining_tool //ABS
{
public:
protected:
	std::string its_id;
};

class Cutting_edge_technological_data
{
public:
protected:
	//Plane_angle_measure* cutting_angle; // OPTIONAL
	//Plane_angle_measure* free_angle; // OPTIONAL
	//Plane_angle_measure* aux_angle; // OPTIONAL
};

class Cutting_component
{
public:
	Cutting_component();
	Cutting_component(std::string num);
protected:
	float tool_offset_length;
	Material* its_material; // OPTIONAL
	Cutting_edge_technological_data* technological_data; // OPTIONAL
	float expected_tool_life; // OPTIONAL
	//Technology* its_technology;
};

class Cutting_tool //ABS
	: public Machining_tool
{
public:
protected:
	Tool_body* its_tool_body;
	std::vector<Cutting_component*> its_cutting_edge; //[1:?]
	float overall_assembly_length;
};


class Turning_machine_tool
	: public Cutting_tool
{
public:
	Turning_machine_tool();
	Turning_machine_tool(std::string num);
protected:
	float overall_assembly_width;
	float minimum_cutting_diameter;
};

#endif // TOOL_HPP



