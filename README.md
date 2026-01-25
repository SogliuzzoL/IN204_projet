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

_Le rendu Graphique_ : Le jeu est en 2.5D, c'est a dire q'il est equivalent a un jeu 2D, mais le renderer permet d'avoir une visualisation 3D pour le client. Il met a jour ses donnes de jeu a partir du serveur. 

_Le network_ : La communication entre le client et le serveur sera faite par des sockets UDP. Le serveur enverra les donnees de jeu au client et recevra les inputs du client.

_Le controleur de jeu_ : Il gere les inputs du joueur et les envoie au serveur.

## Structure du code

### Racine
- **Makefile**: règles de compilation (`make`, `make run`, `make server`, `make clean`).
- **Assets/**: ressources (images, sons, etc.).
- **bin/**: exécutable généré (`bin/game`).
- **build/**: fichiers objets intermédiaires.
- **include/**: en-têtes C++ (interfaces et types partagés).
- **src/**: implémentations C++.

### Moteur (Engine)
- **include/Engine/** et **src/Engine/**:
    - **GameWindow**: création de la fenêtre SDL + contexte OpenGL, boucle principale, timing.
    - **CameraRendering**: rendu 2.5D/GL, caméra et pipeline de rendu.
    - **Controls**: gestion des entrées clavier/souris et conversion en états de jeu.
    - **Maze**: génération et représentation du labyrinthe (grille, connexions, origine, seed).
    - **Menu**: interfaces et interactions du menu.

### Réseau (Network)
- **include/Network/** et **src/Network/**:
    - **Network**, **NetworkClient**, **NetworkServer**: communication UDP via SDL_net, envoi/réception des paquets.
    - **Protocol**: types de paquets et formats (`InputPacket`, `WelcomePacket`, `SeedPacket`, `WorldStatePacket`).
    - **PacketFactory**: création/sérialisation des paquets.
    - **PacketHandler**: traitement des paquets entrants et mise à jour de l’état du monde.

### Entrées / sorties du programme
- **src/main.cpp**: point d’entrée; supporte `--server` pour démarrer le serveur.
- **src/client.cpp**, **src/server.cpp**: logique côté client/serveur.

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
    libsdl2-gfx-dev \
    libglu1-mesa-dev
```

### Sous Arch Linux

Ouvrez un terminal et exécutez les commandes suivantes pour mettre à jour votre système et installer les paquets requis :

```bash
sudo pacman -Syu
sudo pacman -S sdl2 sdl2_image sdl2_net sdl2_mixer sdl2_ttf sdl2_gfx glu
```

## Compilation et exécution

### Compilation

```bash
make
```

### Lancer le jeu (client)

```bash
make run
# ou directement
./bin/game
```

### Lancer le serveur

```bash
make server
# ou directement
./bin/game --server
```

