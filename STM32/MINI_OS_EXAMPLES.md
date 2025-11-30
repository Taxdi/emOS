shell> suspend 1
# Mini OS — Exemples d'utilisation (version révisée)

Ce fichier contient des scénarios pas-à-pas pour utiliser le shell et le scheduler du mini OS.

Pré-requis
-----------

- Compiler le projet dans `OS-FunctionPrograms` avec `pio run`.
- Se connecter au port série (115200) pour utiliser le shell.

Exemple 1 — Lancer plusieurs processus et utiliser le scheduler
-------------------------------------------------------------

1) Créer deux processus :

```
shell> start 1         # prog2 (compteur 0..4)
shell> start 7         # infiniteLoop
```

2) Vérifier les processus :

```
shell> ps
```

3) Lancer l'ordonnanceur :

```
shell> run
```

Observations :
- `prog2` exécutera ses itérations puis se terminera (ZOMBIE).
- `infiniteLoop` continuera tant qu'on ne demande pas son arrêt.
- Appuyez `Ctrl+C` dans le terminal pour demander l'arrêt coopératif (le programme doit appeler `kernel_yield()` pour être réactif).

Exemple 2 — Démarrer, suspendre et reprendre
--------------------------------------------

```
shell> start 4         # morpionv3
shell> start 5         # morpionv4
shell> ps
shell> suspend <pid>   # suspendre un PID obtenu via ps
shell> resume <pid>
```

Exemple 3 — Arrêter un processus bloquant
-----------------------------------------

1) Démarrer la tâche bloquante : `start 7` (infiniteLoop)
2) Noter le PID via `ps`
3) `kill <pid>` pour demander l'arrêt coopératif

Le programme affichera un message d'arrêt et le scheduler nettoiera la table.

Exemple 4 — Scénario avancé (tests automatisés manuels)
-----------------------------------------------------

- Enchaînez `start 1`, `start 7`, `run` puis dans un autre terminal exécutez `kill <pid>` pour observer la réaction asynchrone.

Règles utiles
------------

- `kernel_yield()` : les programmes cooperatifs doivent l'appeler régulièrement pour être préemptibles (déjà ajouté dans `infiniteLoop`).
- `kill` marque un processus en `ZOMBIE` et déclenche le flag d'arrêt coopératif.
- Le scheduler nettoie périodiquement les zombies.

Dépannage rapide
----------------

- Aucun processus listé après `start` → Vérifier que `createProcess()` a renvoyé un PID valide.
- `kill` sans effet → vérifier que le programme appelle `kernel_yield()` ou `delay()`.

