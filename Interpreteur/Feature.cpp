#include "Feature.h"
#include "Workpiece.h"
#include "Geometry.h"
#include "Operation.h"
#include "traitement_fichier.h"
#include "add_object.h"
#include "transformation_matrix.h"
#include "toolpath.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace ublas = boost::numeric::ublas;

using namespace std;



extern map<string, string> mapEntite;

Revolved_flat::Revolved_flat()
{
}
Revolved_flat::Revolved_flat(std::string num)
{
	vector<string> param = getParam(mapEntite[num]);
	cout << "instanciation de Revolved_flat " << param[0] << endl;
	its_id = param[0];
	its_workpiece = addObject<Workpiece>(param[1]);
	for (auto element : stringToVector(param[2]))
	{ 
		string nameOperation = getNameEntity(mapEntite[element]);
		if (nameOperation == "FACING_ROUGH")
		{
			its_operations.push_back(addObject<Facing_rough>(element));
		}
		else if (nameOperation == "FACING_FINISH")
		{
			its_operations.push_back(addObject<Facing_finish>(element));
		}
		else
		{
			cout << nameOperation << " pas implemente dans Operation" << endl;
		}
	}
	feature_placement = addObject<Axis2_placement_3d>(param[3]);
	if (optional(param[4])) { material_side = addObject<Direction>(param[4]); }
	if (optional(param[5])) { radius = stof(param[5]); }
	flat_edge_shape = addObject<Linear_profile>(param[6]);
}

toolpath_t Manufacturing_feature::generateFeatureTP(Operation* its_operation, Elementary_surface* its_secplane)
{
	return toolpath_t();
}
toolpath_t Two5D_manufacturing_feature::generateFeatureTP(Operation* its_operation, Elementary_surface* its_secplane)
{
	return toolpath_t();
}
toolpath_t Turning_feature::generateFeatureTP(Operation* its_operation, Elementary_surface* its_secplane)
{
	return toolpath_t();
}
toolpath_t Revolved_feature::generateFeatureTP(Operation* its_operation, Elementary_surface* its_secplane)
{
	return toolpath_t();
}
toolpath_t Revolved_flat::generateFeatureTP(Operation* its_operation, Elementary_surface* its_secplane)
{
	/*Am�lioration :
	*
	* Prendre en compte l'usinage sur Z
	* Prendre en compte l'usinage sur -X, -Z
	* Prendre en compte le plan de securit� si pas de lift_height
	* Prendre en compte le start/end point si donn�
	*/
	cout << "Generation du parcours d'outils : "<< its_id << endl;
	//Tous les points sont plac�s dans le rep�re de la pi�ce sauf indication contraire
	float zStock = 44; //Valeur qui devrait �tre mesur�e 
	float dStock = 20; //Valeur qui devrait �tre mesur�e
	float eps = 2; // Valeur de sortie du parcours d'usinage en dehors de la pi�ce dans la phase de mouvement rapide avant l'approche

	toolpath_t  toolpath; // vec de Toolpath_segment
	Turning_machining_strategy* machining_strategy = its_operation->getMachining_Strategy();
	Direction* feed_direction = machining_strategy->getFeed_direction();
	float lift_height = machining_strategy->getLift_height();
	std::vector<float> cutting_depth = machining_strategy->getCutting_depth();
	float allowance = its_operation->getAllowance();

	float r = radius;
	float l = flat_edge_shape->getProfile_length();
	
	// Matrice de transformation Workpiece -> Feature
	std::vector<float> vecXwf = { 1, 0, 0 }; // valeur par d�fault ref_direction;
	std::vector<float> vecZwf = { 0, 0, 1 }; // valeur par d�fault axis;
	std::vector<float> vecLwf = feature_placement->getLocation();

	if (feature_placement->getXDirection() != nullptr)
	{
		vecXwf = feature_placement->getXAxis();
	}
	if (feature_placement->getZDirection() != nullptr)
	{
		vecZwf = feature_placement->getZAxis();
	}
	ublas::matrix<float> Mwf = transformationMatrix(vecXwf,vecZwf,vecLwf) ; 

	// Matrice transformation Feature -> Linear_profil
	std::vector<float> vecXflp = { 1, 0, 0 }; // valeur par d�fault ref_direction;
	std::vector<float> vecZflp = { 0, 1, 0 }; // valeur par d�fault axis;
	std::vector<float> vecLflp = { r, 0, 0 }; // Position du rep�re de Linear_profil
	if (flat_edge_shape->getPlacement() != nullptr)
	{
		Axis2_placement_3d* flat_edge_shapePlacement = flat_edge_shape->getPlacement();
		if (flat_edge_shapePlacement->getXDirection() != nullptr)
		{
			vecXflp = flat_edge_shapePlacement->getXAxis();
		}
		if (flat_edge_shapePlacement->getZDirection() != nullptr)
		{
			vecZflp = flat_edge_shapePlacement->getZAxis();
		}
	}
	ublas::matrix<float> Mflp = transformationMatrix(vecXflp, vecZflp, vecLflp); 


	if (feed_direction==nullptr or feed_direction->getDirection_ratios()[2] != 0) // Usinage selon Z
	{
		cout << "selon Z" << endl;
	}
	else if(feed_direction->getDirection_ratios()[0] != 0 ) // Usinage selon -X
	{
		cout << "selon -X" << endl;
		cout << "r : " << r << " ,l : " << l << endl;

		ublas::vector<float> start_point(4); // point de d�part de la mati�re � usiner
		start_point(0) = dStock/2;
		start_point(1) = 0;
		start_point(2) = zStock;
		start_point(3) = 1;

		//dans le rep�re du linear profile
		start_point = ublas::prod(inverseMatrix(Mwf), start_point); //WCS -> FCS
		start_point = ublas::prod(inverseMatrix(Mflp), start_point); //FCS -> LpCS
		start_point(0) = l;

		/*****g�n�ration du parcours d'approche*****/
		Approach_retract_strategy* ap_strategy=its_operation->getApproach();
		Approach_retract_strategy* retract_strategy = its_operation->getRetract();
		float retract_plane_height = its_secplane->getLocation()[2] ; //on initalise le retract plane � la coordonn�e Z du plan de s�curit�
		if (its_operation->getRetract_plane()!=NAN) // si l'op�ration poss�de une hauteur de retrait on l'utilise pour le retract_plane_height
		{
			retract_plane_height = its_operation->getRetract_plane();
		}

		// Matrice transformation Linear_profil -> approach
		// Du fait que l'on se place dans le rep�re Linear_profil il faut que l'approach soit tangent au profile, or l'approache est bas� sur un rep�re x Z, tangent � x
		std::vector<float> vecXlpa = {0, -1, 0 };
		std::vector<float> vecZlpa = { 1, 0, 0 }; 
		std::vector<float> vecLlpa = { 0, 0, 0 }; // Position du rep�re de Linear_profil
		ublas::matrix<float> Mlpa = transformationMatrix(vecXlpa, vecZlpa, vecLlpa);
		
		ublas::vector<float> start_point_ACS = ublas::prod(inverseMatrix(Mlpa), start_point); // start_point dans le rep�re de l'approach
		start_point_ACS(0) = start_point_ACS(0) - cutting_depth[0]; // le point d'approche est situ� � la premi�re passe d'usinage apr�s le point de d�part mati�re
		toolpath_t  toolpath_ap;
		toolpath_ap = ap_strategy->generateApproachTP(start_point_ACS,retract_plane_height);

		for (auto element : toolpath_ap) // ajoute les �l�ments du parcours d'approach en effectuant la transformation : ACS -> LPCS
		{
			element->its_end_point = ublas::prod(Mlpa, element->its_end_point);
			element->its_end_point = ublas::prod(Mflp, element->its_end_point);
			element->its_end_point = ublas::prod(Mwf, element->its_end_point);

			element->its_start_point = ublas::prod(Mlpa, element->its_start_point);
			element->its_start_point = ublas::prod(Mflp, element->its_start_point);
			element->its_start_point = ublas::prod(Mwf, element->its_start_point);

			if (element->its_parameter== element->its_parameter) // v�rifie qu'il y a un centre, cad qu'il y a un rayon car les sections lin�aires n'ont pas de centre
			{
				element->its_center = ublas::prod(Mlpa, element->its_center);
				element->its_center = ublas::prod(Mflp, element->its_center);
				element->its_center = ublas::prod(Mwf, element->its_center);
			}


			toolpath.push_back(new Toolpath_section(element->its_start_point,element->its_end_point, element->its_interpolation_type, element->its_movement_type,element->its_parameter,element->its_center ));
		}

		/*****g�n�ration du parcours d'usinage*****/
		ublas::vector<float> current_point(4);
		current_point = start_point;
		ublas::vector<float> current_pointWCS(4); // correspond au end point
		ublas::vector<float> start_pointWCS(4); // correspond au start point, cad au end point du point d'avant
		current_pointWCS = ublas::prod(ublas::prod(Mwf, Mflp),current_point); // LpCS -> FCS ->WCS
		start_pointWCS = current_pointWCS;
		current_point(1) = current_point(1) - lift_height; // d�calage a une distance lift_height du brute pour compenser le fait que le calcul de la profondeur de passe enl�ve une �paiseur lift_height pour les passes suivantes
		int i = 0; // Compteur pour les cutting_depth
		bool isNotFirstApproach = false;
		while (current_point(1)+lift_height<-allowance)
		{
			if (cutting_depth[i]  < abs(current_point(1) + lift_height + allowance)) // La prodondeur de passe est inf�rieur � la mati�re restante � usiner
			{
				current_point(1) = current_point(1)+lift_height+cutting_depth[i];// r�alisation d'une passe de profondeur d�finie par cutting_depth.
			}
			else
			{
				current_point(1) = -allowance;// r�alisation d'une passe de profondeur jusqu'a l'allowance
			}
			current_pointWCS = ublas::prod(ublas::prod(Mwf, Mflp), current_point);
			if (isNotFirstApproach) // on ne met pas d'approahe � la premi�re it�ration car l'approche est g�n�r� par le mouvement d'approche g�n�rale vers la feature
			{
				toolpath.push_back(new Toolpath_section(start_pointWCS, current_pointWCS, "linear", "approach"));
			}
			isNotFirstApproach = true;
			start_pointWCS = current_pointWCS;
			current_point(0) = 0;
			current_pointWCS = ublas::prod(ublas::prod(Mwf, Mflp), current_point);
			toolpath.push_back(new Toolpath_section(start_pointWCS, current_pointWCS, "linear","machining"));
			start_pointWCS = current_pointWCS;
			current_pointWCS = ublas::prod(ublas::prod(Mwf, Mflp), current_point);
			current_point(1) = current_point(1) - lift_height; // AJOUTER si pas de lift_heigt aller au plan de securit�
			current_pointWCS = ublas::prod(ublas::prod(Mwf, Mflp), current_point);
			toolpath.push_back(new Toolpath_section(start_pointWCS, current_pointWCS, "linear", "retract"));
			start_pointWCS = current_pointWCS;
			current_point(0) = l + eps;
			current_pointWCS = ublas::prod(ublas::prod(Mwf, Mflp), current_point);
			toolpath.push_back(new Toolpath_section(start_pointWCS, current_pointWCS, "linear", "move"));
			start_pointWCS = current_pointWCS;

			if (i + 1 < cutting_depth.size()) // s'il reste des �l�ments dans la liste de cutting_depth, on passe au suivant. Sinon on continue d'utiliser le dernier le liste.
			{
				i++;
			}	
		}

		/*****g�n�ration du parcours d'usinage de retrait*****/
		if (retract_strategy != nullptr)
		{
			toolpath.pop_back();
			toolpath.pop_back(); // supression des �l�ments de retrait si l'op�ration poss�de sa strat�gie de retrait final

			toolpath_t toolpath_retract;
			ublas::vector<float> retractStart_point(4); // point de d�part du retrait dans le rep�re de la pi�ce
			retractStart_point = toolpath.back()->its_end_point;

			ublas::vector<float> retractStart_point_RCS(4);
			retractStart_point = ublas::prod(inverseMatrix(Mwf), retractStart_point); //WCS -> FCS
			retractStart_point = ublas::prod(inverseMatrix(Mflp), retractStart_point); //FCS -> LpCS
			retractStart_point_RCS = ublas::prod(inverseMatrix(Mlpa), retractStart_point); // d�placement dans le rep�re du retrait

			toolpath_retract = retract_strategy->generateRetractTP(retractStart_point_RCS, retract_plane_height); // retract plane height pas utilis� et ne correspond pas pour l'instant (devrait �tre diff�rent pour le retratit et l'approche);
			
			for (auto element : toolpath_retract) // ajoute les �l�ments du parcours de retrait en effectuant la transformation : ACS -> LPCS
			{
				element->its_end_point = ublas::prod(Mlpa, element->its_end_point);
				element->its_end_point = ublas::prod(Mflp, element->its_end_point);
				element->its_end_point = ublas::prod(Mwf, element->its_end_point);

				element->its_start_point = ublas::prod(Mlpa, element->its_start_point);
				element->its_start_point = ublas::prod(Mflp, element->its_start_point);
				element->its_start_point = ublas::prod(Mwf, element->its_start_point);


				if (element->its_parameter == element->its_parameter) // v�rifie qu'il y a un centre, cad qu'il y a un rayon car les sections lin�aires n'ont pas de centre
				{
					element->its_center = ublas::prod(Mlpa, element->its_center);
					element->its_center = ublas::prod(Mflp, element->its_center);
					element->its_center = ublas::prod(Mwf, element->its_center);
				}

				toolpath.push_back(new Toolpath_section(element->its_start_point, element->its_end_point, element->its_interpolation_type, element->its_movement_type, element->its_parameter,element->its_center));
			}

		}
		
		


	}
	return toolpath;
}

