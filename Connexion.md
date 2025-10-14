# Connexion à la VM

Il y a deux possibilités pour se connecter aux machines virtuelles

## 1. Interface web

Il faut créer deux tunnels ssh : 

#### Tunnel 1 :
<pre># ssh -N -L 5556:ensibs-cl-02.univ-ubs.fr:20001 -L 5555:localhost:8006 login@ensibs-cl-01.univ-ubs.fr -i chemin/vers/cle/prive</pre>

Avec **login** qui correspond au numéro étudiant

#### Tunnel 2 :

<pre># ssh -p 5556 -L 5557:localhost:8006 root@localhost</pre>

A partir de là nous pourrons aller sur un navigateur à l'adresse https://localhost:5557 et accéderez directement à l'interface web de "notre" proxmox.

## 2. SSH directement vers une VM de proxmox

Pour se connecter en SSH, le port de connexion est le port 22. Il faut juste modifier le **tunnel 2**

<pre># ssh -p 5556 -L 5557:localhost:8006 -L 5558:X:22 root@localhost</pre>

La valeur X est l'adresse IP de la machine virtuelle sur laquelle nous souhaitons nous connecter. 
Ensuite, il faut lancer une connexion ssh sur l'adresse "localhost:5558"
