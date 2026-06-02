# Bomberman

## Descripción General

Bomberman es una implementación en C++ inspirada en el clásico juego arcade del mismo nombre.

El objetivo del juego es eliminar a los enemigos y sobrevivir hasta el final de la partida utilizando bombas estratégicamente colocadas sobre el mapa. Las explosiones pueden destruir bloques rompibles, eliminar enemigos y dañar jugadores. Durante la partida pueden aparecer mejoras (Power-Ups) que incrementan las capacidades de los jugadores.

El proyecto soporta:

* Modo multijugador local.
* Configuración de jugadores antes de iniciar la partida.
* Diferentes mapas.
* Enemigos controlados por IA.
* Sistema de explosiones y reacciones en cadena.
* Pantalla de pausa.
* Pantalla de resultados al finalizar la partida.
* Arquitectura basada en eventos utilizando hilos (threads) para distintos subsistemas.

---

## Requisitos

### Compilador

Se requiere un compilador compatible con:

* C++17 o superior

Por ejemplo:

* GCC 9+
* Clang 10+
* MSVC con soporte para C++17

---

### Librerías

#### SFML

El proyecto utiliza SFML para:

* Renderizado gráfico
* Manejo de ventanas
* Entrada por teclado
* Manejo de tiempo

Versión recomendada:

* SFML 2.5 o superior

#### POSIX Threads

Se utilizan hilos POSIX (`pthread`) para la ejecución concurrente de distintos sistemas del juego.

En Linux normalmente se encuentran disponibles por defecto.

---

## Instalación de Dependencias

### Ubuntu / Debian

```bash
sudo apt update

sudo apt install \
    build-essential \
    cmake \
    libsfml-dev
```

---

### Arch Linux

```bash
sudo pacman -S \
    base-devel \
    cmake \
    sfml
```

---

### Fedora

```bash
sudo dnf install \
    gcc-c++ \
    cmake \
    SFML-devel
```

---

## Estructura General

```text
Bomberman/
│
├── assets/
│
├── include/
│   ├── core/
│   ├── entities/
│   ├── systems/
│   ├── screens/
│   └── ...
│
├── src/
│   ├── core/
│   ├── entities/
│   ├── systems/
│   ├── screens/
│   └── ...
│
├── tests/
│
├── CMakeLists.txt
│
└── README.md
```

---

## Construcción del Proyecto

### Generar archivos de compilación

Desde la raíz del proyecto:

```bash
mkdir build
cd build

cmake ..
```

---

### Compilar

```bash
cmake --build .
```

o alternativamente:

```bash
make
```

---

## Ejecución

Desde el directorio `build`:

```bash
./bomberman
```

---

## Ejecución de Pruebas

Dependiendo de las pruebas disponibles:

```bash
./PlayerTest
```

```bash
./TableroTest
```

```bash
./RenderSnapshotTest
```

---

## Arquitectura

El proyecto utiliza una arquitectura basada en eventos.

### Componentes principales

#### Engine

Coordina el estado general de la partida.

Responsabilidades:

* Gestión de jugadores.
* Estado de la partida.
* Procesamiento de eventos.
* Comunicación entre subsistemas.

---

#### EventBus

Cola de eventos segura para múltiples hilos.

Permite la comunicación entre:

* Jugadores.
* Enemigos.
* Bombas.
* Motor principal.

---

#### EnemySystem

Gestiona:

* Movimiento de enemigos.
* Generación de eventos de movimiento.
* Estado de los enemigos.

---

#### BombSystem

Gestiona:

* Colocación de bombas.
* Explosiones.
* Reacciones en cadena.
* Creación de eventos relacionados con explosiones.

---

#### Tablero

Representa el estado del mapa.

Contiene:

* Terreno.
* Ocupantes.
* Colisiones.
* Posiciones de entidades.

---

## Controles

### Jugador 1

* Movimiento: WASD
* Bomba: Espacio

### Jugador 2

* Movimiento: Flechas
* Bomba: LShift

### Otros jugadores

Los controles pueden configurarse según la implementación actual del proyecto.

---

## Estado del Proyecto

Proyecto académico desarrollado como una implementación concurrente del juego Bomberman utilizando:

* C++17
* SFML
* pthreads
* CMake

con énfasis en:

* Programación orientada a objetos.
* Arquitecturas basadas en eventos.
* Programación concurrente.
* Diseño modular de sistemas.

