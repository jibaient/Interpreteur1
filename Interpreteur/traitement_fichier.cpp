#include "traitement_fichier.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

/*
* Permet de traiter le fichier .stpnc avec des fonctions de bases pour récupérer, le numéro; le nom, la chaine de caractères et les attributs des entités
* Permet de créer les maps (dictionaires) qui sont utilisées dans tout le programme pour accéder facilement aux paramètres
*
*Amélioration : 
* 
* Ajouter une fonction qui met plusieurs lignes sur la même avec vérification du ; à la fin
* Ajouter le delEspace dans chacune des fonctions
* Faire des vérifications sur l'intégrité du code : une seule fois projet dans le fichier, qu'il y est bien un project,pas de doublon dans les numéros d'entités,...
*/

map<string, string> mapEntite; // Variable global de la map Entite/str
map<string, string> mapName; // Variable global de la map Name/str

int getNumEntity(int posHash, string ligne)
/* Entrée : position du #, ligne
*  Sortie : le nombre derrière le #
*/
{
    int curseur(posHash);
    curseur++;
    string numStrEntity;
    while (isdigit(ligne[curseur]))
    {
        numStrEntity += ligne[curseur];
        curseur++;
    }
    int numEntity = stoi(numStrEntity);
    return numEntity;
}

string getHashEntity(int posHash, string ligne)
/* Entrée : position du #, ligne
*  Sortie : le numéro de l'entité avec le #
*/
{
    int curseur(posHash);
    curseur++;
    string numStrEntity="#";
    while (isdigit(ligne[curseur]))
    {
        numStrEntity += ligne[curseur];
        curseur++;
    }
    return numStrEntity;
}

string getStrEntity(string ligne)
/* Entrée : ligne
*  Sortie : la chaine de caractère après le = sans le ; final
*/
{
    int posEgal = ligne.find("=");
    return ligne.substr(posEgal + 1, ligne.length() - posEgal - 2);
}

string getNameEntity(string ligne)
/* Entrée : ligne
*  Sortie : le nom de l'entité après le = et avant la parenthèse
*/
{
    int posParenth = ligne.find("(");
    int posEgal = ligne.find("=");
    return ligne.substr(posEgal + 1, posParenth - posEgal - 1);
}

string delCom(string ligne)
/* Entrée : ligne
*  Sortie : la ligne sans les commentaires sous la forme /* commentaire * /
*/
{
    int posDebut = ligne.find("/*");
    int posFin = ligne.find("*/");
    while (posDebut > 0)
    {
        ligne.erase(ligne.begin() + posDebut, ligne.begin() + posFin + 2);
        posDebut = ligne.find("/*");
        posFin = ligne.find("*/");
    }
    return ligne;
}

string delEspace(string ligne)
/* Entrée : ligne
*  Sortie : ligne sans les espaces avant et après la chaine de caratère
*/
{
    while (ligne[0] == ' ')
    {
        ligne.erase(ligne.begin(), ligne.begin() + 1);
    }
    if (ligne.size() > 0)
    {
        while (ligne[ligne.size() - 1] == ' ')
        {
            ligne.pop_back();
        }
    }
    return ligne;
}

vector<string> getParam(string ligne)
/* Entrée : ligne
*  Sortie : Un vector qui contient les paramètres de l'entité sous forme de string
*  Amélioration : modifier la condition du while (pas prendre la virgule) mais plutôt quand la ligne est vide pour éviter le dernier if
*/
{
    vector<string> listeParam(0);
    int posDebut = ligne.find("(") + 1;
    int posParenthF = 0;
    ligne.erase(ligne.begin(), ligne.begin() + posDebut); // Supprime le nom de l'entité du string
    int posFin = ligne.find(",");

    while (posFin >= 0) // récupère les attibuts jusqu'a ce qu'il n'y est plus de virgule, cad qu'il reste un dernier argument, où qu'il n'y a plus d'argument dans le cas ou le dernier argument est un tuple
    {
        listeParam.push_back(delEspace(ligne.substr(0, posFin)));
        ligne.erase(ligne.begin(), ligne.begin() + posFin + 1);

        if (ligne[0] == '(') // Traite le cas où le paramètre est un tuple
        {
            posParenthF = ligne.find(")");
            listeParam.push_back(ligne.substr(0, posParenthF + 1));
            ligne.erase(ligne.begin(), ligne.begin() + posParenthF + 1);
            while (ligne[0] == ' ')
            {
                ligne.erase(ligne.begin(), ligne.begin() + 1);
            }
            ligne.erase(ligne.begin(), ligne.begin() + 1);
        }
        posFin = ligne.find(",");
    }
    if (ligne[0] != ')' and ligne != "") // récupère le dernier attribut s'il est présent
    {
        listeParam.push_back(delEspace(ligne.substr(0, ligne.length() - 1)));
    }
    return listeParam;
}

map<string, string> createMap(ifstream & flux)
/* Entrée : la référence du flux du fichier que l'on veut lire
*  Modifie la variable global mapEntite qui est une varibale externe
*  Sortie : map Entite/str 
*/
{
    string ligne;
    while (getline(flux, ligne)) // Traverse toutes les lignes du fichier
        {
        if (ligne[0] == '#') // Uniquement les entitées donc commencant par un #
        {
            string strEntity = delEspace(getStrEntity(delCom(delEspace(ligne))));
                mapEntite[getHashEntity(0, ligne)] = strEntity;
                mapName[delEspace(getNameEntity(ligne))] = strEntity;
        }
    } 
    return mapEntite;
}

vector<string> stringToVector(string parametreStr)
/* Entrée : la ligne de parametre sous forme de string
*  Sortie : les paramètres sous forme de vecteur de string
*/
{
    vector<string> vecteurStr;
    parametreStr.erase(parametreStr.begin(), parametreStr.begin() + 1); //supprime la première parenthèse
    int posFin = parametreStr.find(",");
    while (posFin >= 0)
    { 
        vecteurStr.push_back(delEspace(parametreStr.substr(0, posFin)));
        parametreStr.erase(parametreStr.begin(), parametreStr.begin() + posFin + 1);
        posFin = parametreStr.find(",");
    }
    vecteurStr.push_back(delEspace(parametreStr.substr(0, parametreStr.length() - 1)));
    return vecteurStr;
}

bool stringToBool(string boolStr)
/* Entrée : la ligne de parametre sous forme de string
*  Sortie : le paramètre sous forme de booléen
*/
{
    if (delEspace(boolStr) == ".F.")
    {
        return false;
    }
    else if (delEspace(boolStr) == ".T.")
    {
        return true;
    }
    else
    {
        cout << "Attention probleme de conversion string en Bool" << endl;
        return false;
    }
}
