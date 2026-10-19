# Reactive exploration agent

A reactive agent in C++ that explores an unknown 2D world and maps as much of it as possible before
its battery runs out. Assignment 1 of the Artificial Intelligence course at the University of Granada
(2023/24). **It won first place in the reactive-agent category of the course's internal agents
competition (2024).**

## The problem

The agent starts in a grid world it cannot see. At each step it only receives its sensor readings
(the terrain in front of it, the agents it can see, its battery and, when known, its position and
orientation) and must choose one action. Its score is the percentage of the map it discovers.

- Terrains such as water and forest cost different amounts of battery to cross; items (a bikini and
  sneakers) make them cheaper.
- Charging cells refill the battery.
- Other agents can block the way or kill the agent, which then respawns at a random position and
  loses its items.

## My solution

All the agent's behaviour is in
[`Comportamientos_Jugador/jugador.cpp`](Comportamientos_Jugador/jugador.cpp); the simulator and the
rest of the code were provided by the course staff.

- **Internal map.** Every sensor reading is written into the agent's own map. Until it knows its
  real position it maps in its own coordinates, and merges that map into the world map once it
  locates itself.
- **Scoring each move.** The agent rates the cells around it by the battery cost of the terrain given
  the items it carries and by how much unexplored land they lead to, and moves towards the best one.
- **Frontier targeting.** When the area around it is already explored, it picks a target cell on the
  border of the unexplored region, weighted by how much unknown land surrounds it and how close it
  is, and follows a distance field to reach it.
- **Battery and items.** Charging cells and item cells are valued according to what the agent needs
  at that moment, and it keeps a distance field to the charging cells it has seen so it can go back
  when the battery is low.
- **Other agents.** Cells where it detects other agents are avoided, and after a respawn it resets
  its internal state.

## Build and run (Linux)

```bash
git clone https://github.com/manuel48265/AgenteReactivo.git
cd AgenteReactivo
./install.sh        # installs dependencies and runs cmake and make
./practica1         # with graphical interface
./practica1SG       # without interface
```
