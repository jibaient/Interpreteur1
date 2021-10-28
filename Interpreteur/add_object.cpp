#include "add_object.h"
#include <map>
#include <string>
#include <iostream>
#include <boost/any.hpp>


bool optional(std::string str)
{
	/*
	* Si l'argument optionnel est donnée : renvoie true
	* renvoie false sinon (cad s'il est indiqué $ ou () comme argument)
	*/
	return (str != "$" && str != "()");
}


