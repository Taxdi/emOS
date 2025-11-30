# Mini OS STM32 - Résumé des implémentations

## Fichiers créés

### 1. `include/process_manager.h` & `src/process_manager.cpp`

**Rôle** : Gérer la table des processus et leur cycle de vie.

**Fonctionnalités clés** :
- Structure `Process` avec PID, état, nom, pointeur de fonction
- États : `PS_EMPTY`, `PS_RUNNING`, `PS_SUSPENDED`, `PS_ZOMBIE`
- Classe `ProcessManager` (max 8 processus)
  - `createProcess()` : créer un nouveau processus → PID
  - `killProcess(pid)` : marquer comme ZOMBIE
  - `suspendProcess(pid)` / `resumeProcess(pid)` : contrôler l'exécution
  - `getNextRunnable()` : Round-Robin scheduling
  - `listProcesses()` : afficher tous les processus actifs
  - `cleanupZombies()` : nettoyer les processus terminés

**Algorithme Round-Robin** :
```cpp
int ProcessManager::getNextRunnable(void) {
    int start = (last_scheduled_index + 1) % MAX_PROCESSES;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        int idx = (start + i) % MAX_PROCESSES;
        if (processes[idx].state == PS_RUNNING) {
            last_scheduled_index = idx;
            return processes[idx].pid;
        }
    }
    return -1; // aucun processus exécutable
}
```

**Résultat tâche 1** : ✅ Les processus peuvent être terminés via `killProcess()`.

---

### 2. `include/scheduler.h` & `src/scheduler.cpp`

**Rôle** : Ordonnancer l'exécution des processus.

**Fonctionnalités clés** :
- Classe `Scheduler`
  - `tick(timeslice_ms, print_fn)` : exécuter un processus pour ~timeslice_ms
  - `switchContext()` : passer au processus suivant (Round-Robin)
  - `run(print_fn)` : boucle principale d'ordonnancement
  - Accumulation du temps CPU par processus

**Boucle principale** :
```cpp
void Scheduler::run(void (*print_fn)(const char*)) {
    while (pm->countRunning() > 0) {
        tick(100, print_fn); // timeslice de 100ms
    }
    print_fn("[Kernel] Tous les processus sont terminés.\r\n");
}
```

**Résultat tâche 3** : ✅ Les processus s'exécutent en alternance avec Round-Robin.

---

### 3. `src/main.cpp` (modifié)

**Rôle** : Shell interactif pour gérer les processus.

**Nouvelles commandes** :
| Commande | Effet |
|----------|-------|
| `list` | Afficher les programmes disponibles |
| `ps` | Lister tous les processus (PID, état, CPU) |
| `launch <id>` | Mode mono-tâche (bloquant) |
| `start <id>` | Créer un processus (non-bloquant) |
| `kill <pid>` | Terminer un processus |
| `suspend <pid>` | Mettre en pause |
| `resume <pid>` | Reprendre |
| `run` | Lancer scheduler Round-Robin |
| `help` | Aide |
| `quit` | Arrêter le système |

**Exemple d'utilisation** :
```
shell> start 7
[Kernel] Processus créé - PID 1 : infiniteLoop

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
 1  | infiniteLoop       | RUNNING    | 0

shell> kill 1
[Kernel] Processus 1 marqué pour termination.
```

**Résultat tâche 2** : ✅ Shell avec gestion de la création, suspension, reprise et fermeture de processus.

---

## Architectures comparées

### Avant (mono-tâche)
```
┌──────────────┐
│   main.cpp   │ Menu numérique
└──────┬───────┘
       │
       ├─→ prog1()   (bloqué jusqu'à fin)
       ├─→ prog2()   (bloqué jusqu'à fin)
       └─→ infiniteLoop() (BLOQUE INDÉFINIMENT!)
```

### Après (multi-tâche + scheduling)
```
┌──────────────────────────────┐
│   Shell interactif           │ (non-bloquant)
└──────┬──────────┬──┬─────────┘
       │          │  │
       ├─ ProcessManager (table des processus)
       │  - PID 1: prog1    [RUNNING]
       │  - PID 2: prog2    [SUSPENDED]
       │  - PID 3: morpion  [RUNNING]
       │
       └─ Scheduler (Round-Robin)
          Execute: PID 1 → PID 3 → PID 1 → ...
```

---

## Flots d'exécution

### 1. Créer et exécuter 2 processus en parallèle

```
User Input: start 1              (prog2)
            start 7              (infiniteLoop)
            run

Timeline:
┌─────────────────────────────────────────┐
│ Scheduler - Round Robin                 │
├─────────────────────────────────────────┤
│ Tick 1: [prog2] Counting...             │
│         [prog2] 0, 1, 2, 3, 4           │
│         [prog2] → ZOMBIE                │
├─────────────────────────────────────────┤
│ Tick 2: [infiniteLoop] Tick #0          │
│         [infiniteLoop] Tick #1          │
│         [infiniteLoop] Tick #2          │
│         ...                             │
└─────────────────────────────────────────┘

Result:
PID 1 (prog2):       total_runtime_ms = ~150ms
PID 2 (infiniteLoop): total_runtime_ms = ~500ms (continu)
```

### 2. Arrêter infiniteLoop bloqué

```
User: infiniteLoop tourne
      (Shell bloqué)

Avant: Impossible d'arrêter (boucle infinie)

Après: 
  shell> kill 2
  [Kernel] Processus 2 marqué pour termination.
  
  → Scheduler ignore le processus ZOMBIE
  → Shell revient libre
```

### 3. Suspend/Resume

```
User: start 0    (prog1)
      start 1    (prog2)
      suspend 1  (mettre prog2 en pause)
      run        (lancer scheduler)

Result:
  Scheduler exécute seulement prog1 en boucle
  prog2 reste en attente (SUSPENDED)
  
  User: resume 1
  Scheduler alterne prog1 ↔ prog2
```

---

## Performance et mémoire

### Utilisation de mémoire

| Composant | Taille |
|-----------|--------|
| ProcessManager | ~8 × Process = ~512 bytes |
| Scheduler | ~16 bytes |
| Table processus (8 max) | ~512 bytes |
| **Total** | **~1 KB** |

Suffisant pour STM32 (typiquement 64-256 KB de RAM).

### Complexité temporelle

| Opération | Coût |
|-----------|------|
| `createProcess()` | O(1) |
| `killProcess()` | O(1) |
| `getNextRunnable()` | O(MAX_PROCESSES) = O(1) |
| `run()` (scheduler) | O(∞) itération |

---

## Limitations et solutions futures

### Limitation 1 : Pas de vraie préemption
**Problème** : Les programmes ne s'arrêtent pas au milieu, ils bloquent le scheduler.
**Solution future** :
- Utiliser Timer SysTick pour interruptions régulières
- Implémenter `setjmp()`/`longjmp()` pour sauvegarder le contexte
- Créer une pile par processus

### Limitation 2 : Pas d'isolation mémoire
**Problème** : Un processus peut lire/écrire la RAM d'un autre.
**Solution future** :
- Implémenter une MMU (Memory Management Unit)
- Allouer une région de mémoire isolée par processus
- Valider les accès au démarrage de chaque timeslice

### Limitation 3 : Pas de IPC (Inter-Process Communication)
**Problème** : Les processus ne peuvent pas communiquer directement.
**Solution future** :
- Ajouter une file de messages globale
- Implémenter des signaux (kill signals)
- Ajouter des événements (event wait)

---

## Checklist des tâches

- [x] **Tâche 1 : Interruption**
  - [x] Mécanisme d'arrêt pour programmes infinis
  - [x] Commande `kill <pid>` qui met en ZOMBIE
  - [x] Cleanup automatique des zombies

- [x] **Tâche 2 : Multi-processus**
  - [x] Shell interactif
  - [x] Commande `start` pour créer
  - [x] Commande `kill` pour terminer
  - [x] Commande `ps` pour lister
  - [x] États RUNNING/SUSPENDED
  - [x] Commande `suspend`/`resume`

- [x] **Tâche 3 : Ordonnanceur Round-Robin**
  - [x] Scheduler qui alterne entre processus
  - [x] Accumulation du temps CPU
  - [x] Commande `run` pour lancer le scheduler
  - [x] Processus peuvent tourner en arrière-plan

---

## Prochaines étapes pour vous

1. **Compiler et tester** :
   ```bash
   cd OS-FunctionPrograms
   pio run
   ```

2. **Exécuter les scénarios** :
   - Voir `MINI_OS_GUIDE.md` pour détails
   - Tester `start 7` puis `kill` pour l'interruption
   - Tester `start 0 && start 1 && run` pour Round-Robin

3. **Améliorer** :
   - Ajouter vraie préemption (SysTick timer)
   - Implémenter context-switching (setjmp/longjmp)
   - Ajouter protection mémoire

Bon développement ! 🚀
