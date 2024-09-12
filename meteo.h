#ifndef METEO_H
#define METEO_H

/*
Auteurs : Swifties
Date : Decembre 2022 - Janvier 2023
*/


  /* STRUCTURES */

/*
Structure Relevé :
Permet de traduire une ligne du fichier csv en un ensemble de variables ou sont stockées les données correspondentes
*/
typedef struct releve {
  char* date;
  float directionvent;
  float vitessevent;
  float humidite;
  float pressionstation;
  float variationpression; 
  float temperature;
  float tempmin;
  float tempmax;
} releve;

/*
Structure Station : 
Permet de rassember tous les relevés (et les données telle que les coordonnées et l'altitude) d'une meme station
*/
typedef struct station {
  int id;
  float latitude;
  float longitude;
  int altitude;
  int nbrreleves;
  releve** tabreleves;
  float humiditemax;                        
} station;

/*
Structure Arbre Station
Permet de rassembler toutes les stations dans un arbre ordonné (par défaut par ID de station)
*/
typedef struct noeudstation {
  station* s;
  struct noeudstation* filsG;
  struct noeudstation* filsD;
} noeudstation;
typedef noeudstation* arbrestation;






  /* FONCTIONS */

/*
Résumé : Permet de lire le fichier csv et de stocker les données de manière adaptée à l'aide des structures précédentes.
Entrées : fichier de données, option (température, pression, vent, hauteur, humidité), et les valeurs de filtres
Sortie : Arbre de station contenant toutes les données du fichier classées à l'aide des structures précédentes.
*/
arbrestation lireDonnees(char* fichier, int option, float ming, float maxg, float mina, float maxa, char* mind, char* maxd);



/*
Résumé : Fonction permettant de créer un fichier pour l'option température. Elle effectue les calculs nécessaires (moyenne, min, max) et écrit les données dans le fichier selon l'ordre voulu. 
Entrées : arbre de station, entier qui correspond au mode souhaité (1,2 ou 3)
Sortie : le fichier rempli avec les données sur les températures 
*/
void temperature (arbrestation a, int mode);

/*
Résumé : Fonction permettant de créer un fichier pour l'option pression. Elle effectue les calculs nécessaires (moyenne, min, max) et écrit les données dans le fichier selon l'ordre voulu. 
Entrées : arbre de station, entier qui correspond au mode souhaité (1,2 ou 3)
Sortie : le fichier rempli avec les données sur les pressions 
*/
void pression (arbrestation a, int mode);
/*
Résumé : Fonction permettant de créer un fichier pour l'option vent. Elle effectue les calculs nécessaires (moyenne) et écrit les données dans le fichier selon l'ordre voulu. 
Entrées : arbre de station
Sortie : le fichier rempli avec les données sur le vent 
*/
void vent (arbrestation a);
/*
Résumé : Fonction permettant de créer un fichier pour l'option hauteur. Elle écrit les données dans le fichier selon l'ordre voulu. 
Entrées : arbre de station
Sortie : le fichier rempli avec les données sur les hauteur 
*/
void hauteur(arbrestation a);
/*
Résumé : Fonction permettant de créer un fichier pour l'option humidité. Elle écrit les données dans le fichier selon l'ordre voulu. 
Entrées : arbre de station
Sortie : le fichier rempli avec les données sur l'humidité 
*/
void humidite (arbrestation a);



/*
Résumé : fonction permettant de filtrer les données à lire sur la longitude . Elle vérifie les données à lire correspondantes au filtres. Si les données correspondent aux filtres, on les lit (1), sinon on ne les lit pas (0).
Entrées : pointeur de station, variable qui contient la longitude min et longitude max 
Sortie : 1 si les données sont lues, 0 sinon 
*/
int filtrelongitude(station* s, float longmin, float longmax);
/*
Résumé : fonction permettant de filtrer les données à lire sur la latitude . Elle vérifie les données à lire correspondantes au filtres. Si les données correspondent aux filtres, on les lit (1), sinon on ne les lit pas (0).
Entrées : pointeur de station, variable qui contient la latitude min et latitude max 
Sortie : 1 si les données sont lues, 0 sinon 
*/
int filtrelatitude(station* s, float latimin, float latimax);
/*
Résumé : fonction permettant de filtrer les données à lire sur les dates . Elle vérifie les données à lire correspondantes au filtres. Si les données correspondent aux filtres, on les lit (1), sinon on ne les lit pas (0).
Entrées : pointeur de station, variable qui contient la date  min et date max 
Sortie : 1 si les données sont lues, 0 sinon 
*/
int filtredate(releve* r, char* datemin, char* datemax);
/*
Résumé : fonction permettant de comparer les dates.
Entrées : pointeur des deux dates à comparer  
Sortie : 1 si les données sont lues, 0 sinon 
*/
int comparaisondates(char* date1, char* date2); 



/*
Résumé : fonction d'affichage pour vérifier la bonne lecture des données
Entrée : arbre de station 
Sortie : rien 
*/
void affichageA (arbrestation a);



/*
Résumé : fonction permettant de libérer la mémoire utilisée par les structures pour stocker les différentes données du fichier csv
Entrée : arbre de station 
Sortie : rien 
*/
void freearbrestation(arbrestation a);


#endif