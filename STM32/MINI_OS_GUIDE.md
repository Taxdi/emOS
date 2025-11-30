# Mini OS pour STM32 - Guide d'utilisation

## Vue d'ensemble

Ce mini OS implémente les 3 tâches requises :

### 1. **Mécanisme d'interruption**
- Les programmes qui tournent indéfiniment (ex: `infiniteLoop`) peuvent être arrêtés via la commande `kill <pid>`
- Un processus interrompu passe à l'état `ZOMBIE` et est nettoyé automatiquement

### 2. **Gestion multi-processus**
- Shell interactif avec commandes pour créer, lister, suspendre et terminer les processus
- Chaque processus a un PID unique et un état (RUNNING, SUSPENDED, ZOMBIE)
- Possibilité de basculer entre processus sans "tâche de fond" (mode mono-contexte)

### 3. **Ordonnanceur Round-Robin**
- Implémentation d'un scheduler qui exécute les processus en alternance
- Chaque processus obtient une "tranche de temps" (timeslice) avant de passer au suivant
- Les processus suspendus sont ignorés par le scheduler
- Affichage du temps CPU consommé par chaque processus

---

## Architecture

### Composants principaux

#### `ProcessManager` (process_manager.h/cpp)
- Gère la table des processus (max 8 processus simultanément)
- États des processus : EMPTY, RUNNING, SUSPENDED, ZOMBIE
- Méthodes :
  - `createProcess()` : créer un nouveau processus
  - `getProcess()` : récupérer un processus par PID
  - `killProcess()` : terminer un processus
  # Mini OS — Guide d'utilisation (version révisée)

  **But**

  Fournir un mini noyau pédagogique pour STM32 (QEMU) permettant :
  - d'exécuter plusieurs programmes utilisateur,
  - d'arrêter les programmes bloquants (Ctrl+C ou `kill <pid>`),
  - d'ordonnancer les processus (Round-Robin coopératif).

  Ce guide explique l'utilisation et les scénarios de test rapides.

  **Prérequis**

  - Compiler le projet avec PlatformIO depuis le dossier `OS-FunctionPrograms` :
    ```bash
    pio run
    ```
  - Connectez-vous à la console série (115200) pour accéder au shell.

  **Commandes principales du shell**

  - `list`        : afficher la liste des programmes disponibles (ID).
  - `ps`          : afficher les processus actifs (PID, nom, état, CPU ms).
  - `launch <id>` : lancer un programme en mode bloquant (mono-tâche).
  - `start <id>`  : créer un processus (ajouté à la table et marqué RUNNING).
  - `kill <pid>`  : demander l'arrêt coopératif d'un processus (marque ZOMBIE).
  - `suspend <pid>` / `resume <pid>` : pause / reprise d'un processus.
  - `run`         : lancer l'ordonnanceur Round-Robin (les processus RUNNING sont alternés).
  - `help`        : afficher l'aide.
  - `quit`        : arrêter le système.

  Note : Ctrl+C (ASCII 3) envoyé depuis le terminal demande un arrêt global coopératif (équivalent à `kill` général).

  **Scénarios rapides**

  - Lancer deux processus puis exécuter le scheduler :
    1. `start 1` (prog2)
    2. `start 7` (infiniteLoop)
    3. `run`

    `prog2` sera exécuté et finira, `infiniteLoop` tourne en arrière-plan. Appuyez Ctrl+C dans le terminal pour demander l'arrêt.

  - Arrêter un processus bloquant :
    1. `start 7` (crée infiniteLoop)
    2. `ps` → noter le PID
    3. `kill <pid>` → le processus lira le flag et sortira proprement

  **Comportement du scheduler**

  Le scheduler est coopératif : les programmes doivent appeler `kernel_yield()` régulièrement (ou utiliser `delay`) pour permettre au noyau de lire Ctrl+C et d'effectuer des opérations. `infiniteLoop` inclut désormais `kernel_yield()`.

  **Limitations**

  - Pas de vraie préemption : le noyau ne sauvegarde pas le contexte matériel d'un programme. Pour une préemption complète il faudrait : SysTick + context switch + piles séparées.
  - Pas d'isolation mémoire entre processus.
