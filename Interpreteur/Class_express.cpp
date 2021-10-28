#include "Class_express.h"
#include "Executable.h"
#include "traitement_fichier.h"
#include <string>
#include <map>
#include <iostream>
#include "add_object.h"

using namespace std;

extern map<string, string> mapEntite;

/*Person_and_address*/
Person_and_address::Person_and_address()
{
}

Person_and_address::Person_and_address(std::string numHash)
{
	cout << "instanciation de Person and address " << numHash <<endl;
	vector<string> paramPerson = getParam(mapEntite[numHash]);
	its_person = paramPerson[0]; // ATTENTION TYPE PROVISOIR POUR TEST
	if (optional(paramPerson[1])) { its_address = paramPerson[1]; }; // ATTENTION TYPE PROVISOIR POUR TEST
}

string Person_and_address::get_its_person2() const
{
	std::string laPersonneRenvoye = its_person;
	return laPersonneRenvoye;
}


/*Date_and_time*/

Date_and_time::Date_and_time()
{
}

Date_and_time::Date_and_time(std::string date_and_time) : its_date_and_time(date_and_time)
{
}

