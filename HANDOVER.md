# Handover & Project Status: You Are What You Eat - Rogue

**Repository:** `venim1103/You-Are-What-You-Eat-Rogue`  
**Current Branch:** `master`  
**Latest Production Commit:** `a7bb5d1`  
**GitHub Pages Target:** `venim1103.github.io/You-Are-What-You-Eat-Rogue`  
**Document Date:** October 2026  

---

## 1. Executive Summary & Core Game Concept

*You Are What You Eat - Rogue* is a turn-based dungeon crawler written in C99. The core gameplay loop revolves around **metabolic polymorphing**:
* Killing creatures leaves dynamic corpses with real-time freshness decay (`Fresh` -> `Aging` -> `Tainted` -> `Rotten` -> decomposition).
* Devouring a corpse transforms the player into that monster, altering attributes, attack power, health pools, metabolism rates, active abilities, and traits (e.g., Goblin scavengers, wall-phasing Ghosts, life-draining Bats, venomous Snakes, or crushing Trolls).
* Humanoids have hands to wield weapons (`)`) and wear armor (`[`), whereas beasts gain natural abilities but cannot manipulate complex gear.
* The game compiles natively for Linux/Unix via GCC and Ncurses, and to WebAssembly via Emscripten (`emcc`) with a browser terminal interface and virtual controls.

---

## 2. Completed Architecture, Systems & Fixes

### A. Rendering Engine & Screen Decoupling
* **Row 0 Message Bar Protection (`src/draw.c`):**
  * `RenderFOV()` and `UpdateFOV()` were decoupled from Row 0. Fog of War raycasting strictly operates on rows >= 1.
  * Tactical text, combat alerts, and item notifications on Row 0 are no longer erased by ambient FOV updates.
* **Camera Tracking & Cursor Pinning (`src/text.c`, `src/web_curses.c`):**
  * `PlayerInfoBar()` explicitly restores `move(py, px)` after drawing the status line on row 45.
  * Standardized `refresh()` in `src/web_curses.c` to render a uniform 160-column canvas, ensuring camera centering math (`centerOnPlayer`) remains consistent.

### B. Web GUI & Mobile Usability
* **Terminal Card Bounding Box (`web/index.html`):**
  * Resolved CSS Flexbox container shrinking (`flex-shrink: 0; width: max-content;`), preventing the terminal frame from truncating halfway across mobile screens.
* **Aggressive Cache Busting (`web/index.html`):**
  * Added `Cache-Control` meta headers.
  * Dynamic timestamp query strings (`rogue.js?v=...` and `Module.locateFile` for `rogue.wasm?v=...`) guarantee mobile browsers load the newest WebAssembly binary on each release.
* **Touch Optimization:**
  * Added `document.activeElement.blur()` to touch handlers to avoid lingering button focus interfering with spacebar or Enter inputs.
  * Synchronized toolbar and mobile button layouts.

### C. Tactical Exploration ("Look Mode")
* **Interactive Reticle (`src/text.c:LookMode`):**
  * Toggled via `'v'`, `'V'`, `';'`, `'/'`, `'?'`, or the web Look button without consuming a game turn.
  * Active visual indicator: Reticle `X` rendered with high-contrast styling (`.g-look`), and the UI Look button illuminates in gold (`active-look`).
  * Live Row 0 inspection feedback for monsters (HP, ATK, traits, behavior), corpses (freshness, nutrition, polymorph preview), equipment stats, and dungeon terrain.
  * The camera glides to track the Look reticle across the dungeon and smoothly resets back to the player on exit (`'v'`, `'c'`, `'i'`, `Space`, `Enter`, `Esc`).

### D. Ground Item & Corpse Feedback
* **Underfoot Inspection (`src/game.c`, `src/text.c`):**
  * Moving onto or standing on an item or corpse prints detailed guidance on Row 0 (e.g. `Standing on: Iron Dagger (+2 ATK weapon). Press 'g' to pick up.`).
  * Added a persistent indicator to the bottom HUD bar (`[Floor: Iron Dagger]` / `[Floor: Spider Corpse]`) to provide awareness even when Row 0 is occupied by combat alerts.

### E. Combat & Monster AI Overhaul
* **Non-Blocking Combat Logs (`src/combat.c`):**
  * Replaced blocking `getch()` pauses during melee exchanges with single-line batched summaries on Row 0, including target remaining HP (e.g. `CLEAVE! Hit Troll for 6 dmg! | Troll hits you for 3! [Troll: 9/15 HP]`).
* **Smarter Monster Aggro & Phasing (`src/input.c`, `src/route.c`):**
  * `FOLLOW` monsters now respect a 14-tile sensory radius or direct line-of-sight before actively tracking the player.
  * Enemy Ghosts (`G`) utilize `can_phase_walls` to drift through corridors and stone.
* **Vampire Bat Echolocation (`src/combat.c`):**
  * Echolocation screech (`'f'`) marks a 3x3 perimeter around all living entities as discovered and visible, immediately redrawing the map.

### F. Dungeon Tile Integrity
* **Door & Stair Preservation (`src/check.c`, `src/combat.c`, `src/text.c`):**
  * Opening a door records `:` into `map_backup` so raycasting will not revert it back to `+`.
  * Drops, pickups, and monster deaths occurring on stairs (`<`, `>`) or doors (`:`, `+`) preserve the underlying functional tile in `map_backup`.

---

## 3. Where We Left Off (Current State)

* The repository build pipeline is verified and passing across both native GCC and Emscripten WebAssembly targets (`.github/workflows/ci.yml`).
* Production commit `a7bb5d1` + Sprint 4 upgrades are deployed and live on GitHub Pages.
* Game state is stable regarding movement, combat flow, FOV rendering, web responsiveness, inventory management, Floor 10 boss victory progression, and Metabolic Mastery XP scaling.

---

## 4. Sprint 3 & Sprint 4 Completed Features

### Sprint 3: Core Loop & Endgame Boss
* **XP Awards on Ability & Poison Kills (`src/combat.c`, `src/game.c`):** `player->xp += target->attack` credited with `(+X XP)` combat log feedback across all active form abilities, acid/fire shield reflections, and poison deaths.
* **Ghost "Shadow Blink" Bounds & Collision Safety (`src/combat.c`):** Cycles through all 4 cardinal jump vectors (`dy = {-3, 3, 0, 0}`, `dx = {0, 0, -3, 3}`) to guarantee the destination is within bounds and free of monsters (`GetMonsterAt() == NULL`), immediately updating FOV or refunding cooldown/hunger if blocked.
* **Backpack Corpse Decay & Spoilage System (`src/game.c`, `src/text.c`):** Backpack corpses age at 50% rate (1 tick per 2 turns), decompose into dust at age 220, and retain exact elapsed age/nutrition when dropped.
* **Floor 10 Boss Encounter & Victory Condition (`src/monsters.c`, `src/combat.c`, `src/input.c`, `src/text.c`):** Ancient Dragon (`D`, 45 HP, 8 ATK, Fire Breath) + 3 Troll bodyguards on Floor 10; spawns Golden Portal (`*`) on death, triggering the `GLORIOUS VICTORY` screen and Apex Predator Dragon form upon devouring.
* **Staircase Interaction & Mobile Accessibility (`src/input.c`, `web/index.html`):** `<`/`>`/`r` staircase & portal activation plus dedicated mobile `🪜 Stairs` button.

### Sprint 4: Progression Scaling & Inventory Integrity (Latest Release)
* **Priority 1 — Metabolic Mastery XP Level-Up System (`src/player.c`, `src/combat.c`, `src/game.c`, `src/text.c`):**
  * **Status:** Completed.
  * **Scope:** Every 20 XP earned advances the player's Mastery Rank (`Rk:X` on HUD), granting `+3 Max HP`, `+3 HP` heal, and `+1 ATK` via `CheckXPUpgrade()`. Rank bonuses persist across polymorph transformations in `EatCorpse()`.
* **Priority 2 — Depth-Scaled Equipment Tiers & Mythic Gear (`src/text.c:SpawnFloorItems`):**
  * **Status:** Completed.
  * **Scope:** Floor loot tables now scale by dungeon depth (`lvl <= 2` early gear, `lvl 3-5` mid-tier gear, `lvl >= 6` endgame gear) and introduce `Runed Greatsword (+10 ATK)` and `Dragonscale Mail (+8 DEF)` on deeper floors.
* **Priority 3 — Backpack Corpse Consumption Index & Floor Preservation Fix (`src/text.c`, `src/combat.c`):**
  * **Status:** Completed.
  * **Scope:** Eating a corpse from the backpack (`ShowInventory` / `EatFromInventory`) now properly decrements `equipped_weapon` and `equipped_armor` slot indices when lower slots shift, and `EatCorpse()` only clears floor `%` tiles when the consumed corpse is the active floor corpse underfoot.

---

## 5. Next Sprint Roadmap (Future Enhancements)

* **Priority 1 — Monster Status Visuals & Web HUD Polish:** Highlight poisoned or ensnared monsters and display active buff/debuff indicators in the web UI.
* **Priority 2 — Dungeon Special Rooms & Shrines:** Add rare metabolic altars or subterranean springs on mid-floors (`lvl 4-8`) to purify tainted corpses or restore hunger.
* **Priority 3 — High Score / Run History Persistence:** Save top victory/death runs in browser `localStorage` for WebAssembly sessions.

---

## 6. Codebase Directory Map

```
You-Are-What-You-Eat-Rogue/
├── include/
│   ├── functions.h     # Declarations for combat, FOV, inventory, and navigation
│   ├── global.h        # Constants, enums, dimensions (MAX_X=160, MAX_Y=45)
│   ├── structs.h       # Entity, Corpse, Item, Level, and Position structs
│   └── web_curses.h    # Emscripten browser terminal shim headers
├── src/
│   ├── check.c         # Collision detection and tile validation
│   ├── combat.c        # Melee combat, corpse drops/eating, form abilities
│   ├── draw.c          # Raycasting FOV (UpdateFOV, RenderFOV)
│   ├── game.c          # Main game loop, turn upkeep, starvation, decay
│   ├── input.c         # Keyboard/touch dispatch, player movement
│   ├── level.c         # Level initialization, room generation, item spawning
│   ├── monsters.c      # Monster definitions, AI behaviors, spawn tables
│   ├── player.c        # Player setup, polymorph stat formulas, traits
│   ├── route.c         # Bresenham routing and pathfinding
│   ├── text.c          # HUD, inventory menu, Look Mode, stair interactions
│   └── web_curses.c    # Virtual terminal buffer and JS display bridge
├── web/
│   └── index.html      # Responsive web wrapper, D-pad, and CSS renderer
└── .github/workflows/
    └── ci.yml          # Dual GCC + Emscripten CI/CD to GitHub Pages
```
