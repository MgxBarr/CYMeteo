# Projet Info3 P2 S1

# Projet CYMeteo

Application permettant de générer des graphiques météo à partir d'un fichier de données trié par un programme C et à l'aide d'un script.


# Utilisation

Télécharger les fichiers de la branche main. <br />
Ajouter les droits d'éxecution chmod 700 -R swifties <br />
Executer le script "./script.sh". <br />
Pour afficher l'aide, entrez "./script.sh --help". <br />
<br />
A chaque appel, le script appelle le programme C pour générer les graphiques en fonction des options choisies.<br />
<br />
# Exemples de commandes : <br />

Help : <br />
./script.sh --help 
<br /><br />
Tout le fichier d'entrée : <br />
./script.sh -i meteo_filtered_data_v1.csv 
<br /><br />
Filtre géographique : <br />
./script.sh -i meteo_filtered_data_v1.csv -F -o France.csv
<br /><br />
Filtre date : <br />
./script.sh -i meteo_filtered_data_v1.csv -o date20102012.csv -d "2010-01-01T00:00:00+01:00" "2012-01-01T00:00:00+01:00"
<br /><br />
Filtre lattitude : <br />
./script.sh -i meteo_filtered_data_v1.csv -o lattitude1070.csv -a 10 70
<br /><br />
Filtre longitude : <br />
./script.sh -i meteo_filtered_data_v1.csv -g -50 50 -o longitude-5050.csv
<br /><br />
Toutes les options : <br />
./script.sh -F -i meteo_filtered_data_v1.csv -a 10 70 -g -50 50 -d "2010-01-01T00:00:00" "2012-01-01T00:00:00" -o BAGUET.csv
