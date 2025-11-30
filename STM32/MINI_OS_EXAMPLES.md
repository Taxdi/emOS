# Mini OS STM32 - Exemples pratiques d'utilisation

## Prérequis

- Avoir compilé et uploadé le code sur le STM32/QEMU
- Accès à la console sérielle (115200 baud)
- Familiérité avec les commandes shell

---

## Exemple 1 : Arrêter infiniteLoop (Tâche 1 - Interruption)

### Problème
Le programme `infiniteLoop` (prog4) tourne indéfiniment :
```cpp
void infiniteLoop(void (*print)(const char*)) {
    print("[prog4] Démarrage du compteur continu...\r\n");
    unsigned long count = 0;
    while (true) {  // ← BOUCLE INFINIE
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "[prog4] Tick #%lu\r\n", count++);
        print(buffer);
        delay(500);
    }
}
```

### Solution avec le nouveau OS

**Étape 1 : Lancer le shell**
```
[Kernel] Démarrage du Mini OS...
[Kernel] Système prêt. Tapez 'help' pour l'aide.

shell> 
```

**Étape 2 : Lister les programmes**
```
shell> list

Programmes disponibles :
  ID 0 : prog1
  ID 1 : prog2
  ID 2 : prog3
  ID 3 : morpion
  ID 4 : morpionv3
  ID 5 : morpionv4
  ID 6 : morpionv5
  ID 7 : infiniteLoop

shell>
```

**Étape 3 : Créer le processus infiniteLoop**
```
shell> start 7
[Kernel] Processus créé - PID 1 : infiniteLoop

shell>
```

**Étape 4 : Vérifier l'état**
```
shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | infiniteLoop       | RUNNING    | 0

shell>
```

**Étape 5 : Terminer le processus**
```
shell> kill 1
[Kernel] Processus 1 marqué pour termination.

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---

shell>
```

✅ **Résultat** : Le processus a été arrêté correctement !

---

## Exemple 2 : Basculer entre plusieurs jeux (Tâche 2 - Multi-processus)

### Objectif
Créer 3 versions du Morpion et basculer entre elles sans exécution en tâche de fond.

**Étape 1 : Créer les processus**
```
shell> start 4        # morpionv3
[Kernel] Processus créé - PID 1 : morpionv3

shell> start 5        # morpionv4
[Kernel] Processus créé - PID 2 : morpionv4

shell> start 6        # morpionv5
[Kernel] Processus créé - PID 3 : morpionv5

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | morpionv3          | RUNNING    | 0
 2  | morpionv4          | RUNNING    | 0
 3  | morpionv5          | RUNNING    | 0

shell>
```

**Étape 2 : Mettre en pause certains jeux**
```
shell> suspend 2      # Pause morpionv4
[Kernel] Processus 2 suspendu.

shell> suspend 3      # Pause morpionv5
[Kernel] Processus 3 suspendu.

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | morpionv3          | RUNNING    | 0
 2  | morpionv4          | PAUSED     | 0
 3  | morpionv5          | PAUSED     | 0

shell>
```

**Étape 3 : Jouez au premier jeu (mode mono-tâche)**
```
shell> launch 4       # Lancer morpionv3 directement
=== Morpion interactif v3 ===
Z=haut S=bas Q=gauche D=droite, Entrée pour jouer, Quitter en bas

 X |   |  
---+---+---
   |   |  
---+---+---
   |   |  

Joueur X -> 
```

(Jouez normalement... tapez 'q' pour quitter)

```
Fin de partie.
```

**Étape 4 : Reprendre les jeux en pause**
```
shell> resume 2       # Reprendre morpionv4
[Kernel] Processus 2 repris.

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | morpionv3          | ZOMBIE     | 500
 2  | morpionv4          | RUNNING    | 0
 3  | morpionv5          | PAUSED     | 0

shell>
```

**Étape 5 : Nettoyer et tuer des processus**
```
shell> kill 1         # Tuer le zombie
[Kernel] Processus 1 marqué pour termination.

shell> kill 3         # Tuer morpionv5
[Kernel] Processus 3 marqué pour termination.

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 2  | morpionv4          | RUNNING    | 0

shell>
```

✅ **Résultat** : Vous pouvez créer, suspendre, reprendre et tuer les processus facilement !

---

## Exemple 3 : Exécution concurrente (Tâche 3 - Round-Robin)

### Objectif
Exécuter `prog2` (compteur 0-4) et `infiniteLoop` en parallèle avec Round-Robin.

**Étape 1 : Créer les processus**
```
shell> start 1        # prog2 (compte 0-4)
[Kernel] Processus créé - PID 1 : prog2

shell> start 7        # infiniteLoop (boucle infinie)
[Kernel] Processus créé - PID 2 : infiniteLoop

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | prog2              | RUNNING    | 0
 2  | infiniteLoop       | RUNNING    | 0

shell>
```

**Étape 2 : Lancer le scheduler Round-Robin**
```
shell> run
[Kernel] Démarrage du scheduler (Round-Robin)...
[Kernel] Appuyez sur Ctrl+C pour arrêter (simulé par timeout)

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
[prog4] Tick #3
[prog4] Tick #4
...
```

**Observation** :
- `prog2` s'exécute complètement et finit
- `infiniteLoop` continue après
- Sans vrai preemption, un processus bloque le scheduler jusqu'à fin

**Étape 3 : Tuer infiniteLoop depuis un autre terminal**

*(Dans un autre shell/terminal)*
```
shell> kill 2
[Kernel] Processus 2 marqué pour termination.

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 2  | infiniteLoop       | ZOMBIE     | 5000

shell>
```

*(Le premier terminal affiche)*
```
[prog4] Tick #100
[prog4] Tick #101
...
[Kernel] Tous les processus sont terminés.

shell>
```

✅ **Résultat** : Les processus s'exécutent en Round-Robin !

---

## Exemple 4 : Benchmark du scheduler

### Objectif
Mesurer le temps CPU consommé par chaque processus.

**Setup**
```
shell> start 0        # prog1 (affichage simple)
[Kernel] Processus créé - PID 1 : prog1

shell> start 1        # prog2 (compteur 0-4)
[Kernel] Processus créé - PID 2 : prog2

shell> start 2        # prog3 (message simple)
[Kernel] Processus créé - PID 3 : prog3

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | prog1              | RUNNING    | 0
 2  | prog2              | RUNNING    | 0
 3  | prog3              | RUNNING    | 0

shell> run
```

**Résultat après exécution**
```
[prog1] Hello from program 1!
[prog1] Tick...
[prog1] Tick...
[prog1] Tick...
[prog2] Counting...
[prog2] 0
[prog2] 1
[prog2] 2
[prog2] 3
[prog2] 4
[prog3] This is a simple message from program 3.

[Kernel] Tous les processus sont terminés.

shell> ps
[Kernel] Processus actifs :
PID | Nom                | État       | CPU (ms)
--- | --- | --- | ---
 1  | prog1              | ZOMBIE     | 120
 2  | prog2              | ZOMBIE     | 150
 3  | prog3              | ZOMBIE     | 100

shell>
```

✅ **Résultat** : 
- `prog2` a consommé le plus de CPU (150ms) car il a 5 itérations
- `prog1` a consommé 120ms (4 délai courtes)
- `prog3` a consommé 100ms (message simple)

---

## Exemple 5 : Cas d'erreur et gestion

### Cas 1 : ID de programme invalide
```
shell> start 10
[Error] ID de programme invalide.

shell>
```

### Cas 2 : Trop de processus
```
shell> start 0
[Kernel] Processus créé - PID 1 : prog1
shell> start 0
[Kernel] Processus créé - PID 2 : prog1
... (répéter 8 fois)
shell> start 0
[Error] Trop de processus. Limite atteinte.

shell>
```

### Cas 3 : Tuer un processus qui n'existe pas
```
shell> kill 999
[Error] Processus non trouvé.

shell>
```

### Cas 4 : Suspendre un processus déjà suspendu
```
shell> suspend 1
[Kernel] Processus 1 suspendu.
shell> suspend 1
[Error] Processus non trouvé ou non exécutable.

shell>
```

✅ **Résultat** : Gestion d'erreur correcte !

---

## Commandes rapides (cheat-sheet)

### Visualisation
```bash
help                    # Afficher l'aide
list                    # Lister les programmes
ps                      # Afficher l'état des processus
```

### Gestion des processus
```bash
start 0                 # Créer prog1
launch 7                # Lancer infiniteLoop directement (bloquant)
suspend 1               # Mettre en pause PID 1
resume 1                # Reprendre PID 1
kill 1                  # Terminer PID 1
```

### Scheduler
```bash
run                     # Lancer Round-Robin
quit                    # Arrêter le système
```

---

## Flux de travail recommandé

### Pour tester l'interruption
```
1. start 7              (créer infiniteLoop)
2. ps                   (vérifier RUNNING)
3. kill 7               (arrêter)
4. ps                   (vérifier ZOMBIE)
```

### Pour tester le multi-processus
```
1. start 4              (morpionv3)
2. start 5              (morpionv4)
3. suspend 5            (pause morpionv4)
4. launch 4             (jouer morpionv3)
5. resume 5             (reprendre morpionv4)
6. ps                   (vérifier états)
```

### Pour tester le Round-Robin
```
1. start 1              (prog2)
2. start 7              (infiniteLoop)
3. run                  (scheduler)
4. (dans autre terminal) kill 2
5. ps                   (voir CPU consommé)
```

---

## Astuces de débogage

### Affichage personnalisé
Modifiez le code pour ajouter des logs :
```cpp
Serial.println("[DEBUG] Mon message de débogage");
```

### Vérifier l'état en temps réel
```bash
shell> ps               # Après chaque action
```

### Mesurer les performances
```bash
shell> run              # Les temps CPU sont affichés à la fin
shell> ps               # Voir les statistiques détaillées
```

### Nettoyer les ressources
Si le système semble gelé :
```bash
shell> ps               # Voir s'il y a des ZOMBIE
shell> kill <pid>       # Tuer les zombies
```

---

## Conclusion

Vous pouvez maintenant :
✅ Arrêter les programmes infinis (tâche 1)
✅ Gérer plusieurs processus via shell (tâche 2)
✅ Exécuter les processus en Round-Robin (tâche 3)

Bon développement ! 🚀
