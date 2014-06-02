//
//  piece.cpp
//  Puzzle
//

#include "piece.hpp"

#include "table.hpp"

piece::piece() {
    
}

piece::piece(class table *the_parent_table, int pos_x, int pos_y, int the_value) {
    set_parent_table(the_parent_table);
    set_pos(pos_x, pos_y);
    set_value(the_value);
}

bool piece::set_pos(int pos_x, int pos_y) {
    if (parent_table)
        if (pos_x > parent_table->table_size || pos_x < 1 || pos_y > parent_table->table_size || pos_y < 1)
            return false;
    
    x = pos_x;
    y = pos_y;
    
    return true;
}

bool piece::set_value(int the_value) {
    if (parent_table)
        if (the_value > (parent_table -> table_size * parent_table -> table_size - 1))
            return false;
    
    value = the_value;
    
    return true;
}

void piece::set_parent_table(table *the_parent_table) {
    parent_table = the_parent_table;
}
