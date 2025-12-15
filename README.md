# Projet IN204

## Descriptif du projet

### Overview

Le but du projet est de faire un clone du jeu Doom (1993). Ce jeu etait tres innovant pour l'epoque et beaucoup
d'innovations qu'il a apporte sont utilises dans les jeu modernes.
C'est un jeu en 2.5D (cf paragraphe suivant) qui suit l'histoire d'un soldat qui se bat contre des monstres dans un bunker.
Sa pertinence pour le cours de in204 est qu'il permettra par sa multitude de features d'utiliser pleinement les tenants de la
programation orientee objet (OOP).

### Plan du projet

le projet se decoup en 4 parties largement independantes.
le jeu, le rendu graphique, le network et le controleur de jeu.
Ces quatres parties permettront d'avoir un jeu complet, bien decoupe en fonctions unitaires et separees.

_Le Jeu_ : Les calculs du Gamesense seront fait cote serveur et ceux-ci seront la fondation du jeu

_Le rendu Graphique_ : Le jeu est en 2.5D, c'est a dire q'il est equivalent a un jeu 2D, mais le renderer permet d'avoir une visualisation 3D pour le client

#### todo

_Le network_

_Le Controleur de jeu_

## Installation des dépendances (SDL2)

Ce projet nécessite la bibliothèque SDL2 pour fonctionner.

### Sous Debian / Ubuntu / Linux Mint

Ouvrez un terminal et exécutez les commandes suivantes pour mettre à jour votre système et installer les paquets requis :

```bash
sudo apt update

sudo apt install -y gcc make git \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-net-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev \
    libsdl2-gfx-dev
```

### Sous Arch Linux

Ouvrez un terminal et exécutez les commandes suivantes pour mettre à jour votre système et installer les paquets requis :

```bash
sudo pacman -Syu
sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf
```
