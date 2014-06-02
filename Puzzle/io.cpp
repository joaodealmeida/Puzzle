//
//  io.cpp
//  Puzzle
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <string>

#include "io.h"

#include "piece.hpp"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

const char * puzzleNumbersFileName = "puzzle_numbers.txt";

bool io::saveNewTable(table *aTable, const char *name, short age, kSex sex, int timeInSeconds) {
    char tableSizeStr[25];
    
    sprintf(tableSizeStr, "puzzle_%dx%d_%d.txt", aTable->table_size, aTable->table_size, numberOfSavedGames(aTable->table_size));
    
    std::ofstream thefile (tableSizeStr);
    
    if (thefile.is_open()) {
        for (int y = 0; y < aTable->table_size; y++) {
            for (int x = 0; x < aTable->table_size; x++) {
                piece *aPiece = aTable -> pieceAtPosition(x+1, y+1);
                
                thefile << aPiece -> value;
                
                thefile << "\t";
            }
            
            thefile << "\n";
        }
        
        thefile << "\n";
        
        std::string sexChar;
        
        if (sex)
            sexChar = "F";
        else
            sexChar = "M";
        
        thefile << name << "\t" << age << "\t" << sexChar << "\t" << timeInSeconds;
        
        thefile.close();
        
        addGame(aTable->table_size);
        
        return true;
    } else {
        std::cout << "Unable to open file for writing.";
        
        return false;
    }
}

int io::numberOfSavedGames(int puzzleWH) {
    std::ifstream ifile (puzzleNumbersFileName);
    
    std::string line;
    
    while (puzzleWH > 3) {
        std::getline(ifile, line);
        
        puzzleWH--;
    }
    
    std::getline(ifile, line);
    
    int lineIndex = (int)line.find(" ") + 1;
    int lineSize = (int)line.size();
    
    std::string timesPlayedStr(line.c_str() + lineIndex, lineSize - lineIndex);
    
    return atoi(timesPlayedStr.c_str());
}

bool io::createPuzzleNumbersFile() {
    std::ifstream ifile (puzzleNumbersFileName);
    
    if (ifile)
        return false; // The file already exists. Let's just stop here.
    
    std::ofstream ofile (puzzleNumbersFileName);
    
    if (ofile) {
        for (int i = 3; i < 11; i++)
            ofile << i << "x" << i << " " << 0 << (i != 10 ? "\n" : "");
        
        // It's unnecessary to create a new line on the last possible puzzle type, so let's just not do it. We can even save a bit of disk space this way! :D (even though it's just some tiny tiny bits :p)
        
        return true; // My work here is done!
    }
    
    return false;
}

bool io::addGame(int boardSize) {
    std::vector<int> gamesPlayedOfEachType;
    
    for (int i = 3; i <= 10; i++) {
        gamesPlayedOfEachType.push_back(numberOfSavedGames(i));
    }
    
    gamesPlayedOfEachType[boardSize - 3] = gamesPlayedOfEachType[boardSize - 3]++;
    
    std::remove(puzzleNumbersFileName);
    
    std::ofstream ofile (puzzleNumbersFileName);
    
    if (ofile) {
        for (int i = 3; i < 11; i++)
            ofile << i << "x" << i << " " << gamesPlayedOfEachType[i - 3] << (i != 10 ? "\n" : "");
        
        // It's unnecessary to create a new line on the last possible puzzle type, so let's just not do it. We can even save a bit of disk space this way! :D (even though it's just some tiny tiny bits :p)
        
        return true; // My work here is done!
    }
    
    std::cout << "Unable to open file for writing!" << std::endl;
    
    return false;
}

table * io::loadTable(int tableSize, int gameId) {
    table *aTable = new table(tableSize);
    
    char tableFileName[30];
    
    sprintf(tableFileName, "puzzle_%dx%d_%d.txt", tableSize, tableSize, gameId);
    
    std::ifstream ifile (tableFileName);
    
    std::string line;

	int indx = 0;
    
    int y = 1;
    
    while (std::getline(ifile, line)) {
        int x = 1;
        
        if (line.compare("\n") == 0)
            break;
        
        int currIndex = 0;
        
        std::string numberStr = "";
        
        while (currIndex < (int)(line.length())) {
            char character = line[currIndex];
            
            if (atoi(&character) > 0 || isalnum(character)) {
                numberStr += character;
                
                currIndex++;
            } else {
				if (atoi(numberStr.c_str()) == 0)
					aTable -> pieceAtPosition(x, y) -> set_pos(tableSize, tableSize);
				else
					aTable -> pieceAtPosition(x, y) -> set_value(atoi(numberStr.c_str()));

				x++;
                
                numberStr.clear();
                
                currIndex++;
            }
        }
        
        y++;
    }
    
    return aTable;
}

std::vector<leaderboardEntry> io::tableScores(int tableSize, int gameId) {
    std::vector<leaderboardEntry> leaderboard;
    
    char tableFileName[30];
    
    sprintf(tableFileName, "puzzle_%dx%d_%d.txt", tableSize, tableSize, gameId);
    
    std::ifstream ifile (tableFileName);
    
    std::string line;
    
    for (int i = 0; i < (tableSize + 1); i++) {
        std::getline(ifile, line);
    }
    
    while (std::getline(ifile, line)) {
		line += "\t";

        std::cout << line << std::endl;
        
        int currIndex = 0;
        
        std::string readStr = "";
        
        kReadAction currentAction = kReadName;
        
        leaderboardEntry le;
        
        while (currIndex < (int)(line.length())) {
            char character = line[currIndex];
            
            if (!iscntrl(character)) {
                readStr += character;
                
                currIndex++;
            } else {
                switch (currentAction) {
                    case kReadName:
						char theName[20];
						sprintf(le.name, readStr.c_str());
						currentAction = kReadAge;
                        break;
                        
                    case kReadAge:
                        le.age = atoi(readStr.c_str());
						currentAction = kReadSex;
                        break;
                        
                    case kReadSex:
                        if (strcmp(readStr.c_str(), "M") == 0)
                            le.sex = kSexMale;
                        else
                            le.sex = kSexFemale;
						currentAction = kReadTime;
                        break;
                        
                    case kReadTime:
                        le.timeSpent = atoi(readStr.c_str()); 
                        break;
                        
                    default:
                        break;
                }
                
                readStr = "";
                
                currIndex++;
            }
        }

		std::cout << le.name << " " << le.age << " " << le.sex << " " << le.timeSpent << std::endl;
        
        leaderboard.push_back(le);
	}
    
    return leaderboard;
}

std::vector<leaderboardEntry> io::_newLb(int tableSize, int gameId, leaderboardEntry newEntry) {
    std::vector<leaderboardEntry> lb = tableScores(tableSize, gameId);
    
    std::vector<leaderboardEntry> newlb;

	bool enteredValue = false;
    
    for (int i = 0; i < (int)(lb.size()); i++) {
        leaderboardEntry entry = lb.at(i);
        
        /*if (newEntry.timeSpent < entry.timeSpent) {
            for (int x = 0; x <= i; x++)
                newlb.push_back(lb.at(x));
            
            newlb.push_back(newEntry);
            
            for (int x = (int)newlb.size(); x < (int)(lb.size()); x++)
                newlb.push_back(lb.at(x));
            
            jobDone = true;
        }*/

		if (entry.timeSpent > newEntry.timeSpent)
			if (!enteredValue) {
				newlb.push_back(newEntry);
				enteredValue = true;
			}

		newlb.push_back(entry);
    }
    
    return newlb;
}

bool io::updateTable(int tableSize, int gameId, leaderboardEntry newEntry) {
    std::vector<leaderboardEntry> newlb = _newLb(tableSize, gameId, newEntry);
    
    table *aTable = loadTable(tableSize, gameId);
    
    char tableSizeStr[25];
    
    sprintf(tableSizeStr, "puzzle_%dx%d_%d.txt", aTable->table_size, aTable->table_size, gameId);

	std::remove(tableSizeStr);
    
    std::ofstream thefile (tableSizeStr);
    
    if (thefile.is_open()) {
        for (int y = 0; y < aTable->table_size; y++) {
            for (int x = 0; x < aTable->table_size; x++) {
                piece *aPiece = aTable -> pieceAtPosition(x+1, y+1);
                
                thefile << aPiece -> value;
                
                thefile << "\t";
            }
            
            thefile << "\n";
        }
        
        thefile << "\n";

		for (int i = 0; i < (int)(newlb.size()); i++) {
			std::string sexStr = (newlb[i].sex ? "F" : "M");

			thefile << newlb[i].name << "\t" << newlb[i].age << "\t" << sexStr << "\t" << newlb[i].timeSpent;

			if ((i + 1) < (int)(newlb.size()))
				thefile << std::endl;
		}
        
        thefile.close();
        
        return true;
    } else {
        std::cout << "Unable to open file for writing.";
        
        return false;
    }
}
