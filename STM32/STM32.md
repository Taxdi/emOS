# STM32
## 1. Prérequis

Pour réaliser cette partie, nous allons avoir besoin des outils suivants : 
  - Docker
  - Socat
    - Utile pour connecter des applications à l'intérieur de boîtes séparées
  - ssh ( pour faciliter l'utilisation de la VM
      - Permet de faire des copier-coller plus facilement
  - GIT ( qui va nous permettre d'utiliser les ressources de ce TP )

Nous allons commencer par cloner le dépôt git, sur lequel les ressources pour le TP sont présentes, avec la commande : 
```
git clone https://forgens.univ-ubs.fr/gitlab/charton/virtos.git
```
Enfin, nous allons aller dans le dossier **/virtos/Ressource_OS_STM32/docker** pour construire le stm32 (voir Buil.md).

## 2. Ajout de fonctionnalités

### 2.a Interruption

Notre STM32 ne contient pas de système d'interruption. Il faut donc le créer et l'intégrer à nos différentes fonctions
