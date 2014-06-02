//
//  table.cpp
//  Puzzle
//

#include "table.hpp"

#ifdef WIN32

#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <cstdlib>
#include <iomanip>

#endif

using namespace std;

table::table() {
    table_size = 0;
}

table::table(int thetable_size) {
    table_size = thetable_size;
    
    createPuzzle();
}

void table::revertPuzzle() {
	pieces = initialPieces;
}

bool table::puzzleIsSolved() {
    // int pieceCount = (table_size * table_size - 1);

	int currentValue = 1;

	for (int y = 1; y <= table_size; y++)
		for (int x = 1; x <= table_size; x++) {
			if (!(x == table_size && y == table_size))
				if (pieceAtPosition(x, y)->value != currentValue)
					return false;

			currentValue++;
		}

	/*for (int x = 1; x <= pieceCount; x++) {
		std::cout << pieces[x-1].value << " " << x << std::endl;
	}
    
    for (int x = 1; x <= pieceCount; x++)
        if (pieces[x - 1].value != x)
            return false;*/
    
    return true;
}

bool table::createPuzzle() {
    if (table_size < 3 || table_size > 10) {
        // Size isn't between bounds.
        
        return false;
    }
    
    pieces.clear(); // let's clear the board, else we risk having puzzles with more pieces than anyone can play with
    
    pieces.reserve(table_size * table_size);
    
    int currTableWidth = 1, currTableHeight = 1;
    
    for (int x = 1; x < table_size * table_size; x++) {
        
        if (currTableWidth > table_size) {
            currTableWidth = 1;
            currTableHeight++;
        }
        
        piece *aPiece = new piece(this, currTableWidth, currTableHeight, x);

		piece *anotherPiece = new piece(this, currTableWidth, currTableHeight, x); // Messy workaround, sorry!
        
        aPiece->set_parent_table(this);

		anotherPiece->set_parent_table(this);
        
        pieces.push_back(* aPiece);

		initialPieces.push_back(* anotherPiece);
        
        currTableWidth++;
    }
    
    return true;
}

bool table::movePiece(piece *the_piece) {
    updateWhitespace();
    
    if (movePiece(the_piece, blank_space.x, blank_space.y))
        return true;
    
    return false;
}

bool table::movePiece(piece *the_piece, int new_pos_x, int new_pos_y) {
    if (pieceExistsAtPosition(new_pos_x, new_pos_y))
        return false;
    
    the_piece -> x = new_pos_x;
    the_piece -> y = new_pos_y;
    
    updateWhitespace();
    
    return true;
}

/*
 * Using a whitespace variable is probably
 * good, as it saves us some calls to the array.
 *
 * Less calls, less cpu cycles -> Profit! :D
 */

void table::updateWhitespace() {
    for (int x = 1; x <= table_size; x++)
        for (int y = 1; y <= table_size; y++)
            if (!pieceExistsAtPosition(x, y)) {
                blank_space.x = x;
                blank_space.y = y;
            }
}

void table::_printTable() {
    for (int y = 1; y <= table_size; y++) {
        for (int x = 1; x <= table_size; x++) {
            if (pieceAtPosition(x, y) -> value != 0)
#ifdef WIN32
                cout << setw(5) << pieceAtPosition(x, y) -> value;
#else
                cout << pieceAtPosition(x, y) -> value;
#endif
            else
#ifdef WIN32
                cout << setw(5) << " ";
#else
                cout << " ";
#endif
        }
        cout << endl;
	
    }
}

bool table::shufflePuzzle() {
    srand((unsigned int)time(NULL));
    
    int lastX = 0, lastY = 0;
    
    for (int x = 0; x < 69; x++) {
        vector<piece> theVector = piecesAroundWhitespace();
        
        piece *lePiece = new piece;
        
        while (true) {
            short _randomizedIndex = (short)rand()%(theVector.size());
            
            piece *thePiece = &(theVector[_randomizedIndex]);
            
            if (thePiece->x != lastX || thePiece->y != lastY) {
                lePiece = pieceAtPosition(thePiece->x, thePiece->y);
                
                break;
            }
        }
        
        movePiece(lePiece);
        
        lastX = lePiece -> x;
        lastY = lePiece -> y;
    }
    
    startTime = (double)time(NULL);

	createCopy();
    
    return true;
}

void table::createCopy() {
	for (int i = 0; i < pieces.size(); i++) {
		initialPieces[i].x = pieces[i].x;
		initialPieces[i].y = pieces[i].y;
	}
}

void table::startTimer() {
	startTime = (double)time(NULL);
}

double table::timeSinceStart() {
    return time(NULL) - startTime;
}

bool table::pieceExistsAtPosition(int x, int y) {
    for (vector<piece>::size_type i = 0; i != pieces.size(); i++)
        if (pieces[i].x == x)
            if (pieces[i].y == y)
                return true;
    
    return false;
}

vector<piece> table::piecesAroundWhitespace() {
    updateWhitespace();
    
    vector<piece> the_vector;
    
    the_vector.clear();
    
    the_vector.reserve(4);
    
    if (pieceExistsAtPosition(blank_space.x - 1, blank_space.y))
        the_vector.push_back(* pieceAtPosition(blank_space.x - 1, blank_space.y));
    
    if (pieceExistsAtPosition(blank_space.x + 1, blank_space.y))
        the_vector.push_back(* pieceAtPosition(blank_space.x + 1, blank_space.y));
    
    if (pieceExistsAtPosition(blank_space.x, blank_space.y - 1))
        the_vector.push_back(* pieceAtPosition(blank_space.x, blank_space.y - 1));
    
    if (pieceExistsAtPosition(blank_space.x, blank_space.y + 1))
        the_vector.push_back(* pieceAtPosition(blank_space.x, blank_space.y + 1));
    
    return the_vector;
}

piece* table::pieceAtPosition(int x, int y) {
    for (vector<piece>::size_type i = 0; i != pieces.size(); i++)
        if (pieces[i].x == x)
            if (pieces[i].y == y)
                return &pieces[i];
    
    //
    // Oh wait, we're here? The piece wasn't found?
    //
    // Since somehow returning nil/null/whatever is invalid, let's just create
    // a piece with invalid/impossible values and return it.
    //
    
    return new piece(this, 0, 0, 0);
}