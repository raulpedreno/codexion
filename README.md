# Codexion

*Este proyecto ha sido creado como parte del currículo de 42 por rpedreno.*

## Descripción

Codexion es una simulación de concurrencia escrita en C. El programa representa a varias personas programando, llamadas coders, que comparten un número limitado de dongles USB.

Cada coder se ejecuta en un hilo independiente. Para compilar, un coder necesita tomar dos dongles: el de su izquierda y el de su derecha. Después de compilar, libera ambos dongles, depura y refactoriza. La simulación termina cuando un coder se agota o cuando todos han compilado el número de veces requerido.

El objetivo principal del proyecto es gestionar recursos compartidos usando hilos POSIX, mutexes, variables de condición y una política de planificación FIFO o EDF.

## Compilación

Para compilar el proyecto:

```bash
make
```

Esto genera el ejecutable:

```bash
./codexion
```

Para limpiar los archivos objeto:

```bash
make clean
```

Para limpiar todo, incluido el ejecutable:

```bash
make fclean
```

Para recompilar desde cero:

```bash
make re
```

## Uso

El programa recibe 8 argumentos obligatorios:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

### Argumentos

* `number_of_coders`: número de coders y también número de dongles.
* `time_to_burnout`: tiempo máximo en milisegundos sin empezar a compilar.
* `time_to_compile`: tiempo en milisegundos que tarda un coder en compilar.
* `time_to_debug`: tiempo en milisegundos que tarda un coder en depurar.
* `time_to_refactor`: tiempo en milisegundos que tarda un coder en refactorizar.
* `number_of_compiles_required`: número de compilaciones necesarias para terminar la simulación.
* `dongle_cooldown`: tiempo en milisegundos que un dongle queda bloqueado tras ser liberado.
* `scheduler`: política de planificación. Puede ser `fifo` o `edf`.

## Ejemplos

Ejemplo con FIFO:

```bash
./codexion 4 800 200 200 200 5 10 fifo
```

Ejemplo con EDF:

```bash
./codexion 4 800 200 200 200 5 10 edf
```

Ejemplo donde un único coder no puede compilar porque solo tiene un dongle:

```bash
./codexion 1 800 200 200 200 5 10 fifo
```

Ejemplo con tiempos más ajustados:

```bash
./codexion 5 600 200 150 150 3 20 edf
```

## Formato de salida

Cada cambio de estado se imprime con este formato:

```text
timestamp_in_ms coder_id action
```

Ejemplo:

```text
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
```

Si un coder se agota:

```text
1204 3 burned out
```

## Blocking cases handled

### Deadlock

Para evitar interbloqueos, los coders no toman siempre los dongles en el mismo orden. Los coders pares toman primero el dongle derecho y luego el izquierdo. Los coders impares toman primero el izquierdo y luego el derecho.

Esto reduce el riesgo de que todos cojan un dongle y se queden esperando eternamente al segundo.

### Starvation

Cada dongle tiene una cola de espera. Cuando varios coders quieren el mismo dongle, se ordenan según el scheduler elegido:

* `fifo`: primero entra, primero sale.
* `edf`: tiene prioridad quien tiene el deadline de burnout más cercano.

### Cooldown

Cuando un dongle se libera, se guarda el momento en `released_at_ms`. Antes de que otro coder pueda usarlo, se comprueba que haya pasado `dongle_cooldown`.

### Single coder

Si solo hay un coder, solo existe un dongle. Como para compilar hacen falta dos dongles, el coder toma uno y espera hasta que el monitor detecta el burnout.

### Burnout detection

Existe un hilo monitor separado que revisa constantemente el tiempo desde la última compilación de cada coder. Si un coder supera `time_to_burnout`, el monitor imprime `burned out`, detiene la simulación y despierta a los hilos que puedan estar esperando.

### Serialized logging

El log está protegido con un mutex específico. Esto evita que dos hilos impriman al mismo tiempo y mezclen mensajes en una misma línea.

## Thread synchronization mechanisms

### pthread_mutex_t

Se utilizan mutexes para proteger datos compartidos:

* `log_mutex`: protege la salida por pantalla.
* `active_mutex`: protege la variable global de estado de la simulación.
* `state_mutex`: protege el estado interno de cada coder.
* `dongle->mutex`: protege el estado interno de cada dongle.

### pthread_cond_t

Cada dongle tiene una variable de condición. Cuando un coder no puede tomar un dongle porque no es su turno o porque el dongle está ocupado, espera con `pthread_cond_wait`.

Cuando el dongle se libera o cambia su estado, se usa `pthread_cond_broadcast` para despertar a los coders que están esperando.

### Thread-safe communication

El monitor y los coders comparten información como `compile_count`, `last_compile_start_ms` y `active`. Estos datos se leen y escriben usando mutexes para evitar condiciones de carrera.

## Recursos

* Manual de `pthread_create`
* Manual de `pthread_join`
* Manual de `pthread_mutex_lock`
* Manual de `pthread_cond_wait`
* Manual de `gettimeofday`
* Documentación POSIX Threads
* Subject oficial del proyecto Codexion

## Uso de IA

Durante el desarrollo se ha utilizado IA como apoyo para:

* Entender el subject.
* Dividir el proyecto en pasos pequeños.
* Explicar conceptos de concurrencia.
* Revisar posibles problemas de sincronización.
* Preparar explicaciones para la defensa.
* Redactar documentación.

Todo el código utilizado en el proyecto ha sido revisado, probado y comprendido antes de incluirse.
