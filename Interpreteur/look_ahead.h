#ifndef LOOK_AHEAD_HPP
#define LOOK_AHEAD_HPP
#include "toolpath.h"
#include <string.h>


#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace ublas = boost::numeric::ublas;

void lookAheadFunction(toolpath_t VSections,float feedrate, float machine_max_speed,float start_speed_current);
double determineVelocityBetweenLL(Toolpath_section* N1, Toolpath_section* N2,float feedrate,float machine_max_speed);
double determineVelocityBetweenLC(Toolpath_section* N1, Toolpath_section* N2, float feedrate, float machine_max_speed);
double determineVelocityBetweenCL(Toolpath_section* N1, Toolpath_section* N2, float feedrate, float machine_max_speed);
double determineVelocityBetweenCC();

double determineSpeedAxis(double feedrate, double startPointComponent, double endPointComponent, double blockLenght);
double determineBlockLength(ublas::vector<float> point1, ublas::vector<float> point2);
double determineAngle(ublas::vector<float> point1, ublas::vector<float> point2, ublas::vector<float> point3);

double determineFeedrateMaxSpeed(std::string movement_type, float feedrate, float machine_max_speed);

#endif // LOOK_AHEAD_HPP