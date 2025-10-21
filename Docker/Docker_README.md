# 1. Qu'est-ce que docker ?

<img width="225" height="225" alt="Docker_Logo-removebg-preview" src="https://github.com/user-attachments/assets/4edc36ed-bf62-45d9-9cf1-7a67bf666eb4" /> 

Docker est un logiciel (open source) de conteneurisation qui permet la création et l'utilisation de conteneurs linux. Ce logiciel utilise le noyau linux ainsi que ses fonctionnalités pour séparer des processus afin qu'ils s'exécutent de manière indépendante. Cette indépendnace reflète l'objectif des conteneurs: exécuter plusieurs processus et applications séparément les uns des autres afin d'optimiser l'utilisation de l'infrastructure hôte.

## 2. Fonctionnement

### 2.1 Conteneurisation

Un conteneur est le regroupement d’un logiciel et toutes ses dépendances (librairies, code source, fichiers de configuration…) empaquetés au même endroit. Ce qui permet d’assurer le fonctionnement stable de l’application quelque soit les environnements d’exécutions dans lesquels elle est lancée. Les conteneurs sont des machines virtuelles améliorées, en effet à la différence d’une machine virtuelle classique, un conteneur virtualise un système d’exploitation et réutilise le noyau de l’OS de la machine hôte. Ce qui permet une gestion des ressources optimisée.

<img width="700" height="559" alt="docker_fonct" src="https://github.com/user-attachments/assets/de6cbb52-c656-436c-8d6c-e3fa9cdaa292" />

# Docker Compose

<img width="936" height="450" alt="docker_compose" src="https://github.com/user-attachments/assets/0fc7758b-ff03-4720-99ff-9b4f1dbcd2ed" />


Docker Compose est un outil permettant d'orchestrer plusieurs applications exécutées dans des conteneurs Docker. Cela permet notamment d'écrire l'ensemble des paramètres et dépendances des applications, ainsi que la manière dont elles interagissent entre-elles dans un fichier YAML appelé docker-compose.yml
