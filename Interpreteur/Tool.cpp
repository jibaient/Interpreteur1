#include "Operation.h"
#include "traitement_fichier.h"
#include "Tool.h"
#include "Workpiece.h"
#include "Geometry.h"
#include "add_object.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

extern map<string, string> mapEntite;
Turning_tool_dimension::Turning_tool_dimension()
{
}
Turning_tool_dimension::Turning_tool_dimension(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	cout << "instanciation de Turning tool dimension " <<endl;

	if (optional(param[0])) { cutting_edge_length = stof(param[0]); }
	if (optional(param[1])) { side_rake_angle = stof(param[1]); }
	if (optional(param[2])) { side_relief_angle = stof(param[2]); }
	if (optional(param[3])) { side_clearance_angle = stof(param[3]); }
	if (optional(param[4])) { end_cutting_edge_angle = stof(param[4]); }
	if (optional(param[5])) { side_cutting_edge_angle = stof(param[5]); }
	if (optional(param[6])) { back_rake_angle = stof(param[6]); }
	if (optional(param[7])) { end_relief_angle = stof(param[7]); }
	if (optional(param[8])) { end_clearance_angle = stof(param[8]); }
	if (optional(param[9])) { nose_radius = stof(param[9]); }
	if (optional(param[10])) { circle_diameter = stof(param[10]); }
}
General_turning_tool::General_turning_tool()
{
}

General_turning_tool::General_turning_tool(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	cout << "instanciation de general turning tool " << endl;
	dimension = addObject<Turning_tool_dimension>(param[0]);

	if (optional(param[1])) { hand_of_tool = param[1]; }
	if (optional(param[2])) { maximum_cutting_depth = stof(param[2]); }
	if (optional(param[3])) { tool_allowance_length = stof(param[3]); }
	if (optional(param[4])) { tool_body_height = stof(param[4]); }
	if (optional(param[5])) { tool_body_width = stof(param[5]); }
	if (optional(param[6])) { cout << "rotational_direction de General_turning_tool pas implemente" << endl; }

}

Cutting_component::Cutting_component()
{
}
Cutting_component::Cutting_component(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	tool_offset_length = stof(param[0]);
	if (optional(param[1])) { its_material = addObject<Material>(param[1]); }
	//if (optional(param[2])) { technological_data = addObject<Cutting_edge_technological_data>(param[2]); }
	if (optional(param[3])) { expected_tool_life = stof(param[3]); }
	//if (optional(param[4])) { its_technology = addObject<Technology>(param[4]); }
}

Turning_machine_tool::Turning_machine_tool()
{
}
Turning_machine_tool::Turning_machine_tool(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	its_id = param[0];
	cout << "instanciation de turning machine tool : " << param[0] << endl;


	string nameTool_body = getNameEntity(mapEntite[param[1]]);
	if (nameTool_body == "GENERAL_TURNING_TOOL")
	{
		its_tool_body=addObject<General_turning_tool>(param[1]);
	}
	else
	{
		cout << nameTool_body << " pas implemente dans Tool_body" << endl;
	}

	if (optional(param[2])) 
	{ 

		its_cutting_edge = addObjectVec<Cutting_component>(param[2]);
	}

	if (optional(param[3])) { overall_assembly_length = stof(param[3]); }
	if (optional(param[4])) { overall_assembly_width = stof(param[4]); }
	if (optional(param[5])) { minimum_cutting_diameter = stof(param[5]); }

}