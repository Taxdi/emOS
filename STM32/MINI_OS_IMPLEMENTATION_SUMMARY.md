```


# Mini OS — Résumé d'implémentation (version révisée)

**Objectif**

Ce document résume l'architecture et les choix d'implémentation du mini OS (STM32/QEMU).

**Composants principaux**

- `ProcessManager` (`include/process_manager.h`, `src/process_manager.cpp`)
  - Table de processus (max 8). Chaque entrée contient : PID, nom, état, pointeur vers la fonction utilisateur, timestamps, compteur CPU.
  - États : `PS_EMPTY`, `PS_RUNNING`, `PS_SUSPENDED`, `PS_ZOMBIE`.
  - API : `createProcess()`, `getProcess()`, `killProcess()`, `suspendProcess()`, `resumeProcess()`, `getNextRunnable()`, `listProcesses()`, `cleanupZombies()`.

- `Scheduler` (`include/scheduler.h`, `src/scheduler.cpp`)
  - Ordonnancement Round-Robin coopératif.
  - `kernel_yield()` exposé aux programmes pour leur permettre de donner la main au kernel (détecte Ctrl+C et met `kernel_stop_requested`).
  - `run()` lance la boucle d'ordonnancement : poll série, tick, cleanup.

- `main.cpp` (shell)
  - Shell interactif simple pour manipuler la table de processus et démarrer l'ordonnanceur.

**Comportement clé**

- Les programmes utilisateur respectent une interface simple : `void prog(void (*print_fn)(const char*))`.
- Le scheduler est coopératif : les programmes doivent appeler `kernel_yield()` ou utiliser `delay()` pour permettre au kernel de vérifier les entrées (Ctrl+C) et effectuer des actions.

**Comment le Round-Robin est réalisé**

1. `ProcessManager::getNextRunnable()` parcourt la table (depuis `last_scheduled_index + 1`) et renvoie le PID du prochain processus en `PS_RUNNING`.
2. Le scheduler appelle la fonction utilisateur associée ; le programme s'exécute et fait des yields périodiques.
3. À la sortie, le scheduler met à jour les compteurs de temps CPU et nettoie les zombies.

**Interruption coopérative**

- `kernel_stop_requested` (défini dans `scheduler.cpp`) est un flag lu par les programmes.
- `kill(pid)` dans `ProcessManager` marque le processus en `PS_ZOMBIE` et déclenche le flag d'arrêt coopératif si nécessaire.
- `kernel_yield()` permet de détecter Ctrl+C (ASCII 3) envoyé sur la console et de mettre `kernel_stop_requested = true`.

**Limitations et points d'amélioration**

- Pas de préemption matérielle : pour une vraie préemption il faut SysTick + context switch + sauvegarde des registres et piles par processus.
- Chaque processus partage la même adresse mémoire (pas d'isolation) — risque d'interférences.
- Les programmes doivent être coopératifs (appeler `kernel_yield()`).

Améliorations futures :

- Préemption matérielle (SysTick) + `setjmp()`/`longjmp()` pour sauvegarder le contexte.
- Allouer une pile dédiée par processus et implémenter la commutation de contexte.
- Ajouter IPC simple (queues, signaux) et gestion d'erreurs plus robuste.

**Fichiers importants**

- `include/process_manager.h`, `src/process_manager.cpp`
- `include/scheduler.h`, `src/scheduler.cpp`
- `include/kernel_api.h` (déclare `kernel_stop_requested`, `kernel_yield`)
- `src/main.cpp` (shell)

**Rappel** : ce système est pédagogique — il privilégie la simplicité et la compréhension des concepts d'ordonnancement et d'interruption coopérative.

---

Indiquez si vous souhaitez que je génère un diagramme simple (ASCII) des interactions ou un schéma des états des processus (CREATE → RUNNING → SUSPENDED → ZOMBIE → CLEANUP).  
Bon développement ! 🚀
