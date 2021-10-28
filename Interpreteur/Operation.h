#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <string>
#include <vector>

#include "Geometry.h"
#include "Tool.h"
#include "Technology.h"
#include "Strategy.h"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;

class Operation //ABS
{
public:
	virtual Turning_machining_strategy* getMachining_Strategy() const;
	virtual float getAllowance();
	virtual Approach_retract_strategy* getApproach()const;
	virtual Approach_retract_strategy* getRetract()const;
	virtual float getRetract_plane() const;
	virtual float getFeedrate() const;
protected:
	//Toolpath_list its_toolpath;
	//Tool_direction its_tool_direction;
};

class Machining_operation //ABS
	: public Operation
{
public:
	virtual Turning_machining_strategy* getMachining_Strategy() const;
	virtual float getAllowance();
	virtual Approach_retract_strategy* getApproach()const;
	virtual Approach_retract_strategy* getRetract()const;
	virtual float getRetract_plane()const;
	virtual float getFeedrate() const;

protected:
	std::string its_id;
	float retract_plane; // OPTIONAL
	Cartesian_point* start_point; //OPTIONAL
	Machining_tool* its_tool;
	Technology* its_technology;
	Machine_functions* its_machine_function;
};

class Turning_machining_operation
	: public Machining_operation
{
public:
	virtual Turning_machining_strategy* getMachining_Strategy() const;
	virtual float getAllowance();
	virtual Approach_retract_strategy* getApproach()const;
	virtual Approach_retract_strategy* getRetract()const;
protected:
	Approach_retract_strategy* approach;
	Approach_retract_strategy* retract;
	Turning_machining_strategy* its_machining_strategy;
};

class Facing //ABS
	: public Turning_machining_operation
{
public:
	virtual float getAllowance();
protected:
	float allowance;
};

class Facing_rough
	: public Facing
{
public:
	Facing_rough();
	Facing_rough(std::string num);
protected:
};

class Facing_finish
	: public Facing
{
public:
	Facing_finish();
	Facing_finish(std::string num);
protected:
};

#endif // OPERATION_HPP