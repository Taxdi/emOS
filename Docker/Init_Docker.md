# Prise en main des conteneurs Docker

Pour des raisons de simplicité, nous serons en root sur notre machine virtuelle.
Rappel: Pour passer en root:
```
su -
```

## 1. Installation de Docker

Pour installer Docker, nous pouvons copier-coller les commandes ci-dessous dans un terminal sur notre VM :
```
curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh
```

Si curl n'est pas installé sur la machine : 
```
apt install curl
```

Nous pouvons ajouter le groupe Docker aux utilisateurs pour éviter d'utiliser sudo.
```
sudo usermod -aG docker $USER
```

## 2. Premiers pas !

Nous allons exécuter certaines commandes pour comprendre le fonctionnement de Docker.

```
docker run hello-world
```

Sur notre machine, le conteneur **hello-world** n'est pas présent. Il faut donc qu'il soit "importé". C'est Docker Daemon qui s'en occupe.

<img width="590" height="385" alt="docker-stm32" src="https://github.com/user-attachments/assets/6f277bae-cd7a-4585-911f-ef942918c12c" />

Ici, le Docker client se connecte au Docker Daemon. Désormais Docker Daemon peut importer le conteneur **hello-world** depuis le registre DockerHub. Ce dernier possède un dépôt complet en image.

Lien vers DockerHub : https://hub.docker.com/

```
docker run -it ubuntu bash
```

Dans cette commande, **-i** indique que nous utilisons l'entrée standard. Cela permet d'envoyer des commandes au conteneur une fois qu'il est lancé. L'argument **-t** permet d'allouer un terminal **pseudo-TTY**, qui permet d'envoyer des commandes comme sur une vraie machine.

```
docker ps
```

Cette commande permet de lister les conteneurs actifs sur la machine.

```
docker ps -a
```

Cette commande permet de lister tous les conteneurs de la machine (actifs ou arrêtés).

⚠️ **Les conteneurs arrêtés ne sont pas supprimés.**

Il faut donc : 
```
docker rm *nom_conteneur*
```

Essayons d'autres commandes : 

```
docker run -d --name=test ubuntu bash
```

Dans cette commande, l'argument **-d** permet de faire tourner un conteneur en arrière-plan (daemon). Cependant, ici nous lançons le "bash" de Ubuntu mais aucune commande n'est effectuée. De ce fait, si nous lançons la commande :

```
docker ps
```

Le conteneur que nous avons lancé ne sera pas actif. Pour qu'il soit actif, il faut lancer une commande dans le shell Ubuntu.

Exemple :

```
docker run -d --name=test2 ubuntu sh -c 'while true;do echo "Hello";sleep 1;done'
```

**-c** est un argument de sh qui permet de lancer la commande entre ''
Nous pouvons vérifier si le conteneur est actif avec la commande

```
docker ps
```

Il est possible de connecter l'entrée, la sortie et les messages d'erreur standard du terminal à un conteneur en cours d'exécution à l'aide de son identifiant ou de son nom. Pour ce faire, il faut utiliser la commande : 

```
docker attach "nom_conteneur"
```
