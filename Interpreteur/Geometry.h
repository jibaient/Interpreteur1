#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <string>
#include <vector>
#include "Parameter.h"

class Representation_item
{
public:
protected:
};

class Geometric_representation_item
	: public Representation_item
{
public:
protected:
};

class Surface
	: public Geometric_representation_item
{
public:
protected:
	std::string name;
};

class Point
	: public Geometric_representation_item
{
public:
protected:
	std::string name;
};

class Cartesian_point
	: public Point
{
public:
	Cartesian_point();
	Cartesian_point(std::string num);
	std::vector<float> getCoordinates();
protected:
	std::vector<float> coordinates; //[1:3]
};

class Placement
	: public Geometric_representation_item
{
public:
	Placement();
	std::vector<float> getLocation();
protected:
	std::string name;
	Cartesian_point location;
};

class Direction
	: public Geometric_representation_item
{
public:
	Direction();
	Direction(std::string num);
	virtual std::vector<float> getDirection_ratios();
protected:
	std::string name;
	std::vector<float> direction_ratios; //[2:3]
};

class Axis2_placement_3d
	: public Placement
{
public:
	Axis2_placement_3d();
	Axis2_placement_3d(std::string num);
	std::vector<float> getZAxis();
	std::vector<float> getXAxis();
	Direction* getZDirection(); // permet de vérifier si la direction a été donnée
	Direction* getXDirection(); // permet de vérifier si la direction a été donnée
protected:
	Direction* axis; // OPTIONAL
	Direction* ref_direction; // OPTIONAL
};

class Elementary_surface
	: public Surface
{
public:
	Elementary_surface();
	Elementary_surface(std::string num);
	std::vector<float> getLocation();
protected:
	Axis2_placement_3d* position;
};

class Plane
	: public Elementary_surface
{
public:
	Plane();
	Plane(std::string num);
	std::vector<float> getLocation();
protected:
};

class Profil //ABS
{
public:
	Axis2_placement_3d* getPlacement();
protected:
	Axis2_placement_3d* placement; // OPTIONAL
};

class Open_profil //ABS
	: public Profil
{
public:
protected:
};

class Linear_profile
	: public Open_profil
{
public:
	Linear_profile();
	Linear_profile(std::string num);
	float getProfile_length();
protected:
	Numeric_parameter* profile_length;
};

#endif // GEOMETRY_HPP