#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <windows.h> // pour le compteur
using namespace std;


/*
* Ajouter une fonction qui met plusieurs lignes sur la même avec vérification du ; à la fin
*
*
*
*/

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

string getStrEntity(string ligne)
/* Entrée : ligne
*  Sortie : la chaine de caractère après le = sans le ; final
*/
{
    int posEgal = ligne.find("=");
    return ligne.substr(posEgal + 1, ligne.length()- posEgal-2);
}

string getNameEntity(string ligne)
/* Entrée : ligne
*  Sortie : le nom de l'entité après le = et avant la parenthese
*/
{
    int posParenth = ligne.find("(");
    int posEgal = ligne.find("=");
    return ligne.substr(posEgal+1, posParenth-posEgal-1);
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

    return ligne ;
}

string delEspace(string ligne)
/* Entrée : ligne
*  Sortie : ligne sans les espaces avant et après la chaine de caratère
*/
{
    while (ligne[0]  == ' ')
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
* 
* Amélioration : modifier la condition du while (pas prendre la virgule) mais plutôt quand la ligne est vide pour éviter le dernier if
* 
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
        ligne.erase(ligne.begin(), ligne.begin() + posFin+1);

        if (ligne[0] == '(') // Traite le cas où le paramètre est un tuple
        {
            posParenthF = ligne.find(")");
            listeParam.push_back(ligne.substr(0, posParenthF+1));
            ligne.erase(ligne.begin(), ligne.begin() + posParenthF+1);
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
        listeParam.push_back(delEspace(ligne.substr(0,ligne.length() - 1)));
    }
    return listeParam;
}

int main()
{
    DWORD temps_debut = GetTickCount64();

    string const nomFichier("C:/piece02.stpnc"); // récupère l'adresse
    ifstream monFlux(nomFichier.c_str()); // initialise le flux

    if (monFlux) // vérifie que le fichier est bien accesible
    {
        string ligne;
        while (getline(monFlux, ligne)) // traverse toute les lignes du fichier
        {
            if (ligne[0] == '#') // on ne s'intéresse qu'aux entités donc commencant par un #
            {
                int numEntity = getNumEntity(0, ligne);
                string strEntity = delEspace(delCom(getStrEntity(ligne)));
                string nameEntity = getNameEntity(ligne);
                vector<string> parametre = getParam(strEntity);
                /*
                cout << "Numero d entite : " << numEntity << endl;
                cout << "Nom de l entite : " << nameEntity << endl;
                
                int compteur = 0;
                for (auto element : parametre)
                {

                    cout << "Element " << compteur << " : " << element << endl;
                    compteur++;
                }
                cout << endl;
                */
            }
        }
    }
    else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
    }

    DWORD temps_execution = GetTickCount64() - temps_debut;
    cout << "temps ecoule : " << temps_execution << " ms";
    //system("PAUSE");
    return 0;
}


