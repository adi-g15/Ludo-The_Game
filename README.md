[![Codacy Badge](https://api.codacy.com/project/badge/Grade/af9190f3627842869828fb1c8307b818)](https://app.codacy.com/manual/AdityaGupta150/Ludo-The_Game?utm_source=github.com&utm_medium=referral&utm_content=AdityaGupta150/Ludo-The_Game&utm_campaign=Badge_Grade_Dashboard)
![C/C++ CI](https://github.com/AdityaGupta150/Ludo-The_Game/workflows/C/C++%20CI/badge.svg)
[![Build Status](https://travis-ci.org/AdityaGupta150/Ludo-The_Game.svg?branch=master)](https://travis-ci.org/AdityaGupta150/Ludo-The_Game)

# Ludo - The Game
Ludo, our popular Indian board game, completely implemented in C++

_Please do put your suggestions, ideas, and any problem (even if why g++ main.cpp doesn't work). I will be thankful to you for giving suggetions on this :D_

## Building
### On Linux -
```sh
make
make run
```

### For Windows -
Either install make, or open the Makefile, and execute the commands by yourself, in the order, starting from the 'run' target

## Files

```
.
├── extras
│   ├── boardPrinter.hpp
│   ├── enumerations.hpp
│   ├── exceptions.h
│   └── utilities.hpp
├── includes
│   ├── boardPrinter.hpp
│   ├── enumerations.hpp
│   ├── exceptions.h
│   ├── game.h
│   ├── ludo_box.h
│   ├── ludo_coords.h
│   ├── ludo_goti.h
│   ├── simpleTuple.hpp
│   └── utilities.hpp
├── src
|   ├── CMakeLists.txt
|   ├── game.cpp
|   ├── game.h
|   ├── main.cpp
|   ├── snake.cpp
|   └── snake.h
├── Comments.txt
├── LICENSE.md
└── src
```

## DEVELOPER NOTE -
1. Even though, the codebase may seem big, or confusing, to understand how, you should see the "name of the functions" that gets called inside these, in this order -

    * main.cpp
    * board.cpp::Init_Game()
    * board.cpp::play()

2. I think only seeing the names of functions calls in these, you would get the workflow of this(the functions have been named such)... then if you want to dive more, see the `includes` directory first, and then `src` to know how a particular function works.

3. `//!` or `/**/` -> This syntax for comments has been used for giving info of the code.

4. The whole structure of the code is such that most things can be easily changed, to be able to show effect elsewehere too, some of which for eg.
	1. To change **Order of Gameplay**, you only need to change one vector, ie. `colourOrder<>`
	2. Different **Players can be bound to different gotis**, as you want, just update the `colourMap` and `playerMap`
	3. To change **Number of Gotis**, just update the `goti_per_user`, and _internally_ it just got implemented! No fuss
        * And, for display on output : In the updateDisplay(), may use if conditions to add an additional type2() row, for each +2 gotis of goti_per_user
    4. In most apps, even if you get a 6, you have to decide your move then, but at our _gaon_, we could proceed to roll the die, till its not 6, YOU can opt any of those ways, for that, replace `dieNumbers<>` to a single integer!
    5. Many more if when you search (yaar please consider point 1, if that's the case :-)	
	
## Future Plans - 
* Using the basics of _alpha-beta minimax algorithm_ to create a 'robot' mode. It may not be that well, but at least it will 'think' rather than random.
* Creating a _web version_ of this, probably using reactJS
* Creating _GUI_ versions, using wxWidgets

> Thanks ! :D
