![Tetrist - April 2018](https://i.imgur.com/mhsAPu8.png "Early Screenshot")

Tetrist is a work-in-progress falling block game clone similar in nature
to a certain classic game from the 80s. There are plans to add some kind
of spin to the gameplay to make it unique, but to start development is
focused on getting basic gameplay working (an MVP you might say).

At the time of writing the program only has limited functionality. It
spawns blocks, which fall until they collide and spawn a new piece. The
player can move left, right, or accelerate their descent, but cannot yet
rotate pieces. Filled lines are also not yet cleared. When collision
occurs at the top of the game board game-over is declared, and a timer is
presented counting down to a new game starting.

# TODO

The basic missing functionality is:

- [ ] Player being able to rotate the blocks.
  - [ ] Collision detection working with rotated blocks (either we can
          store the various possible rotated collision maps, or figure out
          how to do the calculations on a rotated map)..
- [ ] Clearing filled lines.
- [ ] Counting the player's score.
- [ ] Storing a scoreboard.

Extended functionality:

- Basic sound effects.
- Prettier graphics.
- Pixel-based graphics and logic (as opposed to tile-based).
- "Flare" (unique ideas to set it apart from other games/clones).

# INSTALL

## Dependencies

To build the program you will need Allegro 5 installed. See liballeg.org
(official site) or allegro.cc (community forum) for details.

## Makefile (Unix-like)

You will need make (development is using GNU make, but in theory it
doesn't matter). While the program should compile and run on Windows, the
makefile is probably Unix-like specific and depending on various Unix
utilities.

    $ make

The makefile supports various targets for convenience, such as `rebuild',
'run', and 'rerun'. To run the game you can either invoke an appropriate
Makefile target to run it from within make, or just execute the game
directly.

    $ make run
    $ # OR
    $ ./game

## Compiling (Alternative)

If the Makefile doesn't work on your platform you can always just compile
all the *.c files manually. Then just execute the resulting program.

# COPYRIGHT

Tetrist is a falling block game.
Copyright (C) 2018 Brandon McCaig

This file is part of Tetrist.

Tetrist is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Tetrist is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libbam.  If not, see <http://www.gnu.org/licenses/>.
