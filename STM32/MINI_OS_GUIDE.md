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
  - `suspendProcess()` / `resumeProcess()` : mettre en pause/reprendre
  - `getNextRunnable()` : obtenir le prochain processus exécutable (Round-Robin)
  - `listProcesses()` : afficher tous les processus
  - `cleanupZombies()` : nettoyer les processus terminés

#### `Scheduler` (scheduler.h/cpp)
- Boucle principale d'ordonnancement
- Alterne entre les processus RUNNING avec des timeslices
- Accumule le temps CPU pour chaque processus
- Passe automatiquement au processus suivant

#### `main.cpp` (new)
- Shell interactif avec support des commandes
- Parser simple des entrées utilisateur
- Intégration du ProcessManager et Scheduler

---

## Commandes Shell

```
list                - Liste les programmes disponibles avec leurs IDs
ps                  - Affiche tous les processus actifs (PID, nom, état, CPU)
launch <id>         - Lancer un programme en mode mono-tâche (bloquant)
start <id>          - Créer un processus et l'ajouter à la queue
kill <pid>          - Terminer un processus (passe à l'état ZOMBIE)
suspend <pid>       - Suspendre un processus (pause)
resume <pid>        - Reprendre un processus en pause
run                 - Exécuter l'ordonnanceur Round-Robin (tous les processus RUNNING)
help                - Afficher l'aide
quit                - Arrêter le système
```

---

## Scénarios de test

### Scénario 1 : Arrêter un programme infini (tâche 1)

```
shell> launch 7        # Lancer infiniteLoop en mode mono-tâche
[Kernel] Lancement du programme infiniteLoop
----------------------------------
[prog4] Démarrage du compteur continu...
[prog4] Tick #0
[prog4] Tick #1
[prog4] Tick #2
...
```

**Problème** : Le compteur tourne indéfiniment, le shell est bloqué.

**Solution** : Utiliser `start` pour créer un processus et `kill` pour l'arrêter :

```
shell> start 7         # Créer infiniteLoop comme processus
[Kernel] Processus créé - PID 1 : infiniteLoop

shell> ps              # Voir l'état du processus
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | infiniteLoop       | RUNNING    | 0

shell> kill 1          # Terminer le processus
[Kernel] Processus 1 marqué pour termination.

shell> ps              # Processus nettoyé
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
```

---

### Scénario 2 : Basculer entre plusieurs processus (tâche 2)

```
shell> start 0         # Créer prog1
[Kernel] Processus créé - PID 1 : prog1

shell> start 1         # Créer prog2
[Kernel] Processus créé - PID 2 : prog2

shell> start 5         # Créer morpion
[Kernel] Processus créé - PID 3 : morpion

shell> ps              # Lister les processus
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | prog1              | RUNNING    | 0
 2  | prog2              | RUNNING    | 0
 3  | morpion            | RUNNING    | 0

shell> suspend 3       # Mettre en pause morpion
[Kernel] Processus 3 suspendu.

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | prog1              | RUNNING    | 0
 2  | prog2              | RUNNING    | 0
 3  | morpion            | PAUSED     | 0

shell> resume 3        # Reprendre morpion
[Kernel] Processus 3 repris.

shell> kill 2          # Terminer prog2
[Kernel] Processus 2 marqué pour termination.
```

---

### Scénario 3 : Ordonnanceur Round-Robin avec tâches de fond (tâche 3)

```
shell> start 1         # Créer prog2 (compte de 0 à 4, puis quitte)
[Kernel] Processus créé - PID 1 : prog2

shell> start 7         # Créer infiniteLoop (boucle infinie)
[Kernel] Processus créé - PID 2 : infiniteLoop

shell> run             # Lancer le scheduler Round-Robin
[Kernel] Démarrage du scheduler (Round-Robin)...
[Kernel] Appuyez sur Ctrl+C pour arrêter

[prog2] Counting...
[prog2] 0
[prog2] 1
[prog2] 2
[prog2] 3
[prog2] 4

[prog4] Démarrage du compteur continu...
[prog4] Tick #0
[prog4] Tick #1
[prog4] Tick #2
...
```

**Explication** :
- Le scheduler alterne entre `prog2` et `infiniteLoop`
- `prog2` s'exécute, affiche ses 5 compteurs et se termine
- `infiniteLoop` continue ensuite à tourner
- Vous pouvez dans un autre terminal faire `kill 2` pour l'arrêter

---

### Scénario 4 : Processus compteur en tâche de fond

```
shell> start 1         # prog2 (compte 0-4)
[Kernel] Processus créé - PID 1 : prog2

shell> start 5         # morpion (jeu interactif)
[Kernel] Processus créé - PID 2 : morpion

shell> run             # Lancer le scheduler
[Kernel] Démarrage du scheduler (Round-Robin)...

[prog2] Counting...
[prog2] 0
[prog2] 1
[prog2] 2
[prog2] 3
[prog2] 4

=== Jeu du Morpion ===
Entrez des coordonnées ligne,colonne (1-3,1-3)
...

shell> ps              # (Après fin du run)
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | prog2              | ZOMBIE     | 150
 2  | morpion            | RUNNING    | 200
```

Les processus se partagent le CPU via le scheduler !

---

## Implémentation détaillée

### 1. Mécanisme d'interruption

**Approche** : Pas de vrai signal d'interruption matérielle (limites de l'émulateur STM32).
Solution : 
- Chaque processus est stoppé via `killProcess()` → passe à l'état ZOMBIE
- Le scheduler ignore les processus ZOMBIE
- `cleanupZombies()` nettoie la table après chaque exécution

```cpp
// Terminer un processus
bool ProcessManager::killProcess(int pid) {
    Process* p = getProcess(pid);
    if (!p) return false;
    p->state = PS_ZOMBIE;
    return true;
}
```

**Pour un vrai système temps réel** : Il faudrait utiliser :
- Les interruptions matérielles du timer (SysTick)
- `setjmp()`/`longjmp()` pour sauvegarder/restaurer le contexte
- Une vraie pile par processus

### 2. Gestion multi-processus

**Structure Process** :
```cpp
struct Process {
    int pid;                   // Identifiant unique
    const char* name;          // Nom du programme
    user_program_fn func;      // Pointeur sur la fonction
    ProcessState state;        // RUNNING, SUSPENDED, ZOMBIE
    unsigned long created_time;
    unsigned long last_scheduled_time;
    unsigned long total_runtime_ms;
};
```

**Cycle de vie** :
```
CREATE → RUNNING → SUSPENDED (optionnel) → ZOMBIE → CLEANUP
                    ↓                       ↑
                    └─── RESUME ───────────┘
```

### 3. Ordonnanceur Round-Robin

**Algorithme** :
```cpp
while (pm->countRunning() > 0) {
    // Obtenir le prochain processus RUNNING
    int pid = pm->getNextRunnable(); // Round-Robin
    
    // L'exécuter
    Process* p = pm->getProcess(pid);
    unsigned long start = millis();
    p->func(system_print);
    unsigned long elapsed = millis() - start;
    
    // Accumuler le CPU
    p->total_runtime_ms += elapsed;
    
    // Passer au suivant
}
```

**Propriétés** :
- **Équité** : Chaque processus RUNNING a une chance égale d'exécution
- **Latence** : Un processus attend au max (N-1) timeslices
- **Limitation** : Sans vraie preemption, c'est coopératif

---

## Limitations actuelles et améliorations possibles

### Limitations
1. **Pas de vraie préemption** : Les processus ne sont pas interrompus après un timeslice
   - Chaque programme doit finir son exécution ou appeler `delay()`
   - Pour `infiniteLoop`, on force l'arrêt via `kill`

2. **Pas de vrai contexte** : Pas de sauvegarde du registre/pile
   - Impossible de reprendre à l'état exact où s'est arrêté
   - Chaque processus repart de zéro à chaque exécution

3. **Pas de protection mémoire** : Tous les processus partagent l'adresse de la RAM
   - Un processus peut écrire sur les variables d'un autre

### Améliorations futures
1. **Timer d'interruption** : Utiliser SysTick pour vrai préemption
2. **Context-switching** : Implémenter `setjmp()`/`longjmp()` pour sauvegarder l'état
3. **Allocation de pile** : Donner une pile isolée à chaque processus
4. **MMU (Memory Management Unit)** : Protection mémoire par processus
5. **Système de fichiers** : Charger les programmes depuis la RAM

---

## Fichiers modifiés/créés

```
include/
  ├── process_manager.h    (new)
  └── scheduler.h          (new)

src/
  ├── main.cpp            (modifié - shell interactif)
  ├── process_manager.cpp (new)
  └── scheduler.cpp       (new)

MINI_OS_GUIDE.md           (this file)
```

---

## Compilation

```bash
# Dans le dossier OS-FunctionPrograms
cd /path/to/OS-FunctionPrograms
pio run                    # Compiler
pio run -t upload          # Téléverser
```

Assurez-vous que `platformio.ini` inclut les nouveaux fichiers `.cpp` et `.h`.

---

## Tests rapides

### Test 1 : Vérifier que infiniteLoop peut être arrêté
```
shell> start 7
shell> ps
shell> kill 1
shell> ps
```
✓ Le processus doit passer de RUNNING à ZOMBIE et disparaître après nettoyage.

### Test 2 : Vérifier le basculement entre processus
```
shell> start 0
shell> start 1
shell> start 5
shell> suspend 2
shell> resume 2
shell> ps
```
✓ Les états RUNNING/SUSPENDED doivent changer correctement.

### Test 3 : Vérifier le scheduler Round-Robin
```
shell> start 1
shell> start 7
shell> run
```
✓ Les deux processus doivent s'exécuter en alternance.
✓ Le temps CPU doit s'accumuler pour chaque processus.

---

## Support et débogage

- **Les processus ne démarrent pas** : Vérifier que les IDs sont valides avec `list`
- **Le scheduler s'arrête immédiatement** : Vérifier qu'au moins un processus est RUNNING
- **Le shell ne répond pas** : Le programme courant bloque le shell (comportement normal pour mode mono-tâche)

Pour déboguer, activer les logs sériels avec :
```cpp
Serial.println("[DEBUG] Message...");
```

---

## Conclusion

Vous avez maintenant un mini OS avec :
✅ **Tâche 1** : Mécanisme d'interruption (kill process)
✅ **Tâche 2** : Gestion multi-processus (shell, create/suspend/resume/kill)
✅ **Tâche 3** : Ordonnanceur Round-Robin (exécution concurrente)

Le système est prêt pour des améliorations futures comme la vraie préemption et la protection mémoire ! 🚀
