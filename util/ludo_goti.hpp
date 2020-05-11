#define CUSTOM_LUDO_GOTI 1

#ifndef CUSTOM_ENUMERATIONS
    #include "enumerations.hpp"
#endif
#ifndef TMP
    #include "global.hpp"
#endif

#ifndef CUSTOM_SIMPLETUPLE
    #include "simpleTuple.hpp"
#endif

#ifndef _GLIBCXX_FUNCTIONAL
    #include<functional>
#endif

class ludo_goti{
protected:
	colours gotiColour;
	direction curr_direction;
	intTuple curr_coords;	

public:
	const intTuple getCoords() const;
    direction get_curr_direction() const;
    colours get_gotiColour(void) const;

	friend void moveGoti(ludo_goti&, unsigned int);

	ludo_goti();
};

class laalGoti : public ludo_goti{
	laalGoti();
};
class hariGoti : public ludo_goti{
	hariGoti();
};
class peeliGoti : public ludo_goti{
	peeliGoti();
};
class neeliGoti : public ludo_goti{
	neeliGoti();
};


//  FUTURE   //
//[FUTURE] - Likely chose between having coordinates, or pointer to ludo_goti(consider the NULL checks)
//[DONE][FUTURE] - Improve security of this function, else coordinates being private wont be useful... Solution used... made moveGoti a friend function
