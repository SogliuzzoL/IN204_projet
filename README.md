# Projet IN204

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
sudo pacman -S --needed gcc make git \
    sdl2 \
    sdl2_image \
    sdl2_net \
    sdl2_mixer \
    sdl2_ttf \
    sdl2_gfx
```