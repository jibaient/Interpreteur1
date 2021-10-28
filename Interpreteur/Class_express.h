#ifndef CLASS_EXPRESS_HPP
#define CLASS_EXPRESS_HPP

#include <string>
#include <vector>

#include "Workpiece.h"
#include "Executable.h"
#include "Feature.h"
#include "Geometry.h"

class Person_and_address
{
public:
	Person_and_address();
	Person_and_address(std::string numHash);
	std::string get_its_person2() const;
	std::string affiche(int nPro) const;
	

public: //A remodifier !!!!!
	std::string its_person; // !!! modifier type !!!
	std::string its_address; //optional // !!! modifier type !!!
};

class Date_and_time
{

public:
	Date_and_time();
	Date_and_time(std::string date_and_time);
private:

	std::string its_date_and_time; // !!! modifier type !!!
};

#endif // CLASS_EXPRESS_HPP
