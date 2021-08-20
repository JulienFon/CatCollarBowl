-----------------
# CATCOLLARBOWL
By Fabien DUTERTE & Julien FONSON
-----------------

## Partie "Gamelle" (RPI)

Pour l'installation des librairies, lancez le script `requirements.sh`
Exécutez la commande `make`
Pour lancer l'application exécutez `./src/main.o`
(il est nécessaire d'être sur une RPI sinon vous aurez un message d'erreur causé par WiringPi)

## Partie "Collier" (Arduino MKR1400)

Veuillez installer l'IDE Arduino https://www.arduino.cc/en/software
Tools > Board > Boards Manager > Rechercher et installer “Arduino SAMD”
Tools > Board > Arduino SAMD > Sélectionner Arduino MKR GSM 1400
Sketch > Include Library > Manage Libraries > Rechercher et installer Arduino Low Power > Rechercher et installer MKRGSM
Brancher l'Arduino MKR1400, Tools > Port > sélectionner le port COM correspondant
Téléverser `collar.ino`

-----------------

Copier - Coller `secrets.h.example`, le renommer en `secrets.h` et remplir les champs nécessaires

# Documentations

moteur pas à pas : https://www.gotronic.fr/pj2-sbc-moto1-fr-1519.pdf
api SMS factor : https://dev.smsfactor.com/en/api/example/c

buzzer : https://wiki.seeedstudio.com/Grove-Buzzer/
documentation GTK 3 : https://devdocs.io/gtk~3.22/
documentation MKR 1400 : https://www.arduino.cc/en/Reference/MKRGSM
