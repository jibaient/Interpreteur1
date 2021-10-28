#include "traitement_fichier.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

/*
* Permet de traiter le fichier .stpnc avec des fonctions de bases pour r�cup�rer, le num�ro; le nom, la chaine de caract�res et les attributs des entit�s
* Permet de cr�er les maps (dictionaires) qui sont utilis�es dans tout le programme pour acc�der facilement aux param�tres
*
*Am�lioration : 
* 
* Ajouter une fonction qui met plusieurs lignes sur la m�me avec v�rification du ; � la fin
* Ajouter le delEspace dans chacune des fonctions
* Faire des v�rifications sur l'int�grit� du code : une seule fois projet dans le fichier, qu'il y est bien un project,pas de doublon dans les num�ros d'entit�s,...
*/

map<string, string> mapEntite; // Variable global de la map Entite/str
map<string, string> mapName; // Variable global de la map Name/str

int getNumEntity(int posHash, string ligne)
/* Entr�e : position du #, ligne
*  Sortie : le nombre derri�re le #
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
/* Entr�e : position du #, ligne
*  Sortie : le num�ro de l'entit� avec le #
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
/* Entr�e : ligne
*  Sortie : la chaine de caract�re apr�s le = sans le ; final
*/
{
    int posEgal = ligne.find("=");
    return ligne.substr(posEgal + 1, ligne.length() - posEgal - 2);
}

string getNameEntity(string ligne)
/* Entr�e : ligne
*  Sortie : le nom de l'entit� apr�s le = et avant la parenth�se
*/
{
    int posParenth = ligne.find("(");
    int posEgal = ligne.find("=");
    return ligne.substr(posEgal + 1, posParenth - posEgal - 1);
}

string delCom(string ligne)
/* Entr�e : ligne
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
/* Entr�e : ligne
*  Sortie : ligne sans les espaces avant et apr�s la chaine de carat�re
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
/* Entr�e : ligne
*  Sortie : Un vector qui contient les param�tres de l'entit� sous forme de string
*  Am�lioration : modifier la condition du while (pas prendre la virgule) mais plut�t quand la ligne est vide pour �viter le dernier if
*/
{
    vector<string> listeParam(0);
    int posDebut = ligne.find("(") + 1;
    int posParenthF = 0;
    ligne.erase(ligne.begin(), ligne.begin() + posDebut); // Supprime le nom de l'entit� du string
    int posFin = ligne.find(",");

    while (posFin >= 0) // r�cup�re les attibuts jusqu'a ce qu'il n'y est plus de virgule, cad qu'il reste un dernier argument, o� qu'il n'y a plus d'argument dans le cas ou le dernier argument est un tuple
    {
        listeParam.push_back(delEspace(ligne.substr(0, posFin)));
        ligne.erase(ligne.begin(), ligne.begin() + posFin + 1);

        if (ligne[0] == '(') // Traite le cas o� le param�tre est un tuple
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
    if (ligne[0] != ')' and ligne != "") // r�cup�re le dernier attribut s'il est pr�sent
    {
        listeParam.push_back(delEspace(ligne.substr(0, ligne.length() - 1)));
    }
    return listeParam;
}

map<string, string> createMap(ifstream & flux)
/* Entr�e : la r�f�rence du flux du fichier que l'on veut lire
*  Modifie la variable global mapEntite qui est une varibale externe
*  Sortie : map Entite/str 
*/
{
    string ligne;
    while (getline(flux, ligne)) // Traverse toutes les lignes du fichier
        {
        if (ligne[0] == '#') // Uniquement les entit�es donc commencant par un #
        {
            string strEntity = delEspace(getStrEntity(delCom(delEspace(ligne))));
                mapEntite[getHashEntity(0, ligne)] = strEntity;
                mapName[delEspace(getNameEntity(ligne))] = strEntity;
        }
    } 
    return mapEntite;
}

vector<string> stringToVector(string parametreStr)
/* Entr�e : la ligne de parametre sous forme de string
*  Sortie : les param�tres sous forme de vecteur de string
*/
{
    vector<string> vecteurStr;
    parametreStr.erase(parametreStr.begin(), parametreStr.begin() + 1); //supprime la premi�re parenth�se
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
/* Entr�e : la ligne de parametre sous forme de string
*  Sortie : le param�tre sous forme de bool�en
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
