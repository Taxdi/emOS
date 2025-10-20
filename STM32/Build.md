# BUILD CONTAINER PlatformIO

```
docker build -t stm32-qemu .
```

```
docker run -it -v $(pwd)/..:/workspace -p 4444:4444
```

# BUILD FirmwareSTM32 (à faire depuis le container

```
pio run &
```

# Accès STM32 (depuis la machine hôte)

```
socat -, raw,echo=0, tcp:localhost:4444
```
