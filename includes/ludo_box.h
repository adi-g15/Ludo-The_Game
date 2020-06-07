#pragma once

#include "ludo_goti.h"
#include<vector>
#include<memory>
#include<map>
#include<string>

enum BOX_TYPE{
	_boxNORMAL,
	_boxLOCK,
	_boxSTOP,
	_boxHOMEPATH,
	_boxHOME_END,
	_boxHOMEAREA,
	_boxUNUSABLE
};

class ludo_box{
	std::vector<std::shared_ptr<ludo_goti>> inBoxGotis;	//! Can make it a map too, but might be inefficient for such less data
	std::string content;
	std::pair<int,int> coords;

	//! NOTE - Before calling this function, explicitly add the shared_ptr to lockedPositions
    bool removeGoti(std::shared_ptr<ludo_goti>&);

public:
	BOX_TYPE box_type;
	std::map<colours, int> numGotis;
	std::weak_ptr<ludo_goti> getGoti(colours);

	short appendGoti(std::shared_ptr<ludo_goti>);	//! Returns 0 if attacked other goti, -1 if unsuccessful; In most cases, this return value will be ignored, only in moveGoti functions it work
	std::string get_box_content() const;
	void sanitizeContent(void);
	bool isPresent(const ludo_goti& goti) const;
	bool isEmpty(void);

	ludo_box();

	friend class game;
	friend class _BoardPrinter;
};