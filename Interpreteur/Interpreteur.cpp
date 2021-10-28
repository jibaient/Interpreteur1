#include "traitement_fichier.h"
#include "Project.h"
#include "Executable.h"
#include "map_object_ext.h"
#include "add_object.h"
#include "Technology.h"
#include "concatenateTP.h"
#include "Param_machine.h"

#include <iterator>
#include <utility>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#include <boost/any.hpp>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <boost/circular_buffer.hpp>

namespace ublas = boost::numeric::ublas;

/*
* Amélioration :
*  
* 
*/

using namespace std;
using namespace boost::numeric::ublas;
extern map<string, string> mapEntite;
extern map<string, string> mapName;
extern map<string, boost::any> mapObject; // clé : numéro de la ligne avec le #, valeur : pointeur d'object de type boost::any

int main()
{
    DWORD temps_debut = GetTickCount64(); // Pour le calcul du temps d'éxécution

    //string const nomFichier("C:/piece02.stpnc"); // Récupération de l'adresse
    string const nomFichier("piece.stpnc"); // Récupération de l'adresse
    //string const nomFichier("C:/Sample1_Part12.stpnc"); // Récupération de l'adresse
    ifstream monFlux(nomFichier.c_str()); // Initialisation du flux


    if (monFlux) // Le fichier est accessible
    {
        createMap(monFlux); // Création des maps Entite/Str et Name/Str
       
        Project monProjet; // Instanciation du projet
        Workplan& mainWorkplan = *monProjet.its_main_workplan; // TEST : récupération du main workplan
        Executable& WSebauche = *mainWorkplan.its_elements[0]; // récupération du premier workingstep
        cout << "\n *** Generation du parcours d'outils du WS1 *** \n" << endl;
        WSebauche.generateTP();
        cout << "\n *** coutMachiningTP *** \n" << endl;
        WSebauche.coutMachiningTP();
        cout << "\n *** plotPyTP *** \n" << endl;
        WSebauche.plotPyTP();

        //cout << "\n toolpath de tout le projet \n" << endl;
        //toolpath_t WorkplanTP = concatenate_toolpath(mainWorkplan);
      
        cout << "\n *** Look Ahead *** \n" << std::endl;
        WSebauche.lookAhead();
        toolpath_t acclist = WSebauche.getTP();

        cout << "\n *** acc dec control *** \n" << std::endl;
        WSebauche.accDec();
        cout << "\n *** rough Interpolation  *** \n" << std::endl;
        WSebauche.roughInterpolation();

        toolpath_t TPtest = WSebauche.getTP();

        /*
        cout << "start point" << TPtest[1]->its_start_point << endl;
        for (auto element : TPtest[1]->its_rough_interpolation)
        {
            cout << element << endl;
        }
        cout << "end point" << TPtest[1]->its_end_point << endl;
        */
        cout << "* \n *** machining TP  *** \n" << std::endl;
        WSebauche.coutMachiningTP();

        cout << "* \n *** plot Py TP  *** \n" << std::endl;
        WSebauche.plotPyTPInterpolation();
    }
    else // Le fichier n'est pas accessible
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
    }

    DWORD temps_execution = GetTickCount64() - temps_debut; // Calcul du temps d'éxécution
    cout << endl <<"temps ecoule : " << temps_execution << " ms" <<endl; // Affichage du temps d'éxécution

    system("PAUSE");
    return 0;
}


