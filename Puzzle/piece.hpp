//
//  piece.h
//  Puzzle
//

#ifndef __Puzzle__piece__
#define __Puzzle__piece__

#include <iostream>

//#include "table.h"

class table;

class piece {
    
public:
    
    int x, y, value;
    
    piece(class table *the_table, int pos_x, int pos_y, int value);
    piece();
    
    table *parent_table;
    
    bool set_pos(int pos_x, int pos_y);
    bool set_value(int the_value);
    
    void set_parent_table(table *the_table);
};

#endif /* defined(__Puzzle__piece__) */
