# Connexion à la VM

Il y a deux possibilités pour se connecter aux machines virtuelles

## 1. Interface web

Il faut créer deux tunnels ssh : 

#### Tunnel 1 :
<pre> ```bash # Ceci est un exemple de bloc de code # Tu peux copier tout ce bloc facilement echo "Hello World" ls -la ``` </pre>

ssh -N -L 5556:ensibs-cl-02.univ-ubs.fr:20001 -L 5555:localhost:8006 login@ensibs-cl-01.univ-ubs.fr -i chemin/vers/cle/prive

Avec **login** qui correspond au numéro étudiant

#### Tunnel 2 :

ssh -p 5556 -L 5557:localhost:8006 root@localhost

A partir de là nous pourrons aller sur un navigateur à l'adresse https://localhost:5557 et accéderez directement à l'interface web de "notre" proxmox.


