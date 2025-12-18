# Synchronisation Réseau et Fluidité (Netcode)

Ce document détaille les stratégies de synchronisation retenues pour assurer une expérience de jeu fluide et précise en mode FFA (Free-For-All) dans notre clone de Doom.

## 1. Client-Side Prediction (Prédiction côté client)

Pour éliminer la sensation de latence lors des déplacements du joueur local :
* **Concept** : Le client applique immédiatement les entrées (clavier/souris) à la position locale sans attendre la réponse du serveur.
* **Mise en œuvre** :
    * Le client maintient un numéro de séquence pour chaque paquet d'input envoyé au serveur.
    * Le client stocke ses prédictions dans un tampon (buffer) en attendant la confirmation du serveur.

## 2. Server Reconciliation (Réconciliation par le serveur)

Pour garantir que le serveur reste l'autorité finale sur l'état du jeu :
* **Concept** : Le serveur traite les inputs et renvoie la position "officielle" accompagnée du dernier numéro de séquence traité.
* **Mise en œuvre** :
    * Si la position renvoyée par le serveur diffère de la prédiction du client, ce dernier doit corriger sa trajectoire.
    * Pour éviter les téléportations brutales, une interpolation douce entre la position prédite et la position réelle est appliquée.

## 3. Entity Interpolation (Interpolation des entités)

Pour que les mouvements des autres joueurs soient fluides malgré la fréquence d'envoi des paquets (Tickrate) :
* **Concept** : Au lieu d'afficher les ennemis à leur dernière position connue, le client effectue une transition entre les deux derniers états reçus.
* **Mise en œuvre** :
    * Le client introduit un léger délai de rendu (ex: 100ms) pour toujours avoir deux points de données entre lesquels interpoler.
    * Cela compense les variations de latence (jitter) de l'UDP.

## 4. Lag Compensation (Compensation de latence)

Indispensable pour la précision des tirs dans un FPS :
* **Concept** : Lorsqu'un joueur tire, le serveur "remonte le temps" pour vérifier la position de la cible au moment précis où le tir a été effectué côté client.
* **Mise en œuvre** :
    * Le serveur garde un historique des positions de tous les joueurs sur les dernières 500ms.
    * Le paquet de tir envoyé par le client doit inclure un horodatage (Timestamp) précis.

## 5. Structure des Paquets (Protocole UDP)

Actuellement, le projet utilise des chaînes de caractères simples. Pour la synchronisation avancée, nous passerons à des structures binaires compactes :

### Format d'en-tête suggéré :
```cpp
struct NetHeader {
    uint32_t sequence;  // Pour ignorer les vieux paquets UDP
    uint32_t timestamp; // Temps de référence pour le "rewind" et l'interpolation
    uint8_t type;       // Type de message (INPUT, STATE, EVENT)
};
```

### Snapshot d'état du monde (Serveur -> Clients) :

Le serveur doit envoyer périodiquement (ex: 30 fois par seconde) l'état de tous les joueurs connectés dans la liste `std::vector<IPaddress> clients`.

## 6. Optimisations futures

* **Delta Encoding** : Envoyer uniquement les changements (ex: si un joueur n'a pas bougé, ne pas renvoyer sa position).
* **Snapshot Compression** : Compresser les données binaires pour réduire l'utilisation de la bande passante.

```

Ce fichier s'appuie sur l'architecture existante utilisant **SDL_net** et définit la feuille de route pour transformer le système actuel de messagerie basique en un moteur réseau de FPS performant.

```