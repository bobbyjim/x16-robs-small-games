# x16-rogue-forest
A somewhat roguelike for the Commander X16

# The Map
* a procedural random dungeon in PETSCII (a forest)
  - clearings can be moved through
  - woods and monsters cannot be moved through

# The Goal
* retrieve the Amulet of Yendor, and escape/perish with as much game points as possible

# Obstacles
* permadeath.  You have one life.

# Gameplay
* turn-based, using cursor keys to move, and the keyboard for one-letter commands
* resource management, and luck, are key to survival
* hack-and-slash
  - bump into a monster to attack it.

* On-screen status of player and game.
  - strength
  - armor and rating
  - weapon and rating
  - score
  - food and hunger levels
  
# Objects
* described by owner, color, type, and attribute
  - typically, the more descriptors, the better
  - descriptors are in natural hierarchies
  - (maybe I should probably mix those hierarchies up each run!!)

* object types
  - food
  - Weapons
  - Armor
  - The Amulet of Yendor
