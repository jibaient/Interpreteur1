#ifndef EXECUTABLE_HPP
#define EXECUTABLE_HPP

#include <string>
#include <vector>

#include "Geometry.h"
#include "Feature.h"
#include "Operation.h"
#include "Setup.h"
#include "toolpath.h"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;

class Executable //ABS
{
public:
	Executable();
	virtual void generateTP();
	virtual void coutMachiningTP();
	virtual void plotPyTP();
	virtual void plotPyTPInterpolation();
	virtual void roughInterpolation();
	virtual toolpath_t getTP();

	virtual void lookAhead();
	virtual void accDec();

protected:
	std::string its_id;
};

class Workingstep : //ABS
	public Executable
{
public:
	virtual void generateTP();
	virtual void coutMachiningTP();
	virtual void plotPyTP();
	virtual void plotPyTPInterpolation();
	virtual void roughInterpolation();
	virtual toolpath_t getTP();
	virtual void lookAhead();
	virtual void accDec();


protected:
	Elementary_surface* its_secplane;
};

class Machining_workingstep :
	public Workingstep
{
public:
	Machining_workingstep();
	Machining_workingstep(std::string num);
	virtual void generateTP();
	virtual void coutMachiningTP();
	virtual void plotPyTP();
	virtual void plotPyTPInterpolation();
	virtual void roughInterpolation();
	virtual toolpath_t getTP();
	virtual void lookAhead();
	virtual void accDec();

protected:
	Manufacturing_feature* its_feature;
	Machining_operation* its_operation;
	toolpath_t generated_machining_toolpath; // Pour stockage du TP générer par l'interpreteur
	//In_process_geometry its_effect; // OPTIONAL

	
	std::vector<ublas::vector<float>> interpolated_points;
};

class Turning_workinstep :
	public Workingstep
{
public:
protected:
	std::vector<Manufacturing_feature*> its_features; // [2:?]
	Turning_machining_operation* its_operation; 
	//its_effect in_process_geometry; // OPTIONAL
};


class Program_structure : //ABS
	public Executable
{
public:
protected:
};

class Workplan :
	public Program_structure
{
public:
	Workplan();
	Workplan(std::string num);
	
public: // ATTENTION : Remettre en protected
	std::vector<Executable*> its_elements; // [0:?]
	//Channel its_channel; // OPTIONAL
	Setup* its_setup; // OPTIONAL
	//In_process_geometry its_effect; // OPTIONAL

};

#endif // EXECUTABLE_HPP

