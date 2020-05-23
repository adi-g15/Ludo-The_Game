#Ludo - The Game
Ludo, our popular Indian board game, completely implemented in C++

DEVELOPER NOTE -
1. Even though, the codebase may seem big, or confusing, to understand how I would advice to see which "name of the functions" that gets called - 
    1. main.cpp
    2. board.cpp::Init_Game()
    3. board.cpp::play()
I think only seeing the names of functions calls in these, you would get the workflow of this(I have tried to name the functions such)... then if you want to dive more to how a particular function works, see its definition (tried to separate all significant code portions into differently named functions)

2. //! or '/**/' -> This syntax for comments has been used for giving info of the code

3. The whole structure of the code is such that most things can be easily changed, to be able to show effect elsewehere too, some of which for eg.
	a> In most apps, even if you get a 6, you have to decide your move then, but at our gaon, we could proceed to roll the die, till its not 6, YOU can opt any of those, basic change dieNumbers vector to a single integer!
	b> To change 'order of gameplay', you only need to change one vector, ie. the colourOrder<>
	c> Different 'players can be bound to different gotis', as you want, just update the colourMap and playerMap
	d> To change 'number of gotis', just update the goti_per_user, and theoretically it just got implemented! No fuss
	d.2> And in the updateDisplay(), use if conditions to add an additional type2() row, for each +2 gotis of goti_per_user [The board will be rectangular, and i have 8 types of rows, use combination of them, for more gotis]
	e>Many more if you really intend to search (yaar please consider point 1, if that's the case :-)