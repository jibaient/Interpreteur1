#include "Setup.h"
#include "traitement_fichier.h"
#include "add_object.h"

#include <map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;
extern map<string, string> mapEntite;

Workpiece_setup::Workpiece_setup()
{
}

Workpiece_setup::Workpiece_setup(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	cout << "instanciation de Workpiece setup" << endl;
	its_workpiece = addObject<Workpiece>(param[0]);
	its_origin = addObject<Axis2_placement_3d>(param[1]);
	if (optional(param[2])) { cout << "offset de workpiece setup pas implement" << endl;; }
	if (optional(param[3])) { cout << "restricted_area de workpiece setup pas implement" << endl; }
	if (optional(param[4])) { cout << "instructions de workpiece setup pas implement" << endl; }
}

Setup::Setup()
{
}
Setup::Setup(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	cout << "instanciation de Setup" << param[0] << endl;
	its_id = param[0];
	if (optional(param[1])) { cout << "origin de Setup pas implemente" << endl;; }
	if (optional(param[2])) 
	{
		string name = getNameEntity(mapEntite[param[2]]);
		if (name == "PLANE")
		{
			its_secplane = addObject<Plane>(param[2]);
		}
		else
		{
			cout << name << " de elementary_surface de Setup pas implemente" << endl;
		}
	}

	if (optional(param[3])) { its_workpiece_setup = addObjectVec<Workpiece_setup>(param[3]); }
}
