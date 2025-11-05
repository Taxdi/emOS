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

---

## 2. Ajout de fonctionnalités

### 2.1 Interruption
Notre STM32 ne contient pas de système d’interruption logiciel équivalent à celui que l’on trouve sur un système d’exploitation classique (comme le signal `SIGINT` sous Linux).
Nous allons donc créer notre propre mécanisme d’interruption globale, qui permettra d’interrompre n’importe quel programme en cours d’exécution à l’aide d’une combinaison clavier, comme `Ctrl + C`

---

#### 2.1.1 Objectifs

- Simuler un comportement similaire à `Ctrl+C` sur un terminal Linux.  
- Pouvoir interrompre **n’importe quel programme** (kernel ou user).  
- Centraliser cette gestion dans un **module unique** : `interrupt_handler`.  
- Offrir un comportement cohérent et reproductible dans tout le système.

---

#### 2.1.2 Principe de fonctionnement

Lorsqu’on tape `Ctrl + C` dans le terminal série (via `socat`), le caractère ASCII `3` est envoyé au STM32.

Une fonction `checkInterrupt()` est chargée de :
1. Lire les caractères présents sur le port série.
2. Détecter le code `3` (correspondant à `Ctrl+C`).
3. Déclencher un **flag global** `interrupted = true`.

Tous les programmes du système peuvent ensuite vérifier ce flag pour s’arrêter proprement.

---

#### 2.1.3 Structure des fichiers

include/
└── interrupt_handler.h

src/
└── interrupt_handler.cpp

#### 2.1.4 Schéma logique du fonctionnement

         ┌──────────────┐
         │  Terminal    │
         │ (Ctrl + C)   │
         └──────┬───────┘
                │
        ASCII 3 via Serial
                │
     ┌──────────▼──────────┐
     │ checkInterrupt()    │
     │ détecte ASCII = 3   │
     │ interrupted = true  │
     └──────────┬──────────┘
                │
     ┌──────────▼──────────┐
     │ Boucles Kernel/User │
     │ if(interrupted)     │
     │   arrêt propre      │
     └─────────────────────┘



