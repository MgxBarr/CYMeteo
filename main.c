#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "meteo.h"





/*
Auteurs : Swifties
Date : Decembre 2022 - Janvier 2023
*/

int main(int argc, char** argv) {
    

            /* Initialisation des variables permttant d'appeler et d'utiliser les fonctions */

    arbrestation a=NULL;

    int ctemperature=0; 
    int cpression=0; 
    int cvent=0; 
    int chauteur=0; 
    int chumidite=0;

    int mode=0;

    int exclu=0;

    int clongitudes=0; 
    int clatitudes=0; 
    int cdates=0; 

    float ming=0;
    float maxg=0; 
    float mina=0;
    float maxa=0; 
    char* mind=NULL;
    mind=malloc(50*sizeof(char));
    char* maxd=NULL;
    maxd=malloc(50*sizeof(char));
    mind="0";
    maxd="0"; 

    char* i="";
    //char* o="";     


    
            /* Utilisation de getopt pour gérer les arguments */

    int options=0;
    int argcompt=1;

    while ((options = getopt (argc, argv, "g:a:d:t:p:whmi:o:H")) != -1) {
        switch (options) {

            //--help (remplacé par -H): affiche une aide sur le programme et le quitte 
            case 'H': 
                printf ("Voici les différentes options que vous pouvez ajouter : \n");
                printf ("--------------------------------------------------------------------------------\n");
                printf ("-H : affiche une aide \n");
                printf ("-t <mode> : températures \n");
                printf ("-p <mode> : pressions atmosphériques \n");
	            printf ("Pour ces 2 options, il faut indiquer la valeur de <mode> :\n 1 : produit en sortie les températures (ou pressions) minimales, maximales et moyennes par station dans l’ordre croissant du numéro de station.\n 2 : produit en sortie les températures (ou pressions)moyennes par date/heure, triées dans l'ordre chronologique. La moyenne se fait sur toutes les stations.\n 3 : produit en sortie les températures (ou pressions) par date/heure par station. Elles seront triées d’abord par ordre chronologique, puis par ordre croissant de l’identifiant de la station.\n"); 
                printf ("-w : Produit en sortie l’orientation moyenne et la vitesse moyenne des vents pour chaque station.  \n");
                printf ("-h: Produit en sortie la hauteur pour chaque station (ordre décroissant). \n");
                printf ("-m : Produit en sortie l’humidité maximale pour chaque station (ordre décroissant). \n");
                printf ("-g <min><max> : Permet de filtrer les données de sortie en ne gardant que les données qui sont dans l’intervalle de longitudes [<min>..<max>] incluses. \n");
                printf ("-a <min><max> : Permet de filtrer les données de sortie en ne gardant que les relevés qui sont dans l’intervalle de latitudes [<min>..<max>] incluses. \n");
                printf ("-d <min><max> : permet de filtrer les dates entre les valeurs min et max. (de forme : AAAA-MM-JJTHH:MM:SS) \n");
                printf ("-i <nom_fichier> : Permet de spécifier le chemin du fichier CSV d’entrée (obligatoire). \n");
                printf ("-o <nom_fichier> : Permet de donner un nom au fichier de sortie contenant les données. \n");
                printf ("--------------------------------------------------------------------------------\n");
                exit(0); 
                break; 
            
            //1 : produit en sortie les températures/pressions minimales, maximales et moyennes par station (ordre croissant d'id de station).
            //2 : produit en sortie les températures/pressions moyennes par date/heure (ordre chronologique) par station.
            //3 : produit en sortie les températures/pressions par date/heure par station (ordre chronologique puis par ordre croissant d'id de station).
            case 't':
                ctemperature++;
                mode=atoi(optarg);
                exclu++;
                argcompt++;
                break;

            case 'p':
                cpression++;
                mode=atoi(optarg); 
                exclu++;
                argcompt++; 
                break;

            //-w : produit en sortie l’orientation moyenne et la vitesse moyenne des vents par station.
            case 'w': 
                cvent++; 
                exclu++;
                break;

            //-h : produit en sortie la hauteur par station (ordre décroissant).
            case 'h': 
                chauteur++; 
                exclu++;
                break;

            //-m : produit en sortie l'humidité maximale par station (ordre décroissant)
            case 'm': 
                chumidite++; 
                exclu++;
                break;
            


            //-g <min><max> : filtre les données selon l'intervalle de longitudes
            case 'g': 
                clongitudes=1; 
                if (argv[argcompt+1]!=NULL) {
                    ming=atof(argv[argcompt+1]);
                }
                if (argv[argcompt+2]!=NULL) {
                    maxg=atof(argv[argcompt+2]);
                }
                argcompt++;
                argcompt++;
                break;

            //-a <min><max> : filtre les données selon l'intervalle de latitudes 
            case 'a': 
                clatitudes=1; 
                if (argv[argcompt+1]!=NULL) {
                    mina=atof(argv[argcompt+1]);
                }
                if (argv[argcompt+2]!=NULL) {
                    maxa=atof(argv[argcompt+2]);
                }
                argcompt++;
                argcompt++;
                break;

            //-d <min><max> : filtre les données selon les dates 
            case 'd': 
                cdates=1;
                if (argv[argcompt+1]!=NULL) {
                    mind=argv[argcompt+1];
                }
                if (argv[argcompt+2]!=NULL) {
                    maxd=argv[argcompt+2];
                }
                argcompt++;
                argcompt++;
                break;



            //-i fichier : définit le fichier d'entrée, OBLIGATOIRE
            case 'i': 
                i=optarg; 
                argcompt++; 
                break; 

            //-o fichier : définit le fichier de sortie
            case 'o': 
                //o=optarg; 
                argcompt++; 
                break; 



        }
        argcompt++; 
    }



            /* Erreurs si les arguments ou l'appel du programme ne sont pas bons */
    
    if (i && !i[0]) {
        printf("Erreur : Veuillez spécifier un fichier d'entrée.\n");
        return 1;
        exit(1);
    }

    if (exclu!=1) {
        printf("Erreur : Veuillez spécifier une seule et unique option exclusive.\n");
        exit(1);
    }
 
    if ((clatitudes>1) || (clongitudes>1) || (cdates>1)) {
        printf("Erreur : Veuillez appeler une seule fois les options de filtrage.\n");
        exit(1);
    }
    
    if ((ming > maxg) || (mina > maxa) || comparaisondates(mind,maxd)==1) { 
        printf("Veuillez spécifier un intervalle correct pour utiliser une des options de filtrage (min <= max).\n");
        exit(1);
    }
    
    

            /* Appel des fonctions */
    
    
    if (ctemperature) {
        a=lireDonnees(i, 1, ming, maxg, mina, maxa, mind, maxd);
        temperature(a,mode);  
    }

    if (cpression) {
        a=lireDonnees(i, 2, ming, maxg, mina, maxa, mind, maxd);
        pression(a,mode);  
    }

    if (cvent) {
        a=lireDonnees(i, 3, ming, maxg, mina, maxa, mind, maxd);
        vent(a);
    }

    if (chauteur) {
        a=lireDonnees(i, 4, ming, maxg, mina, maxa, mind, maxd);
        hauteur(a);
    }

    if (chumidite) {
        a=lireDonnees(i, 5, ming, maxg, mina, maxa, mind, maxd);
        humidite(a);
    }
    

            /* Libération de la mémoire */

    freearbrestation(a);

    return 0;
}