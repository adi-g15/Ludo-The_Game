#pragma once

#include <iostream>

using std::clog;
using std::endl;

/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
#define DEBUG_START std::clog << "[DEBUG] START :" << std::endl;
#define DEBUG_END std::clog << "\n: [DEBUG] PASSED" << std::endl;
/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
/*HELPFUL FOR GETTING TO THE SEG_FAULT SITE*/
#define DEBUG_BLOCK(code) \
    DEBUG_START           \
    code                  \
        DEBUG_END
/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
