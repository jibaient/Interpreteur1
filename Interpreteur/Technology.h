#ifndef TECHNOLOGY_HPP
#define TECHNOLOGY_HPP

#include <string>
#include <vector>

#include "Geometry.h"
#include "Workpiece.h"

class Process_model
{
public:
protected:
	std::string ini_data_file;
	std::string its_type;
};

class Process_model_list
{
public:
protected:
	std::vector<Process_model*> its_list;
};

class Speed_select
{
public:
	virtual float getRot_speed();
protected:
};
class Const_spindle_speed
	: public Speed_select
{
public:
	Const_spindle_speed();
	Const_spindle_speed(std::string num);
	virtual float getRot_speed();
protected:
	float rot_speed;
};
class Const_cutting_speed
	: public Speed_select
{
public:
	Const_cutting_speed();
	Const_cutting_speed(std::string num);
protected:
	float speed_measure;
	float rot_speed; // OPTIONAL
};
class Technology //abs
{
public:
	virtual void calculateFeedrate();
	virtual float getFeedrate();

public: // !!! remetre protected juste pour test !!!
	float feedrate; // OPTIONAL [m/s]
	std::string feedrate_reference; // TYPE tool_reference_point = ENUMERATION OF (tcp, ccp);
};

class Turning_technology
	:public Technology
{
public:
	Turning_technology();
	Turning_technology(std::string num);
	virtual void calculateFeedrate();
	virtual float getFeedrate();

public: // !!! remetre protected juste pour test !!!
	Speed_select* spindle_speed;
	float feedrate_per_revolution; // OPTIONAL [mm/rev]
	bool sync_spindle_and_z_feed;
	bool inhibit_feedrate_override;
	bool inhibit_spindle_override;
};

class Machine_functions
{
};

class Turning_machine_functions
	: public Machine_functions
{
public:
	Turning_machine_functions();
	Turning_machine_functions(std::string num);
protected:
	bool coolant;
	std::string coolant_type; // OPTIONAL //TYPE coolant_select = ENUMERATION OF (flood, mist, through_tool);
	float coolant_pressure; // OPTIONAL
	std::vector<std::string> axis_clamping; // [0:?]
	bool chip_removal;
	Direction*  oriented_spindle_stop; // OPTIONAL
	Process_model_list* its_process_model; // OPTIONAL
	std::vector<Property_parameter*> other_functions; // [0:?]
	bool tail_stock; // OPTIONAL
	bool steady_rest; // OPTIONAL
	bool follow_rest; // OPTIONAL
};

#endif // TECHNOLOGY