#include "Workpiece.h"
#include "traitement_fichier.h"
#include "Class_express.h"

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "add_object.h"

using namespace std;
extern map<string, string> mapEntite;

Material::Material()
{
}

Material::Material(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	standard_identifier = param[0];
	material_identifier = param[1];
	cout << "instanciation de " << param[1] << endl; // !!!SUPPR!!! //

	vector<string> vecMaterial_property = stringToVector(param[2]);
	for (string element : vecMaterial_property)
	{
		string name = getNameEntity(mapEntite[element]);
		if (name=="PROPERTY_PARAMETER")
		{
			material_property=addObjectVec<Property_parameter>(element);
		}
		else if (name == "DESCRIPTIVE_PARAMETER")
		{
			material_property.push_back(addObject<Desciptive_parameter>(element));
		}
		else if (name == "NUMERIC_PARAMETER")
		{
			material_property.push_back(addObject<Numeric_parameter>(element));
		}
	}
}

Workpiece::Workpiece()
{
	cout << "creation de Workpiece"<< endl;
}
Workpiece::Workpiece(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	its_id = param[0];
	cout << "instaciantion de " << param[0] << endl; // !!!SUPPR!!! //
	if (optional(param[1])) { its_material = addObject<Material>(param[1]); }
	if (optional(param[2])) { global_tolerance = stof(param[2]); }
	if (optional(param[3])) { its_rawpiece = addObject<Workpiece>(param[3]); }
	if (optional(param[4])) { cout << "geometry de Workpiece pas implemente" << endl;; }
	if (optional(param[5])) { cout << "bounding_geometry de Workpiece pas implemente" << endl;; }
	if (optional(param[6])) { clamping_positions = addObjectVec<Cartesian_point>(param[6]); }
}

