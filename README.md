![CMake](https://github.com/AdityaGupta150/Ludo-The_Game/workflows/CMake/badge.svg)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/af9190f3627842869828fb1c8307b818)](https://app.codacy.com/manual/AdityaGupta150/Ludo-The_Game?utm_source=github.com&utm_medium=referral&utm_content=AdityaGupta150/Ludo-The_Game&utm_campaign=Badge_Grade_Dashboard)
[![deepcode](https://www.deepcode.ai/api/gh/badge?key=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwbGF0Zm9ybTEiOiJnaCIsIm93bmVyMSI6IkFkaXR5YUd1cHRhMTUwIiwicmVwbzEiOiJMdWRvLVRoZV9HYW1lIiwiaW5jbHVkZUxpbnQiOmZhbHNlLCJhdXRob3JJZCI6MjIzMzksImlhdCI6MTYwMDg5MDE2NH0.bip9zdSe9te0vQIgIyzTOp_Ga4j2dmkWRlmpt7oC4Gc)](https://www.deepcode.ai/app/gh/AdityaGupta150/Ludo-The_Game/_/dashboard?utm_content=gh%2FAdityaGupta150%2FLudo-The_Game)
[![CodeFactor](https://www.codefactor.io/repository/github/adi-g15/ludo-the_game/badge)](https://www.codefactor.io/repository/github/adi-g15/ludo-the_game)
[![Build Status](https://travis-ci.org/adi-g15/Ludo-The_Game.svg?branch=master)](https://travis-ci.org/adi-g15/Ludo-The_Game)

The repo now has a web version too, added by @Athi223, and is available in web/ directory, and deployed at https://ludo-the-game-a10a0.web.app/ :tada:

# Ludo - The Game v2.0

Ludo, our popular Indian board game, completely implemented in C++

<img src=./extras/ludoScreen.png
raw=true alt=“ScreenShot”>

_Ask any doubts in the #[Discussions](https://github.com/adi-g15/Ludo-The_Game/discussions) :D_

## Features

* Easy and Powerful **customisability** *(More in developer notes)*
* **Variable number of inputs** at all input fields
* **Shortcuts, Keywords, _Cheat codes_**
* **Two robot modes** -
  * RandomRobo
  * ThinkerRobo (it will chose the best possible move at a time, from almost 30-90 possibilities)
* Display **adaptive** to width of screen
* Memory-safe (using smart\_pointers, and uncounted for 'new' statements)
* **Multi-threaded** (for ThinkerRobo execution)
* **0 dependencies** outside stdc++  (for coloured output support, agauniyal.github.io/rang/ is utilised and included as a header rang.hpp)
* Plus all basic ludo game features, and my own *custom* keywords _(you can create yours too :D )_

## Building

> On Linux & Windows -

```sh
mkdir build -p
cd build
cmake ..
cmake --build .
```

> After building you will have a `ludo.exe` inside build/ (Linux) or build/{Debug,Release} (Windows), that's your executable 😃

### Installation

> On Arch, simply run 'makepkg -si', the PKGBUILD file will handle rest

After #Building, run `cmake --install .`

### Files in the Project

```
.
├── extras
├── util    # utility library
├── includes
│   ├── boardPrinter.hpp			# showing ludo board
│   ├── rang.hpp					# for colored output
│   ├── die.hpp						# die; functions for die roll
│   ├── enums.hpp					# enums; (coord,colour,Player etc.)
|   ├── exceptions.hpp				# errors
|   ├── game.hpp					# **[MAIN]** The Game class (and all functions)
|   ├── keywords.hpp				# constants
|   ├── ludo_box.hpp				# :)
|   ├── ludo_coords.hpp				# Stores corners and home locations (coordinates, not fixed)
|   ├── ludo_goti.hpp				# :)
|   ├── ludo_state.hpp				# For robots, a lightweight class to store 'state'
│   └── thinker.hpp					# For robots, thinker class, provides logic for robots
├── src
|   ├── game.cpp
|   ├── ludo_box.cpp
|   ├── ludo_coords.cpp
|   ├── ludo_goti.cpp
|   ├── ludo_state.cpp
|   ├── thinker.cpp
├── main.cpp
├── CMakeLists.txt
├── LICENSE.md
└── README.md
```

## READING THE CODE

> You **DON'T** need to understand 'all' code to understand this program
>
> **Discussion**: If you can't understand something, simply ask that in 'Discussion' section, let's grow together !, we will reply :D

1. Even though, the codebase may seem big, or confusing, you should see the "name of the functions" that gets called inside these, in this order -

    * main.cpp
    * board.cpp::Init\_Game()
    * board.cpp::play()

2. I think only seeing the names of functions calls in these, you would get the workflow of this(the functions have been named such)... then if you want, see the `includes` directory first, and for how the function is implemented, see `src`.

3. `//!` or `/**/` -> This syntax for comments has been used for giving info of the code.

> The whole structure of the code is such that most things can be easily changed, to be able to show effect elsewehere too, some of which for eg.
>
>
> * To change **Order of Gameplay**, you only need to change one vector, ie. `colourOrder<>`
>
> * Different **Players can be bound to different gotis**, as you want, just update the `colourMap` and `playerMap`
>
> * To change **Number of Gotis**, just update the `goti_per_user`, and _internally_ it just got implemented! No fuss
>
> * And, for display on output : In the updateDisplay(), may use if conditions to add an additional type2() row, for each +2 gotis of goti_per_user
>
> * In most apps, even if you get a 6, you have to decide your move then, but at our _gaon_, we could proceed to roll the die, till its not 6, YOU can opt any of those ways, for that, replace `dieNumbers<>` to a single integer!
>
> * Many more if when you search (yaar please consider point 1, if that's the case :-)
>

## Contribution -

Start an issue for whatever feature you believe should be good here.

And, in case you want to be a contributer here, and work on it together, do ask.

Do sugggest any other feature you would like, i do claim the codebase to be quite modular, and easily customisable (It was what i used to think earlier, it IS modular enough to understand, i am not modifying it now, so as to keep it more original when i made it).

> Thanks !   	||	   धन्यवाद् 🙏
