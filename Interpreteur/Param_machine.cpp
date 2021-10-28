#include "Param_machine.h"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;


Param_machine::Param_machine()
{
	its_home_position.resize(4);
	its_name = "PC_turn_55";
	its_type = "Lathe";
	max_allowable_acceleration_Z = 0.010; // m/s²
	max_allowable_acceleration_X = 0.010; // m/s²
	min_spindle_speed = 120; //RPM
	max_spindle_speed = 4000; //RPM
	rapid_movement_speed = 0.03333; // m/s (2000 mm/min)
	its_home_position(0) = 140;
	its_home_position(1) = 0;
	its_home_position(2) = 140;
	its_home_position(3) = 1;
	tipo = 0.01;// [s] A définir sampling time pour le controle de position
}

//Instanciation de la machine

Param_machine* myMachine = new Param_machine();
