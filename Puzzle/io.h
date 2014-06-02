//
//  io.h
//  Puzzle
//

#ifndef __Puzzle__io__
#define __Puzzle__io__

#include <iostream>

#include "table.hpp"

typedef enum {
    kSexMale,
    kSexFemale
} kSex;

typedef enum {
    kReadName = 0,
    kReadAge = 1,
    kReadSex = 2,
    kReadTime = 3
} kReadAction;

struct leaderboardEntry {
    char name[20];
    short age;
    kSex sex;
    int timeSpent;
};

class io {
private:
    std::vector<leaderboardEntry> _newLb(int tableSize, int gameId, leaderboardEntry newEntry);
    
public:
    bool saveNewTable(table *aTable, const char* name, short age, kSex sex, int timeInSeconds);
    
    int numberOfSavedGames(int puzzleWH);
    
    bool createPuzzleNumbersFile();
    
    bool addGame(int boardSize);
    
    table * loadTable(int tableSize, int gameId);
    
    std::vector<leaderboardEntry> tableScores(int tableSize, int gameId);
    
    bool updateTable(int tableSize, int gameId, leaderboardEntry newEntry);
};

#endif /* defined(__Puzzle__io__) */
