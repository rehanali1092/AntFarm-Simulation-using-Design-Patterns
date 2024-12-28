# Ant Farm Simulator

Welcome to the **Ant Farm Simulator**, a C++ simulation of ant colonies and their interactions. This program models the behaviors of different types of ants, their activities within rooms, and the dynamics of colonies. The user can control the simulation through a command-line interface.

## Features

1. **Ant Types**:
   - **Drone**: Gathers resources.
   - **Warrior**: Engages in combat.
   - **Queen**: Spawns new ants.

2. **Rooms**:
   - Host ants and manage their resting states.
   - Each room has a maximum resting capacity.

3. **Colonies**:
   - Create colonies with specific species and manage them by unique IDs.

4. **Tick-based Simulation**:
   - Progress the simulation by defined tick cycles.

5. **Command-line Control**:
   - Monitor and control the simulation in real time.

---

## How to Build

1. **Prerequisites**:
   - C++ compiler (e.g., g++, clang++)
   - C++17 support or higher

2. **Compilation**:
   ```bash
   g++ -o AntFarmSimulator main.cpp -std=c++17
   ```

3. **Run the Program**:
   ```bash
   ./AntFarmSimulator
   ```

---

## Commands

### 1. `spawn X Y T`
- **Description**: Spawns a new colony at `(X, Y)` with species `T`.
- **Example**: `spawn 1 1 Killer`

### 2. `give I R A`
- **Description**: Gives resource `R` of amount `A` to colony `I`.
- **Example**: `give 1 food 100`

### 3. `tick [T]`
- **Description**: Progresses the simulation by `T` ticks (default is 1).
- **Example**: `tick 5`

### 4. `summary I`
- **Description**: Provides a summary of colony `I`.
- **Example**: `summary 1`

### 5. `exit`
- **Description**: Ends the simulation.

---

## Program Structure

### Classes

#### 1. `Ant`:
- Base class for all ants with common attributes (health, attack, defense).
- Handles behaviors like resting, ticking, and battling.

#### 2. Derived Ant Classes:
- **Drone**: Resource gatherer.
- **Warrior**: Combat specialist.
- **Queen**: Colony spawner.

#### 3. `AntRoom`:
- Hosts and manages ant interactions.
- Handles resting and active states of ants.

#### 4. `AntFarm`:
- Oversees rooms and colonies.
- Supports colony creation and tick simulation.

#### 5. `Meadow`:
- Singleton class managing the global simulation environment.

---

## Sample Usage

```plaintext
Welcome to the Ant Farm Simulator!
Available commands:
  spawn X Y T
  give I R A
  tick [T]
  summary I
Type 'exit' to quit.
> spawn 1 1 Killer
Colony created with ID: 1
> tick 5
Tick executed for 5 cycles.
> summary 1
Species: Killer
Health: 100
Active Ticks: 5
Status: Alive
> exit
```
```

Let me know if you'd like any further adjustments!
