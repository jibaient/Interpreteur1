#ifndef TOOLPATH_HPP
#define TOOLPATH_HPP

#include <string>
#include <vector>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace ublas = boost::numeric::ublas;


class Toolpath_section
{
public:
	Toolpath_section();
	Toolpath_section(ublas::vector<float> start_point,ublas::vector<float> end_point, std::string interpolation_type, std::string movement_type);
	Toolpath_section(ublas::vector<float> start_point,ublas::vector<float> end_point, std::string interpolation_type, std::string movement_type, float parameter, ublas::vector<float> center);
	void coutTPS();

	ublas::vector<float> its_start_point;
	ublas::vector<float> its_end_point;
	std::string its_interpolation_type; // circular, linear
	std::string its_movement_type; // approach, retract, machining, rapid_move, move
	float its_parameter; //Le rayon pour les interpolations circulaire
	ublas::vector<float> its_center; // Le centre du cercle pour les interpolations circulaire

	float its_VStart; //Vitesse au début de la section (mm/s)
	float its_VEnd; //Vitesse à la fin de la section (mm/s)

	std::vector<float> its_speed_profile;
	std::vector< ublas::vector<float>> its_rough_interpolation;
	std::vector<float> z_displacement; // déplacement relatif pour l'axe z [mm]
	std::vector<float> x_displacement;// déplacement relatif pour l'axe x [mm]

protected:
};

using toolpath_t = std::vector<Toolpath_section*>;

void coutTP(toolpath_t toolpath);
void plotPy(toolpath_t toolpath);
void plotPyInterpolation(toolpath_t toolpath);


#endif // TOOLPATH_HPP
