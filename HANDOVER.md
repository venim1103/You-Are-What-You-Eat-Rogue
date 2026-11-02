# Handover & Project Status: You Are What You Eat - Rogue

**Repository:** `venim1103/You-Are-What-You-Eat-Rogue`  
**Current Branch:** `master`  
**Latest Production Commit:** `dd766e9` + Sprints 11 & 12 Upgrades  
**GitHub Pages Target:** `venim1103.github.io/You-Are-What-You-Eat-Rogue`  
**Document Date:** November 2026  

---

## 1. Executive Summary & Core Game Concept

*You Are What You Eat - Rogue* is a turn-based dungeon crawler written in C99. The core gameplay loop revolves around **metabolic polymorphing**:
* Killing creatures leaves dynamic corpses with real-time freshness decay (`Fresh` -> `Aging` -> `Tainted` -> `Rotten` -> decomposition).
* Devouring a corpse transforms the player into that monster, altering attributes, attack power, health pools, metabolism rates, active abilities, and traits (e.g., Goblin scavengers, wall-phasing Ghosts, life-draining Bats, venomous Snakes, or crushing Trolls).
* The player can always open and close doors (`+` / `:`) regardless of polymorphed form so dungeon navigation never soft-locks, while humanoid forms have hands to wield weapons (`)`) and wear armor (`[`) and non-humanoid beasts rely on natural abilities (`No Gear`). Enemy monsters only open doors if their species has `can_open_doors == YES` (or phase if `can_phase_walls == 1`).
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

### Sprint 5: Monster Status Visuals, Web HUD Polish, Dungeon Shrines & Universal Player Door Access (Latest Release)
* **Priority 1 — Monster Status Visuals & Web HUD Polish (`include/structs.h`, `include/web_curses.h`, `src/web_curses.c`, `src/draw.c`, `src/combat.c`, `src/input.c`, `src/text.c`, `web/index.html`):**
  * **Status:** Completed.
  * **Scope:**
    * Added `snare_turns` to `Entity` so Spider `Spin Web` (3 turns) and Goblin `Flashbang` (2 turns) immobilize nearby foes while allowing adjacent melee counterattacks.
    * Added per-cell status flag buffer (`cell_flag_buf`) in `src/web_curses.c` populated during `RenderFOV()` (`1` = poisoned, `2` = ensnared, `3` = both) and passed to `web/index.html` (`colorizeDungeon`) to highlight afflicted monsters with toxic green (`.g-st-psn`) and web-bound (`.g-st-web` / `.g-st-both`) glow effects.
    * Surfaced monster `[PSN:Xt]` and `[WEB:Xt]` status durations in combat logs and Look Mode (`'v'`).
    * Added active form trait/buff badges (`[REGEN]`, `[PHASE]`, `[CAUSTIC]`, `[EVA:X%]`, `[VENOM]`, `[SCAV:+X%]`) to `PlayerInfoBar()` and built a live `#status-strip` pill bar in `web/index.html` displaying form, HP, Mastery Rank, hunger alerts, active buffs/debuffs, ability readiness, and visible afflicted enemy counts.
* **Priority 2 — Dungeon Special Rooms & Shrines (`include/functions.h`, `src/level.c`, `src/check.c`, `src/input.c`, `src/route.c`, `src/combat.c`, `src/game.c`, `src/text.c`, `web/index.html`):**
  * **Status:** Completed.
  * **Scope:**
    * Added `SpawnFloorShrines()` to generate single-use **Metabolic Altars (`_`)** and **Subterranean Springs (`~`)** on mid-floors (`lvl 2-9`, guaranteed 2 shrines on `lvl 4-8`).
    * **Metabolic Altar (`_`):** Purifies all backpack and nearby floor corpses back to `Fresh` (`age = 0`), cures poison, restores `+5 HP` and `+150` hunger, and awards `+10 XP` (triggering `CheckXPUpgrade()`).
    * **Subterranean Spring (`~`):** Restores `+450` hunger, heals `+10 HP`, cures poison, resets active ability cooldown (`ability_cd = 0`), and cools backpack corpses (`-60 age`).
    * Integrated shrines into movement prompts, `'r'`/`'>'`/`'<'` interaction keys, Look Mode (`'v'`), underfoot HUD indicators, tile preservation rules, and glowing Web UI styles (`.g-altar`, `.g-spring`).
* **Priority 3 — Universal Player Door Operation vs. Monster Door Rules (`src/text.c`, `src/input.c`, `src/check.c`, `src/route.c`, `src/player.c`):**
  * **Status:** Completed.
  * **Scope:**
    * Removed the `can_open_doors == NO` restriction from player door opening (`OpenDoor`) and closing (`'c'` in `HandleInput`) so the player can always open and close doors in any polymorphed form without getting trapped.
    * Retained `can_open_doors` on `Entity` to govern equipment manipulation (beasts still cannot wield weapons or wear armor, updated trait descriptions from `No Hands` to `No Gear`) and enemy AI door rules (only monsters with `can_open_doors == YES` can open closed doors `+`, while beasts remain blocked by closed doors).

### Sprint 6: Persistent Hall of Fame & Run History
* **Priority 1 — High Score / Run History Persistence (`include/web_curses.h`, `src/web_curses.c`, `src/text.c`, `web/index.html`):**
  * **Status:** Completed.
  * **Scope:**
    * Added `web_record_run()` C-to-JS bridge in `src/web_curses.c` invoked from `ExitScreen()` (`src/text.c`) on victory, death, or quit.
    * Upgraded both Victory and Game Over screens in `ExitScreen()` with unified summary blocks displaying Final Form, HP/ATK/DEF stats, Deepest Floor, Turns Taken, Mastery Rank, and calculated Final Score (`(XP * 10) + (Deepest Floor * 50) + (Victory ? 1000 : 0)`).
    * Implemented `localStorage` persistence (`yawye_rogue_hall_of_fame_v1`) in `web/index.html` storing the top 25 runs sorted by score, victory status, and XP.
    * Added a `🏆 Fame` toolbar button and interactive **Hall of Fame & Run History** modal (`#hof-modal`) featuring aggregate career stats (Saved Runs, Victories, High Score, Deepest Floor), detailed run cards with victory/death badges, keyboard dismissal (`Esc`/`h`), and history reset support.

### Sprint 7: Extended Bestiary & Environmental Floor Hazards
* **Priority 1 — Extended Bestiary (`Necromancer Lich`) & Floor Hazards (`^` Sticky Webs, `=` Acid Puddles) (`include/global.h`, `include/functions.h`, `src/level.c`, `src/monsters.c`, `src/player.c`, `src/combat.c`, `src/check.c`, `src/input.c`, `src/route.c`, `src/text.c`, `web/index.html`):**
  * **Status:** Completed.
  * **Scope:**
    * Added **Necromancer Lich (`L`)** to the mid-to-late dungeon bestiary (Floors 5+ and Floor 10 boss guard): 12 HP, 6 ATK, `FOLLOW` AI, 550 corpse nutrition, humanoid gear proficiency (`can_open_doors = YES`), 15% evasion, poison/rot immunity, `+2 HP` Soul Siphon melee life drain (`[DRAIN:+2]`), and the **Soul Harvest** active ability (drains 5 HP from all foes within 3 tiles and heals `+3 HP` per target hit).
    * Added procedural & ability-spawned **Environmental Floor Hazards**: **Sticky Spider Webs (`^`)** and **Caustic Acid Puddles (`=`)** via `SpawnFloorHazards()` and `TriggerFloorHazard()`.
    * **Sticky Spider Web (`^`):** Ensnares non-spider/non-phasing monsters for 3 turns (`snare_turns += 3`) or costs the player 20 hunger to tear free; Spiders (`x`) and Ghosts (`G`) traverse webs unimpeded, and Spider `Spin Web` (`'f'`) places up to 4 persistent `^` web traps on adjacent floor tiles.
    * **Caustic Acid Puddle (`=`):** Burns non-immune monsters for 3 acid damage + 2t poison (awarding XP and dropping corpses on lethal dissolves) or burns the player for 2 HP + 2t poison; Acid Slimes (`B`) absorb puddles for `+2 HP` and `+30 hunger`, and Slime `Acid Jet` (`'f'`) leaves caustic `=` pools on adjacent cardinal floor tiles.
    * Integrated `L`, `^`, and `=` into Look Mode (`'v'`), HUD underfoot badges, status strip pills, and Web UI syntax colorization (`.g-mon-L`, `.g-web`, `.g-acid`).

### Sprint 8: Web Audio Synthesizer & WASM Memory / Bounds Hardening
* **Priority 1 — Sound Effects & Procedural Web Audio Synthesizer (`include/web_curses.h`, `include/functions.h`, `src/web_curses.c`, `src/text.c`, `src/combat.c`, `src/player.c`, `src/game.c`, `web/index.html`):**
  * **Status:** Completed.
  * **Scope:**
    * Added `PlaySound()` and `web_play_sfx()` C-to-JS bridge (`js_play_sfx`) connecting gameplay events to a zero-dependency Web Audio API synthesizer (`window.playRogueSfx`) in `web/index.html`.
    * Synthesizes distinct procedural sound cues for melee hits (`hit`), crushing cleaves (`cleave`), player damage (`hurt`), evasion/misses (`miss`), monster kills (`kill`), boss kills (`boss_kill`), polymorphing (`polymorph`), rotten meat sickness (`sick`), active form abilities (`ability`), dragon fire (`fire`), shrine/spring invocations (`shrine`), Mastery Rank level-ups (`levelup`), item/corpse pickups (`pickup`), door opening (`door`), stair transitions (`stairs`), Floor 10 boss arrival (`boss`), victory (`victory`), and game over (`gameover`).
    * Added a persistent `🔊 SFX` / `🔇 Mute` toggle button (`#sfx-btn`) in the web toolbar backed by `localStorage` (`yawye_rogue_sfx_enabled_v1`) with automatic `AudioContext` unlock on user input.
* **Priority 2 — WebAssembly Stack / Asyncify & Bounds Safety Hardening (`Makefile`, `src/web_curses.c`, `src/level.c`, `src/player.c`, `src/monsters.c`, `src/check.c`, `src/input.c`, `src/draw.c`, `src/text.c`):**
  * **Status:** Completed.
  * **Scope:**
    * Resolved the WebAssembly `RuntimeError: Out of bounds memory access` during movement by moving the 4 KB `buf` arrays in `mvprintw()` and `printw()` (`src/web_curses.c`) to `static` storage (eliminating 4 KB per-call stack pressure on Emscripten's Asyncify unwind buffer) and expanding `-sASYNCIFY_STACK_SIZE=262144` (256 KB) and `-sSTACK_SIZE=1048576` (1 MB) in `Makefile`.
    * Hardened `RandomCoords()`, `PlayerSetup()`, `MonSetup()`, `SpawnFogMonster()`, `SpawnFloorItems()`, `CheckNextChar()`, `StepOn()`, `MonsterMove()`, `HandleInput()`, and `TurnNearby()` with strict coordinate clamping (`1 .. MAX_Y-2`, `1 .. MAX_X-2`), `room.mode > 0` guards, and bounds validation before indexing `map_backup`.

### Sprint 9: Daily Seed & Competitive Challenge Runs
* **Priority 1 — Daily Seed / Challenge Runs (`include/global.h`, `include/web_curses.h`, `src/web_curses.c`, `src/main.c`, `src/game.c`, `src/text.c`, `web/index.html`):**
  * **Status:** Completed.
  * **Scope:**
    * Added `GAME_SEED`, `GAME_SEED_MODE` (`0` = Random, `1` = Daily Challenge, `2` = Custom Seed), and `GAME_SEED_LABEL` globals in C, initialized via `web_get_initial_seed()` (`js_get_seed_value` / `js_get_seed_mode` / `js_get_seed_label` bridge in `src/web_curses.c`) on Emscripten or the `ROGUE_SEED` environment variable on native CLI builds (with deterministic 32-bit FNV-1a string hashing).
    * Added a `🎲 Seed` / `📅 Daily` / `🧪 Seed` toolbar button (`#seed-btn`) and interactive **Daily Challenge & Custom Seed** modal (`#seed-modal`) in `web/index.html` supporting one-click Daily Challenge runs (`?seed=daily`, hashed by UTC date `YYYY-MM-DD`), custom alphanumeric/numeric seeds (`?seed=<custom>`), shareable URL clipboard copying, and seed replay.
    * Surfaced active seed badges across the Intro screen, bottom HUD bar (`[Seed:...]`), clickable `#status-strip` seed pill (`.pill-seed`), Victory/Game Over summary screens, and Hall of Fame run history cards (`.hof-badge-seed`).

### Sprint 10: Interactive Bestiary & Form Compendium
* **Priority 1 — Interactive Bestiary & Form Compendium Modal & Terminal View (`include/web_curses.h`, `src/web_curses.c`, `include/functions.h`, `src/player.c`, `src/draw.c`, `src/combat.c`, `src/input.c`, `src/text.c`, `web/index.html`):**
  * **Status:** Completed.
  * **Scope:**
    * Added `RecordBestiaryEvent()` and C-to-JS bridge functions `web_unlock_bestiary()` / `web_get_bestiary_stat()` (`js_unlock_bestiary`, `js_get_bestiary_stat`) that automatically record monster sightings during `RenderFOV()`, real monster kills in `DropCorpse()` (ignoring backpack corpse drops via `dummy.mode = 0`), and polymorph transformations in `EatCorpse()`.
    * Added an in-terminal **Bestiary & Form Compendium (`ShowBestiary` via `'B'` key)** in `src/text.c` that works in both native CLI and WebAssembly builds without consuming a turn, displaying species habitat, enemy vs. polymorph stats (including live Mastery Rank scaling), corpse nutrition, metabolism burn rate, gear capability, passive traits, and active `'f'` ability details.
    * Built a persistent `📖 Bestiary (X/9)` toolbar button (`#bestiary-btn`), clickable `#status-strip` Form pill (`.pill-form`), and interactive **Bestiary & Form Compendium** modal (`#bestiary-modal`) in `web/index.html` backed by `localStorage` (`yawye_rogue_bestiary_v1`), featuring summary counters (Forms Sighted, Beast Forms Devoured, Total Monsters Slain, Metamorphoses), species cards with status badges (`ORIGIN`, `DEVOURED`, `SIGHTED`, `UNDISCOVERED`, `ACTIVE FORM`), a `👁️ Reveal Unseen` spoiler toggle, and progress reset support.

### Sprint 11: Dungeon Mini-Bosses & Elite Variants
* **Priority 1 — Floor Mini-Bosses, Elite Variants, Relic Loot Drops & Visual Aura (`include/structs.h`, `include/functions.h`, `src/monsters.c`, `src/combat.c`, `src/draw.c`, `src/game.c`, `src/player.c`, `src/text.c`, `web/index.html`):**
  * **Status:** Completed.
  * **Scope:**
    * Added `is_elite` (`0` = Normal, `1` = Elite Variant, `2` = Floor Mini-Boss / Boss) to `Entity` and `Corpse`.
    * Added guaranteed **Floor Mini-Bosses** on milestone floors: **Broodmother Arachne (`x`, Floor 3)**, **Goblin Warlord (`g`, Floor 6)**, **Arch-Lich Malakor (`L`, Floor 8)**, and **Elder Behemoth (`T`, odd Floors 11+)** with arrival warnings on stair descent.
    * Added `PromoteToElite()` in `src/monsters.c` so regular floor spawns (12%–32% scaling by depth) and ambient fog spawns (20% on Floor 2+) can promote into named **Elite Variants** (`Elite Venomweaver`, `Elite Pit Viper`, `Elite Bloodwing Bat`, `Elite Goblin Raider`, `Elite Caustic Ooze`, `Elite Phantom`, `Elite War Troll`, `Elite Lich Acolyte`) with boosted HP, `+2 ATK`, `FOLLOW` aggro, and enhanced traits.
    * Added `SpawnEliteLootDrop()` in `src/combat.c`: Mini-Bosses (`is_elite == 2`) guarantee a signature Relic weapon or armor drop (`Venomfang Sabre +6 ATK`, `Arachnid Carapace +5 DEF`, `Warlord's Cleaver +9 ATK`, `Warlord's Plate +7 DEF`, `Soulreaver Scythe +11 ATK`, `Arch-Lich Robes +9 DEF`), while Elite Variants (`is_elite == 1`) have a 55% chance to drop depth-scaled equipment, alongside `+50%` (Elite) and `+100%` (Mini-Boss) corpse nutrition bonuses.
    * Encoded bit `4` in `cell_flag_buf` (`src/draw.c`) to render a golden pulsing aura (`.g-st-elite`) around visible Elite and Boss monsters in `web/index.html`, added a `👑 Elite/Boss Sighted: X` pill to `#status-strip`, and surfaced `[ELITE]`, `[MINI-BOSS]`, and `[FLOOR BOSS]` tags in Look Mode (`'v'`).

### Sprint 12: Run Statistics & Lifetime Achievement Badges (Latest Release)
* **Priority 1 — Lifetime Career Statistics & Achievement Badges in Hall of Fame (`web/index.html`):**
  * **Status:** Completed.
  * **Scope:**
    * Added persistent career tracking (`yawye_rogue_career_stats_v1` in `localStorage`) cross-referenced with Hall of Fame run history and Bestiary progression (`totalKills`, `totalMorphs`, `devouredSpecies`, `dragonDevoured`, `dailyVictories`, `highestRank`, `deepestFloor`).
    * Added 8 unlockable **Lifetime Achievement Badges** in the Hall of Fame modal (`#hof-achievements`): **First Taste (`🍖`)**, **Deep Delver (`🕯️`)**, **Metabolic Master (`⭐`)**, **Monster Hunter (`⚔️`)**, **Dungeon Conqueror (`👑`)**, **Apex Predator (`🐉`)**, **Daily Conqueror (`📅`)**, and **Master Polymorpher (`🧬`)**.
    * Upgraded the toolbar button to display live badge progress (`🏆 Fame (X/8)`) and added lifetime kill/metamorphosis counters to the Hall of Fame header.

---

## 5. Next Sprint Roadmap (Future Enhancements)

* **Priority 1 — Consumable Dungeon Scroll / Potion Relics:** Add rare single-use tactical consumables or metabolic catalysts for deeper build variety.
* **Priority 2 — Monster Infighting & Faction Rivalries:** Allow rival dungeon species or frenzied monsters to clash when provoked or lured.
* **Priority 3 — Extended Endgame Depth Modifiers (Floors 11+ Endless Abyss):** Add scaling environmental modifiers and abyssal hazards beyond Floor 10 for endless high-score chasers.

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
