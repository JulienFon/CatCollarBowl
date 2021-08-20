#!/usr/bin/env bash

sudo apt update
sudo apt install -y wiringpi libwiringpi-dev libcurl4-openssl-dev

mkdir -p libs_src

curdir=$(pwd)

# téléchargement de la librairie cJSON
echo "téléchargement des sources de la librairie cJSON"
if [ ! -d libs_src/cJSON ]; then
    wget "https://github.com/DaveGamble/cJSON/archive/refs/tags/v1.7.13.zip" -O libs_src/cJSON.zip

    # dépaquetage
    unzip libs_src/cJSON.zip -d libs_src
    mv libs_src/cJSON-1.7.13 libs_src/cJSON

    rm -rf libs_src/cJSON.zip
else
    echo "source de la librairie cJSON présente"
fi

echo $curdir

# compilation de cJSON
echo "compilation et installation de cJSON"
if [ ! -d libs/cJSON ]; then
    mkdir -p libs/cJSON
    pushd libs_src/cJSON
        make all
        sudo make PREFIX=$curdir/libs/cJSON install
        sudo cp -a $curdir/libs/cJSON/lib/* /usr/lib
        sudo ldconfig -n -v /usr/lib
    popd
    echo "compilation de cJSON terminé"
else
    echo "cJSON déjà compilé"
fi

echo "Merci d'utiliser la commande make"