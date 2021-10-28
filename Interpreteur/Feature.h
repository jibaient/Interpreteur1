#ifndef FEATURE_HPP
#define FEATURE_HPP

#include <string>
#include <vector>

#include "Workpiece.h"
#include "Geometry.h"
#include "Operation.h"
#include "toolpath.h"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;

class Manufacturing_feature
{
public:
	virtual toolpath_t generateFeatureTP(Operation* its_operation, Elementary_surface* its_secplane);
protected:
	std::string its_id;
	Workpiece* its_workpiece;
	std::vector<Machining_operation*> its_operations; // [0,?]
};

class Two5D_manufacturing_feature
	:public Manufacturing_feature
{
public:
	virtual toolpath_t generateFeatureTP(Operation* its_operation, Elementary_surface* its_secplane);
protected:
	Axis2_placement_3d* feature_placement;
};

class Turning_feature : // ABS
	public Two5D_manufacturing_feature
{
public:
	virtual toolpath_t generateFeatureTP(Operation* its_operation, Elementary_surface* its_secplane);
protected:
};

class Revolved_feature :
	public Turning_feature
{
public:
	virtual toolpath_t generateFeatureTP(Operation* its_operation, Elementary_surface* its_secplane);
protected:
	Direction* material_side; // OPTIONAL
	float radius; // attention est considéré comme une entité dans la norme mais dans l'exemple est un float
};

class Revolved_flat :
	public Revolved_feature
{
public:
	Revolved_flat();
	Revolved_flat(std::string num);
	virtual toolpath_t generateFeatureTP(Operation* its_operation, Elementary_surface* its_secplane);
protected:
	Linear_profile* flat_edge_shape;
};

#endif // FEATURE_HPP