# Projet CYMeteo
## Project C P2 S1


Application for generating weather graphs from a data file sorted by a C program and a script.


# Use

Download files from the main branch. <br />
Add execution rights chmod 700 -R swifties <br />
Run the script "./script.sh". <br />
To display help, enter "./script.sh --help". <br />
<br />
With each call, the script calls the C program to generate the graphics according to the options chosen.<br />
<br />
# Examples of prompt : <br />

Help : <br />
./script.sh --help 
<br /><br />
All input files : <br />
./script.sh -i meteo_filtered_data_v1.csv 
<br /><br />
Geographic filter : <br />
./script.sh -i meteo_filtered_data_v1.csv -F -o France.csv
<br /><br />
Date filter : <br />
./script.sh -i meteo_filtered_data_v1.csv -o date20102012.csv -d "2010-01-01T00:00:00+01:00" "2012-01-01T00:00:00+01:00"
<br /><br />
Latitude filter : <br />
./script.sh -i meteo_filtered_data_v1.csv -o lattitude1070.csv -a 10 70
<br /><br />
Longitude filter : <br />
./script.sh -i meteo_filtered_data_v1.csv -g -50 50 -o longitude-5050.csv
<br /><br />
All options : <br />
./script.sh -F -i meteo_filtered_data_v1.csv -a 10 70 -g -50 50 -d "2010-01-01T00:00:00" "2012-01-01T00:00:00" -o BAGUET.csv
