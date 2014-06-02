//
//  main.cpp
//  Puzzle
//

#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif

#include "piece.hpp"
#include "table.hpp"

#include "functions.hpp"

#include "io.h"
#include "game.h"

using namespace std;

const int tableSize = 4;

int main(int argc, const char * argv[]) {
    functions::clearConsole();
    
    /*
     * Sample Output of _printTable():
     *
     * <6> <1> <2> <4> <10>
     * <16> <11> <7> <8> <3>
     * <21> <12> <13> <9> <5>
     * <17> <=> <18> <14> <15>
     * <22> <23> <24> <20> <19>
     *
     * <3> <6> <8>
     * <5> <1> <7>
     * <=> <2> <4>
     */
	
#ifdef WIN32
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
              |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
#endif
	game TheGame;
	TheGame.MainMenu();
    
    return 0;
}

