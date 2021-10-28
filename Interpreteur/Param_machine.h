#ifndef PARAM_MACHINE_HPP
#define PARAM_MACHINE_HPP

#include <string>
#include <vector>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;

class Param_machine
{
public:
	Param_machine();

public:
	std::string its_name;
	std::string its_type; //Lathe, Mill
	float rapid_movement_speed; // [RPM] Vitesse du mouvement de déplacement rapide [m/s]
	float min_spindle_speed; // [rpm]
	float max_spindle_speed; // [rpm]
	float max_allowable_acceleration_Z; // en [m/s²]
	float max_allowable_acceleration_X; // en [m/s²]
	ublas::vector<float> its_home_position;
	float tipo;

};

#endif // PARAM_MACHINE