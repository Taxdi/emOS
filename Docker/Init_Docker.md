# Prise en main des containers docker

Pour des raisons de simplicité, nous serons en root sur notre machine virtuelle.
Rappel: Pour passer en root:
```
su -
```

## 1. Installation de docker

Pour installer docker, nous pouvons copier coller les commandes ci-dessous dans un terminal sur notre VM :
```
curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh
```

Si curl n'est pas installé sur la machine : 
```
apt install curl
```

Nous pouvons ajouter le groupe docker aux utilisateurs pour éviter d'utiliser sudo.
```
sudo usermod -aG docker $USER
```

## 2. Premiers pas !

Nous allons executer certaines commandes pour comprendre le fonctionnement de docker.

```
docker run hello-world
```

Sur notre machine, le container **hello-world** n'est pas présent. Il faut donc qu'il soit "importé". C'est docker Daemon qui s'en occupe.

<img width="590" height="385" alt="docker-stm32" src="https://github.com/user-attachments/assets/6f277bae-cd7a-4585-911f-ef942918c12c" />

Ici, le Docker client se contact au Docker Daemon. Désormais Docker Daemon peut importer le container **hello-world** depuis le registry DockerHub. Ce dernier possède un dépôt complet en image.

Lien vers DockerHub : https://hub.docker.com/

```
docker run -it ubuntu bash
```

Dans cette commande, **-i** indique que nous utilisons l'entrée standard. Cela permet d'envoyer des commandes au container une fois qu'il est lancé. L'argument **-t** permet d'allouer un terminal **pseudo-TTY**, qui permet d'envoyer des commandes comme sur une vrai machine.

```
docker ps
```

Cette commande permet de lister les containers actif sur la machine.

```
docker ps -a
```

Cette commande permet de lister tous les containers de la machine (actifs ou arrêtés).

⚠️ **Les containers arrêtés ne sont pas supprimés.**

Il faut donc : 
```
docker rm *nom_container*
```

Essayons d'autres commandes : 

```
docker run -d --name=test ubuntu bash
```

Dans cette commande, l'argument **-d** permet de faire tourner un container en arrière plan (daemon). Cependant, ici nous lançons le "bash" de ubuntu mais aucune commande n'est effectuée. De ce fait, si nous lançons la commande :

```
docker ps
```

Le container que nous avons lancé ne sera pas actif. Pour qu'il soit actif il faut lancer une commande dans le shell ubuntu.

Exemple :

```
docker run -d --name=test2 ubuntu sh -c 'while true;do echo "Hello";sleep 1;done'
```

**-c** est un argument de sh qui permet de lancer la commande entre ''
Nous pouvons vérifier si le container est actif avec la commande

```
docker ps
```

Il est possible de connecter l'entrée, la sortie et les messages d'erreur standrard du terminal à un container en cours d'exécution à l'aide de son identifiant ou de son nom. Pour ce faire, il faut utiliser la commande : 

```
docker attach "nom_container"
```
