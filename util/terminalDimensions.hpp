#ifndef CUSTOM_EXCEPTION
  #include "./exceptions.hpp"
#endif

#include "./simpleTuple.hpp"

#ifdef __linux__
    #include<sys/ioctl.h>
    #include<unistd.h>
#elif __MINGW32
    #include<windows.h>
#elif __CYGWIN32
    #include<windows.h>
#endif

intTuple getTerminalDimen(){  //NOTE - Replace intTuple with simpleTuple<int,int> gives error : conversion from ‘simpleTuple<int, int>’ to non-scalar type ‘intTuple {aka homoTuple<int>}’ requested
  intTuple retTuple(0,0);
  #ifdef __linux__
    winsize windowsSize;
    ioctl (STDOUT_FILENO, TIOCGWINSZ, &windowsSize);
      retTuple[0] = windowsSize.ws_row;
      retTuple[1] = windowsSize.ws_col;

  #else //Windows API
    {
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &csbi );
        retTuple[0] = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        retTuple[1] = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

  #endif

  return retTuple;
}
