[![Build Status](https://travis-ci.org/AdityaGupta150/Ludo-The_Game.svg?branch=master)](https://travis-ci.org/AdityaGupta150/Ludo-The_Game)

# Ludo - The Game
----
Ludo, our popular Indian board game, completely implemented in C++

## Building
----
### On Linux -
```sh
$ make
$ make run
```

### For Windows -
Either install make, or open the Makefile, and execute the commands by yourself, in the order, starting from the 'run' target

## DEVELOPER NOTE -
----
1. Even though, the codebase may seem big, or confusing, to understand how, you should see the "name of the functions" that gets called inside these, in this order -

    * main.cpp
    * board.cpp::Init_Game()
    * board.cpp::play()

2. I think only seeing the names of functions calls in these, you would get the workflow of this(the functions have been named such)... then if you want to dive more to how a particular function works, see its definition

2. `//!` or `/**/` -> This syntax for comments has been used for giving info of the code

3. The whole structure of the code is such that most things can be easily changed, to be able to show effect elsewehere too, some of which for eg.
	1. To change **Order of Gameplay**, you only need to change one vector, ie. `colourOrder<>`
	2. Different **Players can be bound to different gotis**, as you want, just update the `colourMap` and `playerMap`
	3. To change **Number of Gotis**, just update the `goti_per_user`, and _internally_ it just got implemented! No fuss
    * And, for display on output : In the updateDisplay(), may use if conditions to add an additional type2() row, for each +2 gotis of goti_per_user
	e>Many more if you really intend to search (yaar please consider point 1, if that's the case :-)	
    4. In most apps, even if you get a 6, you have to decide your move then, but at our _gaon_, we could proceed to roll the die, till its not 6, YOU can opt any of those ways, for that, replace `dieNumbers<>` to a single integer!
	
> Thanks ! :D
