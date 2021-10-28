#ifndef ADD_OBJECT_HPP
#define ADD_OBJECT_HPP

#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <iostream>

#include <boost/any.hpp>

#include "traitement_fichier.h"
#include "map_object_ext.h"

extern std::map<std::string, boost::any> mapObject;

bool optional(std::string str);

template <typename T> // le type de l'objet
T* addObject(std::string numHash)
/* Entrée : la classe de l'objet, le numéro de l'entité
*  Sortie : Renvoie le pointeur de l'objet créé
*/
{
	std::map<std::string, boost::any>::iterator it = mapObject.find(numHash);
	if (it != mapObject.end()) // l'objet a déja été instancié
	{
		std::cout << numHash << " deja instancie" <<  std::endl;
		return boost::any_cast<T*>(mapObject[numHash]); // Renvoie le pointeur associé
	}

	else // l'objet n'est pas encore instancié
	{	
		std::cout << "instanciation de " << numHash << std::endl;
		T* newObjectPtr = new T(numHash); // Instanciation de l'objet et stockage de son pointeur
		mapObject[numHash] = newObjectPtr; // Stockage du pointeur dans la mapObject
		return newObjectPtr; // Renvoie du pointeur associé
	}
}

template <typename Tv> // le type de l'object pour ceux avec vecteur
std::vector<Tv*> addObjectVec(std::string numVecHash)
/* De même que la fonction ci-dessue mais répète l'opération pour chacun des éléments du vecteur
*  Fonctionne uniquement si c'est un vecteur d'objet homogène
*/
{
	std::vector<Tv*> vec;
	for (auto element : stringToVector(numVecHash))
	{
		vec.push_back(addObject<Tv>(element));
	}
	return vec;
}

#endif // ADD_OBJECT_HPP