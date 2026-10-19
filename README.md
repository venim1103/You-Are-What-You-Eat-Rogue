# You-Are-What-You-Eat-Rogue

A classic, tactical terminal roguelike in the spirit of *NetHack*, built around a literal take on an old adage: **You are what you eat.**

---

## 🍽️ The Core Concept

In *You-Are-What-You-Eat-Rogue*, hunger is not just a timer—it is your greatest tactical weapon and your most dangerous curse. 

Every time you consume the corpse of a fallen monster, you **polymorph directly into that creature**. Your current form, stats, movement, and abilities are entirely dictated by your latest meal:

- **Inherited Stats & Attributes:** Gain the strength, agility, armor, speed, and health pool of whatever you devour.
- **Intrinsic Abilities & Resistances:** Acquire monster-specific traits (night vision, poison immunity, tunneling, venomous strikes, breath attacks).
- **Inherent Drawbacks & Vulnerabilities:** Suffer the creature's natural flaws—eat a giant slug and you will be slow and vulnerable to salt; eat an ethereal beast and you might lack the physical strength to wield weapons or wear heavy armor.
- **The Hunger Dilemma:** You cannot simply starve. As hunger mounts, you will be forced to choose between starving to death or eating whatever corpse is at hand—even if turning into a fragile rat or an unwieldy brute completely alters your battle plan.

Surviving the dungeon requires thinking several meals ahead.

---

## 🕹️ Requirements

- **Linux / POSIX environment**
- **C Compiler** (`gcc` or `clang`)
- **ncurses library** (`libncurses-dev` on Debian/Ubuntu, `ncurses-devel` on Fedora/Arch)
- **Standard C library headers** (`time.h`, `stdlib.h`, etc.)
- **Terminal size:** Recommended terminal window size of at least **190 × 50** columns/rows.

---

## 🚀 Building & Running

Clone the repository and build using `make`:

```bash
# Build the executable
make

# Launch the game
make run

# Run with Valgrind for memory checking
make test

# Clean build artifacts
make clean
```
