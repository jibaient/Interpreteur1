
#ifndef STRATEGY_HPP_HPP
#define STRATEGY_HPP_HPP

#include <string>
#include <vector>

#include "Geometry.h"
#include "toolpath.h"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;


class Turning_machining_strategy //abs
{
public:
	virtual Direction* getFeed_direction();
	virtual float getLift_height();
	std::vector<float> getCutting_depth();
protected:
	float overcut_lenth; // OPTIONAL
	bool allow_multiple_passes; // OPTIONAL
	std::vector<float> cutting_depth; // OPTIONAL [0:?]
	float variable_feedrate; // OPTIONAL
};

class Unidirectional_turning 
	: public Turning_machining_strategy
{
public:
	Unidirectional_turning();
	Unidirectional_turning(std::string num);
	virtual Direction* getFeed_direction();
	virtual float getLift_height();
protected:
	Direction* feed_direction; // OPTIONAL
	Direction* back_path_direction; // OPTIONAL
	Direction* lift_direction; // OPTIONAL
	Direction* stepover_direction;// OPTIONAL
	float lift_height; // OPTIONAL
	float lift_feed; // OPTIONAL
	float stepover_feed; // OPTIONAL
};

class Approach_retract_strategy
{
public:
	virtual toolpath_t generateApproachTP(ublas::vector<float> start_point, float retract_plane_height);
	virtual toolpath_t generateRetractTP(ublas::vector<float> start_point, float retract_plane_height);
protected:
	Direction* tool_orientation; // OPTIONAL
};

class Air_strategy
	: public Approach_retract_strategy
{
public:
	virtual toolpath_t generateApproachTP(ublas::vector<float> start_point, float retract_plane_height);
	virtual toolpath_t generateRetractTP(ublas::vector<float> start_point, float retract_plane_height);
protected:
};
	
class Ap_retract_angle
	: public Air_strategy
{
public:
	Ap_retract_angle();
	Ap_retract_angle(std::string num);
	virtual toolpath_t generateApproachTP(ublas::vector<float> start_point, float retract_plane_height);
	virtual toolpath_t generateRetractTP(ublas::vector<float> start_point, float retract_plane_height);
protected:
	float angle;
	float travel_length;
};

class Ap_retract_tangent
	: public Air_strategy
{
public:
	Ap_retract_tangent();
	Ap_retract_tangent(std::string num);
	virtual toolpath_t generateApproachTP(ublas::vector<float> start_point, float retract_plane_height);
	virtual toolpath_t generateRetractTP(ublas::vector<float> start_point, float retract_plane_height);
protected:
	float radius;
};

#endif // STRATEGY_HPP
