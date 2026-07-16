# 🎮 Multi-Tank

A C++ console-based **local 2-player tank battle game** built with OOP principles. Two players share a keyboard, fight across a randomly generated map full of obstacles, and try to destroy each other's tank.

Built as a learning project — actively in development.

---

## 📸 What It Looks Like

```
+=========================================================+
| Player1 : 10                               Player2 : 7  |
+=========================================================+
|                  #########                              |
|   /|\                          +~+| |+~+                |
|   !O!      ~~~                                          |
|   \=/                  ###[X]###                        |
|                                            /=\          |
|         [|] X [|]                          O~~          |
|                                            \=/          |
+=========================================================+
```

- Randomly generated map every round — different every time
- 250–300 randomly placed obstacles from a preset library
- Two tanks spawn at opposite corners with correct facing directions
- Live HUD shows both player names and health in their colors

---

## 🕹️ Controls

| Action       | Player 1 | Player 2 |
|--------------|----------|----------|
| Move Up      | `W`      | `I`      |
| Move Down    | `S`      | `K`      |
| Move Left    | `A`      | `J`      |
| Move Right   | `D`      | `L`      |
| Rotate Left  | `Q`      | `U`      |
| Rotate Right | `E`      | `O`      |
| Fire         | `F`      | `P`      |

---

## ✅ What Works

- **Random map generation** — grid with border walls and randomly seeded obstacle clusters placed every round
- **Tank rendering** — 3×3 directional tank sprites that change shape based on facing direction
- **Tank movement** — both players can move and rotate independently
- **Wall collision** — tanks check their full 3×3 footprint before moving and stop at borders and obstacles
- **Bullet firing** — bullets spawn from the tank's nose in the correct direction
- **Bullet-wall collision** — bullets erase themselves when they hit any non-empty cell
- **HUD** — health bar at the top of the screen, player names in their colors, redraws only when health changes
- **Main menu** — add/remove players, start round, exit
- **Duplicate player name detection**
- **Player colors** — each tank gets a unique console color

---

## 🐛 Known Bugs & What's Still Missing

### Bugs

- **Tanks erase obstacles** — when a tank moves, erasing its old position writes spaces unconditionally. If an obstacle overlapped that area, the obstacle character gets blanked out. The map grid still has the correct data; the visual is just wrong. Fix: re-read `map.getpoint()` for each erased cell and redraw the map character if it isn't a space.

- **Bullets pass through tanks** — `checkcollisionstanks()` exists but its body is empty. Bullets currently fly through enemy tanks without dealing any damage. The function needs to iterate over all bullets and check if any bullet position falls inside an enemy tank's 3×3 footprint.

- **Bullet collision box is off** — bullets check the *next* position before moving, which sometimes causes them to "die" one cell early next to a wall, or slip through a thin gap. Needs a tighter per-step check.

- **No win condition** — the game loop runs forever. When one tank's health reaches zero nothing happens. A check for alive player count needs to be added after collision resolution.

- **No victory screen** — winner display, ASCII big text, and "Press Enter to Continue" flow are not implemented yet.

---

## 🗺️ Planned Features

- [ ] Bullet-tank collision (damage system)
- [ ] Win condition detection
- [ ] Victory screen with large ASCII text
- [ ] Obstacle redraw fix after tank erase
- [ ] Bullet owner tracking (no self-damage)
- [ ] LAN multiplayer scaffold (4 players via network, architecture already planned as `IController` → `LocalController` / `NetworkController`)

---

## 🏗️ Architecture

```
Game_Manager
├── Map              — grid[56][201], random obstacle placement
├── Player[]         — name + Tank
│   └── Tank         — health, color, position, direction, alive
├── Bullet[]         — direction, position (owner field: planned)
└── Game Loop
    ├── checkcollisionsbullets()   — bullet vs wall
    ├── checkcollisionstanks()     — bullet vs tank (empty — WIP)
    ├── checkmovementsbullets()    — advance bullets
    └── checkmovementstanks()      — keyboard input + tank movement
```

---

## 🛠️ Build

Requires Windows (uses `conio.h` for `_getch` / `_kbhit`) and a C++17 compiler.

```bash
g++ -std=c++17 -o MultiTank Multi-Tank.cpp
./MultiTank
```

> Zoom out your terminal 5–6× (`Ctrl + -`) before starting a round — the grid is 201 columns wide.

---

## 📁 Files

| File | Description |
|------|-------------|
| `Multi-Tank.cpp` | Main game source |
| `utilities.h` | Console helpers — colors, `gotoxy`, `design::draw_header`, typewriter, spinner |

---

## 👤 Author

**Basit Ahmad** — [@Basit-Ahmad-GCUF](https://github.com/Basit-Ahmad-GCUF)  
*Basit Creative Media*
