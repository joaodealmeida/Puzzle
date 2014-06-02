//
//  table.h
//  Puzzle
//

#ifndef __Puzzle__table__
#define __Puzzle__table__

#include <iostream>
#include <vector>

#include "piece.hpp"

struct whitespace {
    int x;
    int y;
};

class table {
    
private:
    
    void updateWhitespace();
    
    bool movePiece(piece *the_piece, int new_pos_x, int new_pos_y);
    
    std::vector<piece> piecesAroundWhitespace();
    
public:
    
    table();
    
    table(int thetable_size);
    
    
    
    std::vector<piece> initialPieces;
    
    std::vector<piece> pieces;
    
    
    void _printTable(); // this method is just to be used for testing. probably turning it private after
    
	void startTimer();

	void revertPuzzle();

	void createCopy();


    whitespace blank_space;
   

    bool createPuzzle();
    
    bool shufflePuzzle();
    
    bool movePiece(piece *the_piece);
    
    bool puzzleIsSolved();
    
    bool pieceExistsAtPosition(int x, int y);
    
    
    piece* pieceAtPosition(int x, int y);
    
    
    int table_size; // Let's probably limit this to min:3x3 (else it's too easy) and max:10x10 (memory issues, too hard, etc)
    
    
    double startTime;
    
    double timeSinceStart();
};

#endif /* defined(__Puzzle__table__) */
