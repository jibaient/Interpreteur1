#include "Project.h"
#include "Class_express.h"
#include "Executable.h"
#include "traitement_fichier.h"
#include "add_object.h"

#include <string>
#include <map>
#include <iostream>

using namespace std;

extern map<string, string> mapEntite;
extern map<string, string> mapName;

Project::Project()
{
	cout << "instanciation de projet" << endl;
	vector<string> param = getParam(mapName["PROJECT"]);
	its_id = param[0];
	cout << "creation de " << its_id << endl;
	its_main_workplan = addObject<Workplan>(param[1]);
	its_workpieces = addObjectVec<Workpiece>(param[2]);
	if (optional(param[3])) {its_owner = addObject<Person_and_address>(param[3]);}
	if (optional(param[4])) {its_release = addObject<Date_and_time>(param[4]);}
	if (optional(param[5])) {its_status = param[5];} // !!! modifier type !!!
}


string Project::get_its_id() const
{
	return its_id;
}
string Project::get_its_person() const
{
	return its_owner->get_its_person2();
}