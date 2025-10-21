# Génération de zombies

Dans un programme zombie_generator.c --> on crée des zombies à intervalle régulier (2s)
Il y a une fonction qui crée les zombies, un handler qui permet de créer la commande USR1 pour tuer les zombies

## Pour compiler le programme :

```
gcc -Wall -O2 -o zombie_manager zombie_manager.c   
```

## Pour afficher les processus

```
ps --ppid <PID_DU_PROGRAMME> -o pid,ppid,stat,cmd
```

## Pour kill les processus zombie : 

```
kill -USR1 <PID_DU_PROGRAMME>
```
