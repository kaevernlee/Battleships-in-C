# Battleships-in-C
The classic 1v1 board game battleships ring any bells? If yes {Great! I've coded the game in C and you can enjoy this at the comfort of your own terminal!} else {You've been under a rock, but that's okay! Read the rest and have fun!}.

## Ship
There are 5 ships for each player, each ship has a location on the board and a length and orientation. The length of a ship corresponds to the number of hittable sections, once a section has been hit, it will unable to be hit again. Once a ship has been hit the program outputs `We have hit the target!`. Once the ship has been destroyed, the program outputs `We have sunk your ` followed by the ship name. If the shot is missed, your program outputs `You have missed!`.

There are 5 different ships and are covers a different amount of cells given to each player:

* Carrier, 5 cells
* Battleship, 4 cells
* Destroyer, 3 cells
* Submarine, 2 cells
* Patrol Boat, 2 cells

A player will have a chance to take a shot at the opponents fleet. The player will need to input the coordinates in the following format:

`[A-H] [1-8]`.

Coordinate placement starts from left (specified coordinate) to right for horizontal and top (specified coordinate) to bottom denoted by H (horizontal) or V (vertical).

### Example (Configuration)
Player 1, please set up your ships (y, x, orientation)
Carrier: A 1 H
Battleship: D 5 V
Destroyer: C 5 H
Submarine: B 2 H
Patrol Boat: G 7 H

## Commands
* shots - Shows the shots on a grid 
* fire <A-H> <1-8> - Fires a shot at this location
* exit - Quits the game early
