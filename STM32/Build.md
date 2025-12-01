# Construction STM32

## Prérequis

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
Enfin, nous allons aller dans le dossier **/virtos/Ressource_OS_STM32/docker** pour construire le stm32 

-------------------------------------------------------------------------------------------------------------------------------------

## BUILD CONTAINER PlatformIO

```
docker build -t stm32-qemu .
```
Une fois le conteneur créé, nous allons utiliser le réseau pour nous connecter sur ce conteneur via le port 4444. On va également monter tout le répertoire de ressources dessus.

```
docker run --name stm32 -it -v $(pwd)/..:/workspace -p 4444:4444 stm32-qemu
```
Une fois le conteneur quitté, il est possible de retourner dessus en utilisant la commande : 

```
docker start -ai stm32
```
-------------------------------------------------------------------------------------------------------------------------------------

## BUILD FirmwareSTM32 (à faire depuis le conteneur )
Nous allons désormais monter le firmware du stm32 depuis le conteneur installé. Pour ce faire, nous allons nous rendre dans le dossier **/OS-FunctionPrograms** et lancer la commande suivante : 

```
pio run -t clean && pio run
```
Un message affichant "LED Off" s'affichera si tout se passe bien.

-------------------------------------------------------------------------------------------------------------------------------------

## Accès STM32 (depuis la machine hôte)

Pour accéder au STM32 depuis la machine hôte, nous allons exécuter la commande suivante : 

```
socat -,raw,echo=0 tcp:localhost:4444
```
