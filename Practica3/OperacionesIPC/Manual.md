# Manual de Operaciones IPC

## Semáforos

```bash
# Creación de semáforos
./OperacionesIPC sem crear NSEMS [PERMS]

# Establecer semáforos
./OperacionesIPC sem establecer SEMID SEMNUM VAL

# Leer todos los valores de un semáforo
./OperacionesIPC sem leer SEMID
# Leer el valor de un sólo semáforo
./OperacionesIPC sem leer SEMID SEMNUM

# Operar todos los semáforos
./OperacionesIPC sem operar SEMID OP
# Operar un sólo semáforo
./OperacionesIPC sem operar SEMID OP SEMNUM

# Liberar un semáforo
./OperacionesIPC sem liberar SEMID
```

## Memoria compartida

```bash
# Crear nuevo bloque de memoria compartida
./OperacionesIPC shm crear NBYTES [PERMS]

# Establecer los datos de un bloque de memoria compartida a partir de la entrada estandar
./OperacionesIPC shm escribir SHMID

# Leer los datos de un bloque de memoria compartida hasta encontrar un final de cadena 
# o llegar al final de la memoria.
./OperacionesIPC shm leer SHMID

# Liberar el bloque de memoria compartida con el shmid especificado
./OperacionesIPC shm liberar SHMID
```

## Mensajes

```bash
# Crear una nueva cola de mensajes
./OperacionesIPC msg crear [PERMS]

# Enviar un mensaje desde con contenidos desde la entrada estándar
./OperacionesIPC msg enviar MSGID TYPE

# Recibir un mensaje con el tipo especificado
./OperacionesIPC msg recibir MSGID TYPE

# Liberar una cola de mensajes
./OperacionesIPC msg liberar MSGID
```
