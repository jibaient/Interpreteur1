#ifndef TRAITEMENT_FICHIER_HPP
#define TRAITEMENT_FICHIER_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>

int getNumEntity(int posHash, std::string ligne);
std::string getHashEntity(int posHash, std::string ligne);
std::string getStrEntity(std::string ligne);
std::string getNameEntity(std::string ligne);
std::string delCom(std::string ligne);
std::string delEspace(std::string ligne);
std::vector<std::string> getParam(std::string ligne);
std::map<std::string, std::string> createMap(std::ifstream &flux);
std::vector<std::string> stringToVector(std::string parametreStr);
bool stringToBool(std::string);

#endif // TRAITEMENT_FICHIER_HPP
