Après avoir réalisé les prérequis du fichier STM32.md, nous allons pouvoir construire les différents conteneurs nécessaires.

# BUILD CONTAINER PlatformIO

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

# BUILD FirmwareSTM32 (à faire depuis le conteneur )
Nous allons désormais monter le firmware du stm32 depuis le conteneur installé. Pour ce faire, nous allons nous rendre dans le dossier **/OS-FunctionPrograms** et lancer la commande suivante : 

```
pio run -t clean && pio run
```
Un message affichant "LED Off" s'affichera si tout se passe bien.

# Accès STM32 (depuis la machine hôte)

Pour accéder au STM32 depuis la machine hôte, nous allons exécuter la commande suivante : 

```
socat -, raw,echo=0 tcp:localhost:4444
```
Nous pouvons désormais reprendre le cours du sujet.
