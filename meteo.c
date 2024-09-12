#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "meteo.h"


/*
Auteurs : Swifties
Date : Decembre 2022 - Janvier 2023
*/



/*
Fonctions permettant de gérer l'arbre de station
rechercherStation permet de retourner le station dans laquelle il faut ajouter les données
ajouterStation permet dajouter une station à l'arbre de stations en fonction de l'option
*/
station* rechercherStation (arbrestation a, int idstation) {
  if (a==NULL) {
    return NULL;
  }
  else {
    if (idstation==a->s->id) {
      return a->s;
    }
    if (idstation<a->s->id) {
      return rechercherStation(a->filsG, idstation);
    }
    else {
      return rechercherStation(a->filsD, idstation);
    }
  }
}

arbrestation ajouterStation (arbrestation a, station* s, int option) {
  if (a==NULL) {
    a=malloc(1*sizeof(noeudstation));
    a->s=NULL;
    a->s=s;
    a->filsG=NULL;
    a->filsD=NULL;
    return a;
  }

  switch (option) {
    //par hauteur
    case 4:
      if (s->altitude <= a->s->altitude) {
        a->filsG=ajouterStation(a->filsG,s, option);
      }
      else {
          if (s->altitude > a->s->altitude) {
              a->filsD=ajouterStation(a->filsD,s, option);
          }
          else {
            return a;
          }
      }
      break;

    //par humidite max
    case 5:
      if (s->humiditemax <= a->s->humiditemax) {
        a->filsG=ajouterStation(a->filsG,s, option);
      }
      else {
          if (s->humiditemax > a->s->humiditemax) {
              a->filsD=ajouterStation(a->filsD,s, option);
          }   
          else {
            return a;
          }
      }
      break;

    //par id
    default:
      if (s->id < a->s->id) {
        a->filsG=ajouterStation(a->filsG,s, option);
      }
      else {
          if (s->id > a->s->id) {
              a->filsD=ajouterStation(a->filsD,s, option);
          }   
          else {
            return a;
          }
      }
      break;

  }

  return a;

}



/*
Fonctions d'affichage pour vérifier les données stockées dans les structures
*/
void affichagereleve (releve* r) {
  printf("Date : %s\n", r->date);
  printf("Direction vent : %f\n", r->directionvent);
  printf("Vitesse vent : %f\n", r->vitessevent);
  printf("Humidité : %f\n", r->humidite);
  printf("Pression station : %f\n", r->pressionstation);
  printf("Variation pression : %f\n", r->variationpression);
  printf("Temperature : %f\n", r->temperature);
  printf("Temp min : %f\n", r->tempmin);
  printf("Temp max : %f\n", r->tempmax);
}
void affichagestation (station* s) {
  printf("-------------- Station %d | %d relevé(s) --------------\n", s->id, s->nbrreleves);
  printf("\n");
  printf("Coordonnées : %f %f\n", s->latitude, s->longitude);
  printf("Altitude : %d\n", s->altitude);
  printf("Humid Max : %f\n", s->humiditemax);
  printf("\n");

  /*
  for (int i = 0; i < s->nbrreleves; i++)
  {
    affichagereleve(s->tabreleves[i]);
    printf("\n");
  }
  printf("\n\n");
  */
}
void affichageA (arbrestation a) {
  if (a!=NULL){
    affichageA(a->filsG);
    affichagestation(a->s);
    affichageA(a->filsD);
  }
}



/*
Fonction Initialiser Station
Permet d'allouer la mémoire nécessaire pour un element de type station
*/
station* initialiserstation() {
    station* s=NULL;
    s=malloc(1*sizeof(station));
    //s->tabreleves=malloc(0*sizeof(releve*));
    s->tabreleves=malloc(40000*sizeof(releve*));
    s->nbrreleves=0;
    return s;
}

/*
Fonction free Arbre Station
Libère la mémoire d'un arbre de stations et de toutes les structures et données stockées à l'intérieur
*/
void freearbrestation(arbrestation a) {
  if (a!=NULL) {
    freearbrestation(a->filsG);
    freearbrestation(a->filsD);

    for (int i = 0; i < a->s->nbrreleves; i++) {
      free(a->s->tabreleves[i]->date);
      a->s->tabreleves[i]->date=NULL;
      free(a->s->tabreleves[i]);
      a->s->tabreleves[i]=NULL;
    }
    free(a->s->tabreleves);
    a->s->tabreleves=NULL;
    free(a->s);
    a->s=NULL;
    free(a);
    a=NULL;
  }
}

/*
Fonction qui permet d'ajouter un relevé météo au tableau de relevés de la station associée
On agrandi le tableau de relevés de la station s et on y ajoute le relevé
*/
releve** ajouterReleve (station* s, releve* rel) {
  s->nbrreleves++;
  //s->tabreleves=realloc(s->tabreleves, (s->nbrreleves)*sizeof(releve*));
  s->tabreleves[s->nbrreleves-1]=malloc(1*sizeof(releve));
  *(s->tabreleves[s->nbrreleves-1])=*rel;
  return s->tabreleves;
}







/* Fonction permetant de lire un fichier CSV */
arbrestation lireDonnees(char* fichier, int option, float ming, float maxg, float mina, float maxa, char* mind, char* maxd) {   

  FILE* donnees=NULL;
  char ligne[1000];
  donnees=fopen(fichier, "r");
  int colonne=2;
  arbrestation a=NULL;
  station* s=NULL;
  int ss=0;
  int sc=0;

  fgets(ligne,1000,donnees);

  if (fichier == NULL) {
    printf("Erreur ouverture fichier \n");
  }

  //Lecture du fichier de données
  else {
    while (fgets(ligne,300,donnees) !=NULL) {
      char* temp2=NULL;
      
      char* lignecopie=strdup(ligne);
      temp2=strsep(&lignecopie, ";");

      //on definit la station dans laquelle on va mettre les données lues : si la station existe cest donc elle qu'on va utiliser, si elle existe pas on la crée et l'ajoute à l'arbre
      s=rechercherStation(a, atoi(temp2));  
      if (s==NULL) {
        s=initialiserstation();
        s->id=atoi(temp2);
        ss=1;
        sc=1;
      }

      releve* r=NULL;
      r=malloc(1*sizeof(releve));

      while ((temp2=strsep(&lignecopie, ";"))) {
        switch (option) {  
          //temperature t
          case 1:
            switch (colonne) {
              case 2:
                r->date=strdup(temp2);
                break;
              case 11:
                r->temperature=atof(temp2);
                break;
              case 12:
                r->tempmin=atof(temp2);
                break;
              case 13:
                r->tempmax=atof(temp2);
                break;
            }
            break;

          //pression p
          case 2:
            switch (colonne) {
              case 2:
                r->date=strdup(temp2);
                break;
              case 7:
                r->pressionstation=atof(temp2);
                break;
              case 8:
                r->variationpression=atof(temp2);
                break;
            }
            break;

          //vent w
          case 3:
            switch (colonne) {
              case 2:
                r->date=strdup(temp2);
                break;
              case 4:
                r->directionvent=atof(temp2);
                break;
              case 5:
                r->vitessevent=atof(temp2);
                break;
            }
            break;

          //hauteur h
          case 4:
            switch (colonne) {
              case 2:
                r->date=strdup(temp2);
                break;
              case 14:
                s->altitude=atoi(temp2);
                break;
            }
            break;

          //humidité m
          case 5:
            switch (colonne) {
              case 2:
                r->date=strdup(temp2);
                break;
              case 6:
                r->humidite=atof(temp2);
                break;
            }
            break;
        }
        if ((colonne==10) && (sc)) {
          sscanf(temp2, "%f,%f", &s->latitude, &s->longitude);
          sc=0;
        }
        colonne++;
      }
      
      if (ss && filtrelongitude(s, ming, maxg) && filtrelatitude(s, mina, maxa)) {
        a=ajouterStation(a, s, 1);
        ss=0;
      }
      
      if (filtrelongitude(s, ming, maxg) && filtrelatitude(s, mina, maxa) && option!=4 && filtredate(r, mind, maxd)) {
        s->tabreleves=ajouterReleve(s,r);
      }
      
      colonne=2;

      /* FREE /!\ */
      free(r);
      r=NULL;
      free(temp2);
      temp2=NULL;
      free(lignecopie);
      lignecopie=NULL;
    }
    fclose(donnees);
  }
  return a;
}



        /* TEMPERATURE */

void addtemp(arbrestation a) {
  if (a!=NULL) {
    FILE* fichiertemp1=NULL;
    fichiertemp1=fopen("resultats/temperature1.csv", "a");
    float min=a->s->tabreleves[0]->temperature;
    float max=a->s->tabreleves[0]->temperature;
    float moyenne=0;
    for (int i = 0; i < a->s->nbrreleves; i++) {
      moyenne=moyenne+a->s->tabreleves[i]->temperature;
      if (a->s->tabreleves[i]->temperature>max) {
        max=a->s->tabreleves[i]->temperature;
      }
      if (a->s->tabreleves[i]->temperature<min) {
        min=a->s->tabreleves[i]->temperature;
      }
    
      if (a->s->tabreleves[i]->tempmax!=0 && a->s->tabreleves[i]->tempmax>max) {
        max=a->s->tabreleves[i]->tempmax;
      }
      if (a->s->tabreleves[i]->tempmin!=0 && a->s->tabreleves[i]->tempmin<min) {
        min=a->s->tabreleves[i]->tempmin;
      }
      
    }
    moyenne=moyenne/a->s->nbrreleves;
    
    fprintf(fichiertemp1, "%d %.2f %.2f %.2f\n", a->s->id, moyenne, min, max);
    fclose(fichiertemp1);
  }
}
void temperature1 (arbrestation a) {
  if (a!=NULL) {
    temperature1(a->filsG);
    addtemp(a);
    temperature1(a->filsD);
  }
}
void temperature (arbrestation a, int mode) {
  FILE* fichiertemp1=NULL;
  fichiertemp1=fopen("resultats/temperature1.csv", "w");
  //fprintf(fichiertemp1, "ID Station;Temperature Moyenne;Temperature Min;Temperature Max\n");
  fclose(fichiertemp1);
  temperature1(a);
}



      /* PRESSION */

void addpress(arbrestation a) {
  if (a!=NULL) {
    FILE* fichierpress1=NULL;
    fichierpress1=fopen("resultats/pression1.csv", "a");
    int j=0;
    while (a->s->tabreleves[j]->pressionstation==0)
    {
      j++;
    }
    float min=a->s->tabreleves[j]->pressionstation;
    float max=a->s->tabreleves[j]->pressionstation;
    float moyenne=0;
    int zero=0;
    for (int i = 0; i < a->s->nbrreleves; i++) {
      if (a->s->tabreleves[i]->pressionstation==0) {
        zero++;
      }
      moyenne=moyenne+a->s->tabreleves[i]->pressionstation;
      if (a->s->tabreleves[i]->pressionstation!=0 && a->s->tabreleves[i]->pressionstation>max) {
        max=a->s->tabreleves[i]->pressionstation;
      }
      if (a->s->tabreleves[i]->pressionstation!=0 && a->s->tabreleves[i]->pressionstation<min) {
        min=a->s->tabreleves[i]->pressionstation;
      }
      
    }
    moyenne=moyenne/(a->s->nbrreleves-zero);
    
    fprintf(fichierpress1, "%d %.2f %.0f %.0f\n", a->s->id, moyenne, min, max);
    fclose(fichierpress1);
  }
}
void pression1 (arbrestation a) {
  if (a!=NULL) {
    pression1(a->filsG);
    addpress(a);
    pression1(a->filsD);
  }
}
void pression (arbrestation a, int mode) {
  FILE* fichierpress1=NULL;
  fichierpress1=fopen("resultats/pression1.csv", "w");
  //fprintf(fichiertemp1, "ID Station;Pression Moyenne;Pression Min;Pression Max\n");
  fclose(fichierpress1);
  pression1(a);
}



        /* VENT */

void addvent(arbrestation a) {
  if (a!=NULL) {
    FILE* fichierv=NULL;
    fichierv=fopen("resultats/vent.csv", "a");
    float mvvent=0;
    float mdvent=0;
    float mxventvect=0;
    float myventvect=0;
    float xvect=0;
    float yvect=0;
    for (int i = 0; i < a->s->nbrreleves; i++) {
      mvvent=mvvent+a->s->tabreleves[i]->vitessevent;
      mdvent=mdvent+a->s->tabreleves[i]->directionvent;

      xvect=0;yvect=0;
      xvect=cos(a->s->tabreleves[i]->directionvent)*a->s->tabreleves[i]->vitessevent;
      yvect=sin(a->s->tabreleves[i]->directionvent)*a->s->tabreleves[i]->vitessevent;
      mxventvect=mxventvect+xvect;
      myventvect=myventvect+yvect;
    }
    mvvent=mvvent/a->s->nbrreleves;
    mdvent=mdvent/a->s->nbrreleves;
    mxventvect=mxventvect/a->s->nbrreleves;
    myventvect=myventvect/a->s->nbrreleves;
    //printf("VENT : %.2f %.2f", mxventvect, myventvect);
    fprintf(fichierv, "%d %f %f %.0f %.2f %.2f %.2f\n", a->s->id, a->s->longitude, a->s->latitude, mdvent, mvvent, mxventvect, myventvect);
    fclose(fichierv);
  }
}
void ventC (arbrestation a) {
  if (a!=NULL) {
    ventC(a->filsG);
    addvent(a);
    ventC(a->filsD);
  }
}
void vent (arbrestation a) {
  FILE* fichierv=NULL;
  fichierv=fopen("resultats/vent.csv", "w");
  //fprintf(fichierv, "ID Station;Longitude;Latitude;Direction du vent moyen 10 mn;Vitesse du vent moyen 10 mn;Moyenne Vecteur X;Moyenne Vecteur Y\n");
  fclose(fichierv);
  ventC(a);
}



        /* HAUTEUR */

arbrestation ahauteur (arbrestation a, arbrestation arbrehauteur) {
  if (a!=NULL) {
    arbrehauteur=ahauteur(a->filsG, arbrehauteur);
    arbrehauteur=ahauteur(a->filsD, arbrehauteur);

    arbrehauteur=ajouterStation(arbrehauteur, a->s, 4);
  }
  return arbrehauteur;
}
void addhauteur(arbrestation a) {
  if (a!=NULL) {
    FILE* fichierh=NULL;
    fichierh=fopen("resultats/hauteur.csv", "a");
    fprintf(fichierh, "%d %f %f %d\n", a->s->id, a->s->longitude, a->s->latitude, a->s->altitude);
    fclose(fichierh);
  }
}
void hauteurC (arbrestation a) {
  if (a!=NULL) {
    hauteurC(a->filsD);
    addhauteur(a);
    hauteurC(a->filsG);
  }
}
void hauteur (arbrestation a) {
  FILE* fichierh=NULL;
  fichierh=fopen("resultats/hauteur.csv", "w");
  //fprintf(fichierh, "ID Station;Longitude;Latitude;Hauteur\n");
  fclose(fichierh);
  arbrestation ahaut=NULL;
  ahaut=ahauteur(a,ahaut);
  hauteurC(ahaut);
}


        /* HUMIDITE */

arbrestation ahumidite (arbrestation a, arbrestation arbrehumidite) {
  if (a!=NULL) {
    arbrehumidite=ahumidite(a->filsG, arbrehumidite);
    arbrehumidite=ahumidite(a->filsD, arbrehumidite);
    float hmax=-100;
    for (int i = 0; i < a->s->nbrreleves; i++)
    {
      if (a->s->tabreleves[i]->humidite>hmax) {
        hmax=a->s->tabreleves[i]->humidite;
      }
    }
    a->s->humiditemax=hmax;
    arbrehumidite=ajouterStation(arbrehumidite, a->s, 5);
  }
  return arbrehumidite;
}
void addhumidite(arbrestation a) {
  if (a!=NULL) {
    FILE* fichierm=NULL;
    fichierm=fopen("resultats/humidite.csv", "a");
    fprintf(fichierm, "%d %f %f %.0f\n", a->s->id, a->s->longitude, a->s->latitude, a->s->humiditemax);
    fclose(fichierm);
  }
}
void humiditeC (arbrestation a) {
  if (a!=NULL) {
    humiditeC(a->filsD);
    addhumidite(a);
    humiditeC(a->filsG);
  }
}
void humidite (arbrestation a) {
  FILE* fichierm=NULL;
  fichierm=fopen("resultats/humidite.csv", "w");
  //fprintf(fichierm, "ID Station;Longitude;Latitude;Humidite Max\n");
  fclose(fichierm);
  arbrestation ahumid=NULL;
  ahumid=ahumidite(a,ahumid);
  humiditeC(ahumid);
}





    /* Optionelles (filtres g a d ) */

int filtrelongitude(station* s, float longmin, float longmax) {
  int res=1;
  if (!longmin && !longmax) {
    res=1;
  }
  else if((s->longitude<longmin) || (s->longitude>longmax)) {
    res=0;
  }
  //printf("filtre : %f %f\n", longmin, longmax);
  //printf("filtre longitude %d %f %d\n\n", s->id, s->longitude, res);
  return res;
}

int filtrelatitude(station* s, float latimin, float latimax) {
  int res=1;
  if (!latimin && !latimax) {
    res=1;
  }
  else if((s->latitude<latimin) || (s->latitude>latimax)) {
    res=0;
  }
  //printf("filtre : %f %f\n", latimin, latimax);
  //printf("filtre lattitude %d %f %d\n\n", s->id, s->latitude, res);
  return res;
}

int filtredate(releve* r, char* datemin, char* datemax) {
  int res=1;
  if (!strcmp(datemin, "0") && !strcmp(datemax, "0")) {
    res=1;
  }
  else if((comparaisondates(r->date,datemin)>1) || (comparaisondates(r->date,datemax)==1)) {
    res=0;
  }
  return res;
}

int comparaisondates(char* date1t, char* date2t){
  date1t=strsep(&date1t, "+");
  date2t=strsep(&date2t, "+");
  int annee1=0;
  int mois1=0;
  int jour1=0;
  int heure1=0;
  int minutes1=0;
  int secondes1=0;
  int annee2=0;
  int mois2=0;
  int jour2=0;
  int heure2=0;
  int minutes2=0;
  int secondes2=0;
  sscanf(date1t, "%d-%d-%dT%d:%d:%d", &annee1, &mois1, &jour1, &heure1, &minutes1, &secondes1);
  sscanf(date2t, "%d-%d-%dT%d:%d:%d", &annee2, &mois2, &jour2, &heure2, &minutes2, &secondes2);

  //comparaison des 2 dates / renvoie 1 si la première date est + gde et 2 si c'est la deuxième et 0 si égales
  if (annee1>annee2){
    return 1;
  }else if (annee2>annee1){
    return 2;
  }else{
    if (mois1>mois2){
      return 1;
    }else if (mois2>mois1){
      return 2;
    }else{
      if (jour1>jour2){
        return 1;
      }else if (jour2>jour1){
        return 2;
      }else{
        // comparaison des horaires
        if (heure1>heure2){
          return 1;
        }else if (heure2>heure1){
          return 2;
        }else{
          if (minutes1>minutes2){
            return 1;
          }else if (minutes2>minutes1){
            return 2;
          }else{
            if (secondes1>secondes2){
              return 1;
            }else if (secondes2>secondes1){
              return 2;
            }else{
              return 0;
            }
          }
        }
      }
    }
  }
  return 0;
}
