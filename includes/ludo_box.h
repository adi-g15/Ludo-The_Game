#pragma once

#include "ludo_goti.h"
#include<vector>
#include<memory>
#include<map>
#include<string>

class ludo_box{
	const coordinate coords;
	std::vector<std::shared_ptr<ludo_goti>> inBoxGotis;	//! Can make it a map too, but might be inefficient for such less data
	std::string content;

	//! NOTE - Before calling this function, explicitly add the shared_ptr to lockedPositions
    bool removeGoti(std::shared_ptr<ludo_goti>&);

public:
	BOX_TYPE box_type;

	//! Returns 0 if attacked other goti, -1 if unsuccessful; In most cases, this return value will be ignored, only in moveGoti functions it work
	short appendGoti(std::shared_ptr<ludo_goti>);
	std::string get_box_content() const;
	void sanitizeContent(void);
	bool isPresent(const ludo_goti& goti) const;
	inline bool isEmpty(void) const;

	bool areOpponentsPresent(colours) const;

	ludo_box(const coordinate&, BOX_TYPE = _boxNORMAL);

	friend class game;
	friend class _BoardPrinter;
	friend class ludo_state;

};