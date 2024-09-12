#!/bin/bash



  #début mesure temps d'execution du script

debut=`date +%s.%N | cut -b1-12 `



  #executable

estex=0
for i in ./*
do
  if [ -x $i ]  && [[ ! "$i" =~ ^.*.sh$ ]] && [ ! -d $i ] 
  then
    executable=$i
    estex=1
  fi
done
if [ $estex -eq 0 ]
then
  echo `make clean`
  echo `make`
  for i in ./*
  do
    if [ -x $i ]  && [[ ! "$i" =~ ^.*.sh$ ]] && [ ! -d $i ] 
    then
      executable=$i
      estex=1
    fi
  done
fi

if [ $estex -eq 0 ]
then
  echo "ERREUR : La compilation n'a pas marchée, veuillez corriger vos fichiers et réessayer"
  exit 1
fi



    #Help

for i in $(seq 1 $#)
do
  if [ "${!i}" == "--help" ]
  then
    echo ""
    echo "Executez le script via la commande ./script.sh"
    echo "Options :"
    echo ""
    echo "-F : (F)rance : permet de limiter les mesures à celles présentes en France métropolitaine + Corse."
    echo "-G : (G)uyane : permet de limiter les mesures à celles qui sont présentes en Guyane."
    echo "-S : (S)aint-Pierre et Miquelon : permet de limiter les mesures à celles qui sont présentes sur l'ile située à l'Est du Canada."
    echo "-A : (A)ntilles : permet de limiter les mesures à celles qui sont présentes aux Antilles."
    echo "-O : (O)céan indien : permet de limiter les mesures à celles qui sont présentes dans l'océan indien."
    echo "-Q : antarcti(Q)ue permet de limiter les mesures à celles qui sont présentes en antarctique."
    echo ""
    echo "-g <min><max> : Permet de filtrer les données de sortie en ne gardant que les données qui sont dans l'intervalle de longitudes [<min>..<max>] incluses."
    echo "-a <min><max> : Permet de filtrer les données de sortie en ne gardant que les relevés qui sont dans l'intervalle de latitudes [<min>..<max>] incluses."
    echo "-d <min><max> : permet de filtrer les dates entre les valeurs min et max. (de forme : AAAA-MM-JJTHH:MM:SS)"
    echo "-i <nom_fichier> : Permet de spécifier le chemin du fichier CSV d'entrée (obligatoire)."
    echo "-o <nom_fichier> : Permet de donner un nom au fichier de sortie contenant les données."
    echo ""
    echo "Pour obtenir l'aide du programme C, compilez et tappez ./cymeteo -H"
    echo ""
    exit 1
  fi
done



 #Creation des dossiers données et resultats

res=0
don=0
for i in ./*
do
 if [ -d "resultats" ]
 then
   res=1
 fi
 if [ -d "donnees" ]
 then
   don=1
 fi
done
if [ $res -eq 0 ]
then
 echo `mkdir resultats`
fi
if [ $don -eq 0 ]
then
 echo `mkdir donnees`
fi



 #Check option exclusive

param=0
filtre="all"
for i in $(seq 1 $#)
do
 if [ $param -gt 1 ]
 then
   echo "ERREUR : Veuillez entrer une seule option géographique"
   exit 1
 fi
 if [ "${!i}" == "-F" ] || [ "${!i}" == "-G" ] || [ "${!i}" == "-S" ] || [ "${!i}" == "-A" ] || [ "${!i}" == "-O" ] || [ "${!i}" == "-Q" ]
 then
   param=$((param+1))
   filtre=${!i}
 fi
done



  #getopt pour les arguments 

ming=0
maxg=0
mina=0
maxa=0
mind="0"
maxd="0"
fichierentree="0"
fichiersortie="meteodonnees.csv"
clongitudes=0; 
clatitudes=0; 
cdates=0; 
while getopts ":g: :a: :d: i:o:" opt; do
  OPTIND=1
  case "${opt}" in 
    g)
      shift
      ming=$1
      shift
      maxg=$1
      shift
      clongitudes=$((clongitudes+1))
      ;;
    a)
      shift
      mina=$1
      shift
      maxa=$1
      shift
      clatitudes=$((clatitudes+1))
      ;;
    d)
      shift
      mind=$1
      shift
      maxd=$1
      shift
      cdates=$((cdates+1))
      ;;
    i)
      fichierentree=$OPTARG
      shift
      shift
      ;;
    o)
      fichiersortie=$OPTARG
      shift
      shift
      ;;
    *)
      shift
      ;;
  esac
done

<<com
echo "executable $executable"
echo "filtre region $filtre"
echo "fe $fichierentree fs $fichiersortie"
echo "ming $ming maxg $maxg"
echo "mina $mina maxg $maxa"
echo "mind $mind maxd $maxd"
com



  #Codes d'erreurs

if [ $fichierentree == "0" ]
  then 
    echo "Erreur : Veuillez spécifier un fichier d'entrée"
    exit 1
fi

if [ $clatitudes -gt 1 ] || [ $clongitudes -gt 1 ] || [ $cdates -gt 1 ]
then
  echo "Erreur : Veuillez appeler une seule fois les options de filtrage."
  exit 1
fi



  #Création d'un fichier de données temporaire contenant seulement les données associées au paramètre géographique exclusif
if [ $filtre = 'all' ]
  then
    cp $fichierentree donnees/$fichiersortie
fi
if [ $filtre = '-F' ]
  then
    grep '^07' $fichierentree  > donnees/$fichiersortie
fi
if [ $filtre = '-G' ]
  then
    grep '^81' $fichierentree > donnees/$fichiersortie
fi
if [ $filtre = '-S' ]
  then
    grep '^71805' $fichierentree  > donnees/$fichiersortie
fi
if [ $filtre = '-A' ]
  then
    grep '^78' $fichierentree  > donnees/$fichiersortie
fi
if [ $filtre = '-O' ]
  then
    grep '^6' $fichierentree  > donnees/$fichiersortie
fi
if [ $filtre = '-Q' ]
  then
    grep '^89' $fichierentree  > donnees/$fichiersortie
fi



  #Execution du programme
echo `make clean`
echo `make`
echo `./cymeteo -i donnees/$fichiersortie -o $fichiersortie -g $ming $maxg -a $mina $maxa -d "$mind" "$maxd" -t 1`
echo `./cymeteo -i donnees/$fichiersortie -o $fichiersortie -g $ming $maxg -a $mina $maxa -d "$mind" "$maxd" -p 1`
echo `./cymeteo -i donnees/$fichiersortie -o $fichiersortie -g $ming $maxg -a $mina $maxa -d "$mind" "$maxd" -w`
echo `./cymeteo -i donnees/$fichiersortie -o $fichiersortie -g $ming $maxg -a $mina $maxa -d "$mind" "$maxd" -h`
echo `./cymeteo -i donnees/$fichiersortie -o $fichiersortie -g $ming $maxg -a $mina $maxa -d "$mind" "$maxd" -m`



  #Suprression des fichiers temporaires
#rm -r donnees



  #GNUPLOT

gnuplot -e "
  set title 'Température Mode 1';
  set xlabel 'ID Station';
  set ylabel 'Températures';
  set autoscale;
  set offset graph 0,0,0.10,0.10;
  set terminal 'png';
  set output 'resultats/temperature1';
  plot 'resultats/temperature1.csv' u 1:2:3:4 t '' w yerr;
"


gnuplot -e "
  set title 'Pression Mode 1';
  set xlabel 'ID Station';
  set ylabel 'Pressions';
  set yrange [ 0 : 120000 ] ;
  set offset graph 0,0,0.10,0.10;
  set terminal pngcairo;
  set output 'resultats/pression1';
  plot 'resultats/pression1.csv' u 1:2:3:4 t '' w yerr;
"

gnuplot -e "
  set title 'Vent';
  set xlabel 'Longitude';
  set ylabel 'Latitude';
  set autoscale;
  set offset graph 0.10,0.10;
  set terminal pngcairo;
  set output 'resultats/vent';
  plot 'resultats/vent.csv' using 2:3:6:7 t '' with vectors head filled lt 1;
"

gnuplot -e "
  set border 4095;
  set title 'Carte interpolée des hauteurs';
  set xlabel 'Longitude';
  set ylabel 'Latitude';
  set zlabel 'Hauteur';
  set autoscale;
  set palette rgb 33,13,10;
  set dgrid3d;
  set dgrid3d 100,100,10;
  set pm3d corners2color c2;
  set terminal pngcairo;
  set output 'resultats/hauteur';
  splot 'resultats/hauteur.csv' u 2:3:4 t '' with pm3d;
"

gnuplot -e "
  set border 4095;
  set title 'Carte interpolée des humidités max';
  set xlabel 'Longitude';
  set ylabel 'Latitude';
  set zlabel 'Humidité Max';
  set autoscale;
  set palette rgb 33,13,10;
  set dgrid3d;
  set dgrid3d 100,100,10;
  set pm3d corners2color c2;
  set terminal pngcairo;
  set output 'resultats/humidite';
  splot 'resultats/humidite.csv' u 2:3:4 t '' with pm3d;
"



  #Ouverture des graphs
xdg-open resultats/temperature1
xdg-open resultats/pression1
xdg-open resultats/vent
xdg-open resultats/hauteur
xdg-open resultats/humidite



  #Temps d'éxecution du script 
temps=$(echo "$(date +%s.%N | cut -b1-12 ) - $debut" | bc | sed 's/^\./0./' )
echo "Temps d'execution du script: $temps s"



  #Le script s'est bien executé
exit 0
