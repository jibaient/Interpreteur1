#ifndef PROJECT_HPP
#define PROJECT_HPP


#include "Class_express.h"
#include "Executable.h"
#include "traitement_fichier.h"
#include <string>


class Project
{
public:
	Project();
	std::string get_its_id() const;
	std::string get_its_person() const;

public: // ATTENTION : Remetre en protected

	std::string its_id;
	Workplan* its_main_workplan; 
	std::vector<Workpiece*> its_workpieces; // [0:?]
	Person_and_address* its_owner; //optional
	Date_and_time* its_release; //optional //
	std::string its_status; //optional // !!! modifier type !!!
};


#endif // PROJECT_HPP